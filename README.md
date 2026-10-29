# Cidades e Vizinhanças em uma Estrada

Trabalho acadêmico em C que lê cidades e suas posições ao longo de uma estrada, organiza os dados em uma lista encadeada e calcula a menor vizinhança e a cidade correspondente.

## Conceitos praticados

- Structs, ponteiros e alocação dinâmica
- Listas encadeadas
- Leitura de arquivos e validação de entrada
- Ordenação com `qsort`

## Executar no Windows

O [main.c](main.c) utiliza `windows.h` e funções de configuração do console do Windows. Com GCC/MinGW disponível, execute na raiz:

```powershell
gcc main.c cidades.c -o cidades.exe
.\cidades.exe teste01.txt
```

Sem argumento, o programa utiliza `teste03.txt`.

## Formato da entrada

A primeira linha informa o tamanho da estrada (`T`), a segunda informa a quantidade de cidades (`N`) e as demais contêm posição e nome:

```text
10
2
8 Birnin Zana
5 Birnin SYan
```

O exemplo corresponde a [teste01.txt](teste01.txt). As restrições verificadas por `getEstrada` incluem `3 ≤ T ≤ 1000000`, `2 ≤ N ≤ 10000`, posições internas à estrada e ausência de posições duplicadas.

## Arquivos

| Caminho | Conteúdo |
| --- | --- |
| [main.c](main.c) | Entrada do programa e exibição dos resultados |
| [cidades.h](cidades.h) | Structs e funções públicas |
| [cidades.c](cidades.c) | Leitura, ordenação e cálculos |
| `teste01.txt` a `teste05.txt` | Entradas de exemplo |
| `teste-erro01.txt` a `teste-erro06.txt` | Entradas para exercitar validações |

## Integrantes

- Hugo Nunes
- Nicholas Rafael
- Louie Felipe

## Licença

Consulte [LICENSE](LICENSE).

## Leitura e testes atualizados

A leitura é compartilhada pelas funções públicas: aceita nomes com espaços, rejeita campos longos, valida as posições e trata falhas de alocação. O console específico do Windows fica condicionado a `_WIN32`; o programa também compila no Linux.

```bash
gcc -std=c11 -Wall -Wextra -I. cidades.c tests/test_cidades.c -lm -o teste
./teste
gcc -std=c11 main.c cidades.c -o cidades
./cidades teste01.txt
```

## Guia do código

Consulte [GUIA_CODIGO.md](GUIA_CODIGO.md) para localizar os módulos, entender as convenções e estudar as alterações.
