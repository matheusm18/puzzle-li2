#include "estado.h"

// colocar o estado de agora em cima da stack (push)
void guardarEstado(Estado *atual) {
    Estado *estadoGuardar = malloc(sizeof(Estado));
    if (estadoGuardar != NULL) {
        memcpy(estadoGuardar, atual, sizeof(Estado));
        estadoGuardar->ultimoEstado = atual->ultimoEstado;
        atual->ultimoEstado = estadoGuardar;
    }
}

// liberar a memoria de todos os estados
void liberarEstados(Estado *e) {
    while (e != NULL) {
        Estado *anterior = e->ultimoEstado;
        free(e);
        e = anterior;
    }
}

// print do tabuleiro
void mostrarTabuleiro(Estado *e) {
    int L = e->linhas, C = e->colunas;

    // cabecalho para as letras (primeira linha)
    printf("\n     ");
    for (int j = 0; j < C; j++) {
        printf(" %c", 'a' + j);
    }
    printf("\n");

    // linha para separar
    printf("     ");
    for (int j = 0; j < C; j++) {
        printf(" ─");
    }
    printf("\n");

    // imprimir cada linha (inclui o numero da linha, a barra e a linha do tabuleiro)
    for (int i = 0; i < L; i++) {
        // num da linha e barra
        printf(" %2d |", i);

        // imprimir cada coluna da linha com a devida cor (extra)
        for (int j = 0; j < C; j++) {
            char atual = e->tabuleiro[i][j];
            if (atual == '#') {
                printf(COLOR_RED " %c" COLOR_RESET, atual);
            }
            else if (isupper(atual)) {
                printf(" %c", atual);
            }
            else {
                printf(COLOR_GRAY " %c" COLOR_RESET, atual);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// comparar tabuleiros
bool comparaTabuleiros(Estado *e1, Estado *e2) {
    if (e1->linhas != e2->linhas || e1->colunas != e2->colunas) return false;
    for (int i = 0; i < e1->linhas; i++) {
        for (int j = 0; j < e1->colunas; j++) {
            if (e1->tabuleiro[i][j] != e2->tabuleiro[i][j]) return false;
        }
    }
    return true;
}

// verifica se o tabuleiro está resolvido
int tabuleiroResolvido(Estado *e) {
    
    e->printar = false; // desativa a flag de printar
    cmdVerificarRestricoes('v', NULL, e);

    if (e->temInfracoes) {
        e->printar = true; // reativa a flag de printar
        return 0; // retorna 0 se o tabuleiro não pode ser resolvido
    }

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') return 2; // retorna 2 se ainda houver letras minúsculas
        }
    }
    e->printar = true; // reativa a flag de printar
    return 1; // retorna 1 se o tabuleiro está resolvido
}