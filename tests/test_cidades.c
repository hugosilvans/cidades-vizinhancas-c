#include "cidades.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *cidade = cidadeMenorVizinhanca("teste01.txt");
    assert(cidade && strcmp(cidade, "Birnin Zana") == 0);
    assert(fabs(calcularMenorVizinhanca("teste01.txt") - 3.5) < 0.001);
    free(cidade);
    assert(getEstrada("arquivo-inexistente.txt") == NULL);
    for (int i = 1; i <= 6; i++) {
        char caminho[40];
        snprintf(caminho, sizeof caminho, "teste-erro%02d.txt", i);
        assert(calcularMenorVizinhanca(caminho) < 0);
    }
    return 0;
}
