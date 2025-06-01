#define TESTING
#include <CUnit/Basic.h>
#include "comandos.h"

void testar_cmdLer_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = '#';
    e->tabuleiro[1][0] = '#'; e->tabuleiro[1][1] = 'B';

    FILE *file = fopen("lerValido.txt", "w"); // modo write cria o arquivo se n existir, se existir escreve por cima
    fprintf(file, "2 2\nab\ncd\n");
    fclose(file);

    CU_ASSERT_TRUE(cmdLer('l', "lerValido.txt", e));

    CU_ASSERT_TRUE(e->carregouTabuleiro);
    CU_ASSERT_TRUE(e->deuAviso);
    CU_ASSERT_EQUAL(e->linhas, 2);
    CU_ASSERT_EQUAL(e->colunas, 2);
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'c');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'd');

    remove("lerValido.txt"); // remover o arquivo apos o teste
    liberarEstados(e); // liberar a memória alocada
}

void testar_cmdLer_semArgumento() {
    Estado e = {0};
    CU_ASSERT_FALSE(cmdLer('l', NULL, &e));
    CU_ASSERT_TRUE(e.deuAviso);
}

void testar_cmdLer_ficheiroInvalido() {
    Estado e = {0};
    CU_ASSERT_FALSE(cmdLer('l', "ficheiroinvalido.txt", &e));
    CU_ASSERT_FALSE(e.carregouTabuleiro);
}

// formato do tabuleiro invalido
void testar_cmdLer_formatoInvalido() {
    Estado e = {0};
    FILE *file = fopen("formatoInvalido.txt", "w");
    fprintf(file, "3\nabc\ndef\nghi\n");
    fclose(file);

    CU_ASSERT_FALSE(cmdLer('l', "formatoInvalido.txt", &e));
    CU_ASSERT_FALSE(e.carregouTabuleiro);

    remove("formatoInvalido.txt");
}

void testar_cmdLer_tabuleiroInvalido() {
    Estado e = {0};
    FILE *file = fopen("tabuleiroInvalido.txt", "w");
    fprintf(file, "-1 -1\nab\ncd\n");
    fclose(file);

    CU_ASSERT_FALSE(cmdLer('l', "tabuleiroInvalido.txt", &e));
    CU_ASSERT_FALSE(e.carregouTabuleiro);

    remove("tabuleiroInvalido.txt");
}

void testar_cmdLer_linhaInvalida() {
    Estado e = {0};
    FILE *file = fopen("linhaInvalida.txt", "w");
    fprintf(file, "2 2\nab\n"); // era suposto ter 2 linhas e so tem uma
    fclose(file);
    
    CU_ASSERT_FALSE(cmdLer('l', "linhaInvalida.txt", &e));
    CU_ASSERT_FALSE(e.carregouTabuleiro);
    
    remove("linhaInvalida.txt");
}

void testar_cmdLer_linhaInvalida2() {
    Estado e = {0};
    FILE *file = fopen("linhaInvalida2.txt", "w");
    fprintf(file, "2 3\na\ncde\n"); // linha 1 tem apenas 1 char e nao 3
    fclose(file);
    
    CU_ASSERT_FALSE(cmdLer('l', "linhaInvalida2.txt", &e));
    CU_ASSERT_FALSE(e.carregouTabuleiro);
    
    remove("linhaInvalida2.txt");
}

void testar_cmdGravar_valido() {
    Estado e = {0};
    e.printar = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = 'a'; e.tabuleiro[0][1] = 'b';
    e.tabuleiro[1][0] = 'c'; e.tabuleiro[1][1] = 'd';

    CU_ASSERT_TRUE(cmdGravar('g', "gravarValido.txt", &e));

    FILE *file = fopen("gravarValido.txt", "r");
    char linha[100];
    if (fgets(linha, sizeof(linha), file) != NULL) CU_ASSERT_STRING_EQUAL(linha, "2 2\n");
    if (fgets(linha, sizeof(linha), file) != NULL) CU_ASSERT_STRING_EQUAL(linha, "ab\n");
    if (fgets(linha, sizeof(linha), file) != NULL) CU_ASSERT_STRING_EQUAL(linha, "cd\n");
    
    fclose(file);
    remove("gravarValido.txt");
}

void testar_cmdGravar_invalido() {
    Estado e = {0};
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = 'a'; e.tabuleiro[0][1] = 'b';
    e.tabuleiro[1][0] = 'c'; e.tabuleiro[1][1] = 'd';

    CU_ASSERT_FALSE(cmdGravar('g', NULL, &e));
}

void testar_cmdSair_valido() {
    Estado e = {0};
    e.looping = true;
    CU_ASSERT_TRUE(cmdSair('s', NULL, &e));
    CU_ASSERT_FALSE(e.looping);
}

void testar_cmdSair_invalido() {
    Estado e = {0};
    CU_ASSERT_FALSE(cmdSair('s', "sair", &e)); // arg dif de null
}

void testar_cmdPintar_valido() {
    Estado *e = malloc(sizeof(Estado)); // aloca o estado na heap
    memset(e, 0, sizeof(Estado)); // inicializa o estado com zeros (o tamanho do bloco que sera preenchido é o tamanho do estado)

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    CU_ASSERT_TRUE(cmdPintar('b', "a0", e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');

    liberarEstados(e); // libera a memória alocada
}

void testar_cmdPintar_celulaRiscada() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = '#';

    e->printar = true;

    CU_ASSERT_FALSE(cmdPintar('b', "a0", e));
    CU_ASSERT_TRUE(e->deuAviso);
    
    liberarEstados(e);
}

void testar_cmdPintar_celulaPintada() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'A';

    e->printar = true;

    CU_ASSERT_FALSE(cmdPintar('b', "a0", e));
    CU_ASSERT_TRUE(e->deuAviso);
    
    liberarEstados(e);
}

void testar_cmdPintar_invalido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    CU_ASSERT_FALSE(cmdPintar('b', "a0", e));

    liberarEstados(e);
}

void testar_cmdPintar_coordsInvalida() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    
    // coordenadas fora do tabuleiro
    CU_ASSERT_FALSE(cmdPintar('b', "a5", e));
    CU_ASSERT_FALSE(cmdPintar('b', "z0", e));

    liberarEstados(e);
}

void testar_cmdRiscar_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    CU_ASSERT_TRUE(cmdRiscar('r', "a0", e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], '#');

    liberarEstados(e);
}

void testar_cmdRiscar_celulaRiscada() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = '#';

    e->printar = true;

    CU_ASSERT_FALSE(cmdRiscar('r', "a0", e));
    CU_ASSERT_TRUE(e->deuAviso);
    
    liberarEstados(e);
}

void testar_cmdRiscar_invalido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    CU_ASSERT_FALSE(cmdRiscar('x', "a0", e)); // comando inválido

    liberarEstados(e);
}

void testar_cmdRiscar_coordsInvalida() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->printar = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;

    CU_ASSERT_FALSE(cmdRiscar('r', "a5", e));
    CU_ASSERT_FALSE(cmdRiscar('r', "z0", e));

    liberarEstados(e);
}

void testar_cmdUndo_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    cmdRiscar('r', "a0", e);

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], '#');
    CU_ASSERT_TRUE(cmdUndo('d', NULL, e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'a');

    liberarEstados(e);
}


void testar_cmdUndo_ultimoEstadoNULL() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    CU_ASSERT_FALSE(cmdUndo('d', NULL, e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'a');

    liberarEstados(e);
}

void testar_cmdVerificar_verifValida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_FALSE(e.temInfracoes);
}


void testar_cmdVerificar_linhaInvalida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'A';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_colunaInvalida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'C';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_riscadaDireita() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = '#';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_riscadaBaixo() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_semCaminho() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.printar = true;

    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = '#';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'B';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdAjudar_invalido() {
    Estado e = {0};

    CU_ASSERT_FALSE(cmdAjudar('a', "ajudar", &e)); // arg invalido
    CU_ASSERT_FALSE(cmdAjudar('a', NULL, &e)); // sem carregar tabuleiro
}

void testar_cmdAjudar_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'a';
    e->tabuleiro[1][0] = 'd'; e->tabuleiro[1][1] = 'b';


    CU_ASSERT_TRUE(cmdAjudar('a', NULL, e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'B');

    liberarEstados(e);
}

void testar_cmdAjudar_valido2() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'd';
    e->tabuleiro[1][0] = 'a'; e->tabuleiro[1][1] = 'b';


    CU_ASSERT_TRUE(cmdAjudar('a', NULL, e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'B');

    liberarEstados(e);
}

void testar_cmdAjudar_semAlteracoes() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'D';
    e->tabuleiro[1][0] = '#'; e->tabuleiro[1][1] = 'B';


    CU_ASSERT_TRUE(cmdAjudar('a', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'B');

    liberarEstados(e);
}

void testar_cmdAjudar_comInfracoes() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'A';
    e->tabuleiro[1][0] = 'm'; e->tabuleiro[1][1] = '#';


    CU_ASSERT_TRUE(cmdAjudar('a', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'm');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], '#');

    liberarEstados(e);
}

void testar_cmdAjudar_gerouInfracoes() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'a';
    e->tabuleiro[1][0] = 'm'; e->tabuleiro[1][1] = '#';


    CU_ASSERT_TRUE(cmdAjudar('a', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'm');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], '#');

    liberarEstados(e);
}

void testar_cmdA_invalido() {
    Estado e = {0};

    CU_ASSERT_FALSE(cmdA('A', "ajudar", &e)); // arg invalido
    CU_ASSERT_FALSE(cmdA('A', NULL, &e)); // sem carregar tabuleiro
}

void testar_cmdA_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 3; e->colunas = 3;

    e->tabuleiro[0][0] = 'e'; e->tabuleiro[0][1] = 'c'; e->tabuleiro[0][2] = 'a';
    e->tabuleiro[1][0] = 'd'; e->tabuleiro[1][1] = 'h'; e->tabuleiro[1][2] = 'f';
    e->tabuleiro[2][0] = 'B'; e->tabuleiro[2][1] = 'c'; e->tabuleiro[2][2] = 'b';

    CU_ASSERT_TRUE(cmdA('A', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'E');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[0][2], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'H');
    CU_ASSERT_EQUAL(e->tabuleiro[1][2], 'F');
    CU_ASSERT_EQUAL(e->tabuleiro[2][0], 'B');
    CU_ASSERT_EQUAL(e->tabuleiro[2][1], 'C');
    CU_ASSERT_EQUAL(e->tabuleiro[2][2], '#');

    liberarEstados(e);
}

void testar_cmdA_gerouInfracoes() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 3; e->colunas = 3;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'a'; e->tabuleiro[0][2] = 'b';
    e->tabuleiro[1][0] = 'c'; e->tabuleiro[1][1] = 'a'; e->tabuleiro[1][2] = 'b';
    e->tabuleiro[2][0] = 'a'; e->tabuleiro[2][1] = 'b'; e->tabuleiro[2][2] = 'c';


    CU_ASSERT_TRUE(cmdA('A', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[0][2], 'b');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'c');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[1][2], 'b');
    CU_ASSERT_EQUAL(e->tabuleiro[2][0], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[2][1], 'b');
    CU_ASSERT_EQUAL(e->tabuleiro[2][2], 'c');

    liberarEstados(e);
}

void testar_cmdA_semAlteracoes() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'D';
    e->tabuleiro[1][0] = '#'; e->tabuleiro[1][1] = 'B';


    CU_ASSERT_TRUE(cmdA('A', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'B');

    liberarEstados(e);
}

void testar_cmdResolver_invalido() {
    Estado e = {0};

    CU_ASSERT_FALSE(cmdResolver('R', "resolver", &e)); // arg invalido
    CU_ASSERT_FALSE(cmdResolver('R', NULL, &e)); // sem carregar tabuleiro
}

void testar_cmdResolver_impossivel() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'A'; e->tabuleiro[0][1] = 'A';
    e->tabuleiro[1][0] = 'm'; e->tabuleiro[1][1] = 'b';

    CU_ASSERT_TRUE(cmdResolver('R', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'm');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'b');

    liberarEstados(e);
}

void testar_cmdResolver_tabuleiroResolvido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 3; e->colunas = 3;

    e->tabuleiro[0][0] = 'E'; e->tabuleiro[0][1] = '#'; e->tabuleiro[0][2] = 'A';
    e->tabuleiro[1][0] = 'D'; e->tabuleiro[1][1] = 'H'; e->tabuleiro[1][2] = 'F';
    e->tabuleiro[2][0] = 'B'; e->tabuleiro[2][1] = 'C'; e->tabuleiro[2][2] = '#';

    CU_ASSERT_TRUE(cmdResolver('R', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'E');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[0][2], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'H');
    CU_ASSERT_EQUAL(e->tabuleiro[1][2], 'F');
    CU_ASSERT_EQUAL(e->tabuleiro[2][0], 'B');
    CU_ASSERT_EQUAL(e->tabuleiro[2][1], 'C');
    CU_ASSERT_EQUAL(e->tabuleiro[2][2], '#');

    liberarEstados(e);
}

void testar_cmdResolver_tabuleiroImpossivel() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 2; e->colunas = 2;

    e->tabuleiro[0][0] = 'a'; e->tabuleiro[0][1] = 'b';
    e->tabuleiro[1][0] = 'a'; e->tabuleiro[1][1] = 'b';

    CU_ASSERT_TRUE(cmdResolver('R', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'a');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'b');

    liberarEstados(e);
}

void testar_cmdResolver_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
    e->guardarEstados = true;
    e->printar = true;
    e->linhas = 3; e->colunas = 3;

    e->tabuleiro[0][0] = 'e'; e->tabuleiro[0][1] = 'c'; e->tabuleiro[0][2] = 'a';
    e->tabuleiro[1][0] = 'd'; e->tabuleiro[1][1] = 'h'; e->tabuleiro[1][2] = 'f';
    e->tabuleiro[2][0] = 'b'; e->tabuleiro[2][1] = 'c'; e->tabuleiro[2][2] = 'b';

    CU_ASSERT_TRUE(cmdResolver('R', NULL, e));

    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'E');
    CU_ASSERT_EQUAL(e->tabuleiro[0][1], '#');
    CU_ASSERT_EQUAL(e->tabuleiro[0][2], 'A');
    CU_ASSERT_EQUAL(e->tabuleiro[1][0], 'D');
    CU_ASSERT_EQUAL(e->tabuleiro[1][1], 'H');
    CU_ASSERT_EQUAL(e->tabuleiro[1][2], 'F');
    CU_ASSERT_EQUAL(e->tabuleiro[2][0], 'B');
    CU_ASSERT_EQUAL(e->tabuleiro[2][1], 'C');
    CU_ASSERT_EQUAL(e->tabuleiro[2][2], '#');

    liberarEstados(e);
}

void testar_cmdHelp_valido() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdHelp('h', NULL, &e));
}

void testar_cmdHelp_invalido() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_FALSE(cmdHelp('h', "help", &e));
}

void testar_cmdStatus_valido() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.printar = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'A';
    e.tabuleiro[1][0] = 'B'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdStatus('S', NULL, &e));
}

void testar_cmdStatus_comInfracoes() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.printar = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdStatus('S', NULL, &e));
}

void testar_cmdStatus_invalido() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.printar = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_FALSE(cmdStatus('s', NULL, &e)); // inseriu 's' em vez de 'S'
    CU_ASSERT_FALSE(cmdStatus('S', "status", &e)); // passou arg
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Testes do Projeto", NULL, NULL); // primeiro NULL é a funcao de inicializacao(setup), segundo é a de limpeza

    CU_add_test(suite, "cmdLer - Arquivo valido", testar_cmdLer_valido);
    CU_add_test(suite, "cmdLer - Sem argumento", testar_cmdLer_semArgumento);
    CU_add_test(suite, "cmdLer - Arquivo inexistente", testar_cmdLer_ficheiroInvalido);
    CU_add_test(suite, "cmdLer - Formato invalido", testar_cmdLer_formatoInvalido);
    CU_add_test(suite, "cmdLer - Linha incompleta", testar_cmdLer_linhaInvalida);
    CU_add_test(suite, "cmdLer - Menos linhas", testar_cmdLer_linhaInvalida2);
    CU_add_test(suite, "cmdLer - Dimensões inválidas", testar_cmdLer_tabuleiroInvalido);
    CU_add_test(suite, "cmdGravar - Comando válido", testar_cmdGravar_valido);
    CU_add_test(suite, "cmdGravar - Comando inválido", testar_cmdGravar_invalido);
    CU_add_test(suite, "cmdSair - Comando válido", testar_cmdSair_valido);
    CU_add_test(suite, "cmdSair - Comando inválido", testar_cmdSair_invalido);
    CU_add_test(suite, "cmdPintar - Comando válido", testar_cmdPintar_valido);
    CU_add_test(suite, "cmdPintar - Célula já riscada", testar_cmdPintar_celulaRiscada);
    CU_add_test(suite, "cmdPintar - Célula já pintada", testar_cmdPintar_celulaPintada);
    CU_add_test(suite, "cmdPintar - Comando inválido", testar_cmdPintar_invalido);
    CU_add_test(suite, "cmdPintar - Coordenadas inválidas", testar_cmdPintar_coordsInvalida);
    CU_add_test(suite, "cmdRiscar - Comando válido", testar_cmdRiscar_valido);
    CU_add_test(suite, "cmdRiscar - Célula já riscada", testar_cmdRiscar_celulaRiscada);
    CU_add_test(suite, "cmdRiscar - Comando inválido", testar_cmdRiscar_invalido);
    CU_add_test(suite, "cmdRiscar - Coordenadas inválidas", testar_cmdRiscar_coordsInvalida);
    CU_add_test(suite, "cmdUndo - Comando válido", testar_cmdUndo_valido);
    CU_add_test(suite, "cmdUndo - Último estado NULL", testar_cmdUndo_ultimoEstadoNULL);
    CU_add_test(suite, "cmdVerificarRestricoes - Tabuleiro Válido", testar_cmdVerificar_verifValida);
    CU_add_test(suite, "cmdVerificarRestricoes - Linha com duas brancas", testar_cmdVerificar_linhaInvalida);
    CU_add_test(suite, "cmdVerificarRestricoes - Coluna com duas brancas", testar_cmdVerificar_colunaInvalida);
    CU_add_test(suite, "cmdVerificarRestricoes - Casa em baixo riscada", testar_cmdVerificar_riscadaBaixo);
    CU_add_test(suite, "cmdVerificarRestricoes - Casa a direita riscada", testar_cmdVerificar_riscadaDireita);
    CU_add_test(suite, "cmdVerificarRestricoes - Sem caminho", testar_cmdVerificar_semCaminho);
    CU_add_test(suite, "cmdAjudar - Comando inválido", testar_cmdAjudar_invalido);
    CU_add_test(suite, "cmdAjudar - Comando válido 1", testar_cmdAjudar_valido);
    CU_add_test(suite, "cmdAjudar - Comando válido 2", testar_cmdAjudar_valido2);
    CU_add_test(suite, "cmdAjudar - Sem alterações", testar_cmdAjudar_semAlteracoes);
    CU_add_test(suite, "cmdAjudar - Comando com infracões", testar_cmdAjudar_comInfracoes);
    CU_add_test(suite, "cmdAjudar - Gerou infracões", testar_cmdAjudar_gerouInfracoes);
    CU_add_test(suite, "cmdA - Comando inválido", testar_cmdA_invalido);
    CU_add_test(suite, "cmdA - Gerou infracões", testar_cmdA_gerouInfracoes);
    CU_add_test(suite, "cmdA - Válido", testar_cmdA_valido);
    CU_add_test(suite, "cmdA - Sem alterações", testar_cmdA_semAlteracoes);
    CU_add_test(suite, "cmdResolver - Inválido", testar_cmdResolver_invalido);
    CU_add_test(suite, "cmdResolver - Impossível", testar_cmdResolver_impossivel);
    CU_add_test(suite, "cmdResolver - Válido", testar_cmdResolver_valido);
    CU_add_test(suite, "cmdResolver - Tabuleiro Resolvido", testar_cmdResolver_tabuleiroResolvido);
    CU_add_test(suite, "cmdResolver - Tabuleiro Impossível", testar_cmdResolver_tabuleiroImpossivel);
    CU_add_test(suite, "cmdHelp - válido", testar_cmdHelp_valido);
    CU_add_test(suite, "cmdHelp - inválido", testar_cmdHelp_invalido);
    CU_add_test(suite, "cmdStatus - válido", testar_cmdStatus_valido);
    CU_add_test(suite, "cmdStatus - com infracões", testar_cmdStatus_comInfracoes);
    CU_add_test(suite, "cmdStatus - inválido", testar_cmdStatus_invalido);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}