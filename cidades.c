#include "cidades.h"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int posicao;
    char nome[256];
} CidadeLida;

static void liberarEstrada(Estrada *estrada) {
    if (!estrada)
        return;
    Cidade *cidade = estrada->Inicio;
    while (cidade) {
        Cidade *proxima = cidade->Proximo;
        free(cidade);
        cidade = proxima;
    }
    free(estrada);
}

static int comparar(const void *a, const void *b) {
    int x = ((const CidadeLida *)a)->posicao;
    int y = ((const CidadeLida *)b)->posicao;
    return (x > y) - (x < y);
}

/* strtol permite detectar números fora do intervalo, ao contrário de atoi. */
static int lerInteiro(char **texto, int *valor) {
    char *fim;
    long numero = strtol(*texto, &fim, 10);
    if (fim == *texto || numero < INT_MIN || numero > INT_MAX)
        return 0;
    *texto = fim;
    *valor = (int)numero;
    return 1;
}

Estrada *getEstrada(const char *nomeArquivo) {
    if (!nomeArquivo)
        return NULL;
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo)
        return NULL;

    char linha[1024], *texto;
    int tamanho, quantidade;
    CidadeLida *dados = NULL;
    Estrada *estrada = NULL;

    if (!fgets(linha, sizeof linha, arquivo))
        goto falha;
    texto = linha;
    if (!lerInteiro(&texto, &tamanho) || tamanho < 3 || tamanho > 1000000)
        goto falha;
    while (isspace((unsigned char)*texto))
        texto++;
    if (*texto)
        goto falha;
    if (!fgets(linha, sizeof linha, arquivo))
        goto falha;
    texto = linha;
    if (!lerInteiro(&texto, &quantidade) || quantidade < 2 || quantidade > 10000)
        goto falha;
    while (isspace((unsigned char)*texto))
        texto++;
    if (*texto)
        goto falha;

    dados = calloc((size_t)quantidade, sizeof *dados);
    if (!dados)
        goto falha;
    for (int i = 0; i < quantidade; i++) {
        if (!fgets(linha, sizeof linha, arquivo))
            goto falha;
        /* Uma linha truncada não pode virar um nome parcial silenciosamente. */
        if (!strchr(linha, '\n') && !feof(arquivo))
            goto falha;
        texto = linha;
        if (!lerInteiro(&texto, &dados[i].posicao))
            goto falha;
        if (!isspace((unsigned char)*texto))
            goto falha;
        while (isspace((unsigned char)*texto))
            texto++;
        size_t comprimento = strlen(texto);
        while (comprimento && isspace((unsigned char)texto[comprimento - 1]))
            comprimento--;
        if (!comprimento || comprimento >= sizeof dados[i].nome)
            goto falha;
        memcpy(dados[i].nome, texto, comprimento);
        dados[i].nome[comprimento] = '\0';
        if (dados[i].posicao <= 0 || dados[i].posicao >= tamanho)
            goto falha;
    }
    /* Ordenar uma vez permite validar duplicatas comparando apenas vizinhos. */
    qsort(dados, (size_t)quantidade, sizeof *dados, comparar);
    for (int i = 1; i < quantidade; i++) {
        if (dados[i - 1].posicao == dados[i].posicao)
            goto falha;
    }
    estrada = calloc(1, sizeof *estrada);
    if (!estrada)
        goto falha;
    estrada->N = quantidade;
    estrada->T = tamanho;
    Cidade **destino = &estrada->Inicio;
    for (int i = 0; i < quantidade; i++) {
        Cidade *cidade = calloc(1, sizeof *cidade);
        if (!cidade)
            goto falha;
        cidade->Posicao = dados[i].posicao;
        memcpy(cidade->Nome, dados[i].nome, sizeof cidade->Nome);
        *destino = cidade;
        destino = &cidade->Proximo;
    }
    free(dados);
    fclose(arquivo);
    return estrada;

falha:
    free(dados);
    liberarEstrada(estrada);
    fclose(arquivo);
    return NULL;
}

/* Uma única regra atende ao cálculo numérico e à identificação da cidade. */
static Cidade *menorVizinhanca(Estrada *estrada, double *menor) {
    Cidade *anterior = NULL, *escolhida = NULL;
    *menor = (double)estrada->T + 1;
    for (Cidade *atual = estrada->Inicio; atual; atual = atual->Proximo) {
        double esquerda = anterior ? (anterior->Posicao + atual->Posicao) / 2.0 : 0;
        double direita =
            atual->Proximo ? (atual->Posicao + atual->Proximo->Posicao) / 2.0 : estrada->T;
        double comprimento = direita - esquerda;
        if (comprimento < *menor) {
            *menor = comprimento;
            escolhida = atual;
        }
        anterior = atual;
    }
    return escolhida;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return -1.0;
    double menor;
    menorVizinhanca(estrada, &menor);
    liberarEstrada(estrada);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada)
        return NULL;
    double menor;
    Cidade *cidade = menorVizinhanca(estrada, &menor);
    /* O chamador recebe sua própria cópia e deve liberá-la com free. */
    char *nome = malloc(strlen(cidade->Nome) + 1);
    if (nome)
        strcpy(nome, cidade->Nome);
    liberarEstrada(estrada);
    return nome;
}
