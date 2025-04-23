#include "estado.h"

// coloco o estado de agora em cima da stack
void guardarEstado(Estado *atual) {
    Estado *estadoGuardar = malloc(sizeof(Estado));
    if (estadoGuardar != NULL) {
        memcpy(estadoGuardar, atual, sizeof(Estado));
        estadoGuardar->ultimoEstado = atual->ultimoEstado;
        atual->ultimoEstado = estadoGuardar;
    }
}

void liberarEstados(Estado *e) {
    while (e != NULL) {
        Estado *anterior = e->ultimoEstado;
        free(e);
        e = anterior;
    }
}

void mostrarTabuleiro(Estado *e) {
    printf("\n");
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            printf("%c ", e->tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool comparaTabuleiros(Estado *e1, Estado *e2) {
    if (e1->linhas != e2->linhas || e1->colunas != e2->colunas) return false;
    for (int i = 0; i < e1->linhas; i++) {
        for (int j = 0; j < e1->colunas; j++) {
            if (e1->tabuleiro[i][j] != e2->tabuleiro[i][j]) return false;
        }
    }
    return true;
}

int tabuleiroResolvido(Estado *e) {
    if (!e->carregouTabuleiro) return false;
    e->printar = false; // desativa a flag de printar
    cmdVerificarRestricoes('v', NULL, e);

    if (e->temInfracoes) {
        e->printar = true; // reativa a flag de printar
        return 0;
    }

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') return 2;
        }
    }
    e->printar = true; // reativa a flag de printar
    return 1;
}