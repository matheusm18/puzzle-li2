#ifndef ESTADO_H
#define ESTADO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define TMAX_MATRIZ 26

// cores ANSI para poder mudar a cor do texto no terminal
#define COLOR_GRAY   "\x1b[90m"
#define COLOR_RED    "\x1b[31m"
#define COLOR_CYAN   "\x1b[1;36m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_WHITE  "\x1b[1;37m"
#define COLOR_RESET  "\x1b[0m"

typedef struct estado {
    char tabuleiro[TMAX_MATRIZ][TMAX_MATRIZ];
    int linhas, colunas;
    bool carregouTabuleiro;
    bool looping;
    bool temInfracoes; // flag que sinaliza se há infrações no tabuleiro
    bool guardarEstados; // flag que sinaliza se é pra guardar estados antes dos comandos
    bool printar; // flag que sinaliza se é pra printar informações (separa a lógica da interface)
    bool deuAviso; // flag para avisar se já houve um aviso (printf) na função permitindo distinguir erros de comandos não reconhecidos
    struct estado *ultimoEstado;
} Estado;

bool cmdVerificarRestricoes(char cmd, char *arg, Estado *e);
void guardarEstado(Estado *atual);
void liberarEstados(Estado *e);
void mostrarTabuleiro(Estado *e);
bool comparaTabuleiros(Estado *e1, Estado *e2);
int tabuleiroResolvido(Estado *e);

#endif