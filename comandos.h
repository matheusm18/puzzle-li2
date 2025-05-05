#ifndef COMANDOS_H
#define COMANDOS_H

#include "estado.h"

typedef bool (*Comando)(char cmd, char *arg, Estado *e);

bool cmdLer(char cmd, char *arg, Estado *e);
bool cmdGravar(char cmd, char *arg, Estado *e);
bool cmdSair(char cmd, char *arg, Estado *e);
bool cmdPintar(char cmd, char *arg, Estado *e);
bool cmdRiscar(char cmd, char *arg, Estado *e);
bool cmdUndo(char cmd, char *arg, Estado *e);
bool cmdVerificarRestricoes(char cmd, char *arg, Estado *e);
bool cmdAjudar(char cmd, char *arg, Estado *e);
bool cmdA(char cmd, char *arg, Estado *e);
bool cmdResolver(char cmd, char *arg, Estado *e);
bool cmdHelp(char cmd, char *arg, Estado *e);
bool cmdStatus(char cmd, char *arg, Estado *e);

#endif