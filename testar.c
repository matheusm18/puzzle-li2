#define TESTING
#include <CUnit/Basic.h>
#include "comandos.h"

void testar_cmdLer_valido() {
    Estado e = {0};
    FILE *file = fopen("lerValido.txt", "w"); // modo write cria o arquivo se n existir, se existir escreve por cima
    fprintf(file, "2 2\nab\ncd\n");
    fclose(file);

    CU_ASSERT_TRUE(cmdLer('l', "lerValido.txt", &e));
    CU_ASSERT_TRUE(e.carregouTabuleiro);
    CU_ASSERT_EQUAL(e.linhas, 2);
    CU_ASSERT_EQUAL(e.colunas, 2);
    CU_ASSERT_EQUAL(e.tabuleiro[0][0], 'a');
    CU_ASSERT_EQUAL(e.tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(e.tabuleiro[1][0], 'c');
    CU_ASSERT_EQUAL(e.tabuleiro[1][1], 'd');

    remove("lerValido.txt"); // remover o arquivo apos o teste
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

// arg dif de null
void testar_cmdSair_invalido() {
    Estado e = {0};
    CU_ASSERT_FALSE(cmdSair('s', "sair", &e));
}

void testar_cmdPintar_valido() {
    Estado *e = malloc(sizeof(Estado)); // aloca o estado na heap
    memset(e, 0, sizeof(Estado)); // inicializa o estado com zeros (o tamanho do bloco que sera preenchido é o tamanho do estado)

    e->carregouTabuleiro = true;
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    CU_ASSERT_TRUE(cmdPintar('b', "a0", e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'A');
    liberarEstados(e); // libera a memória alocada
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
    e->linhas = 2; e->colunas = 2;
    e->tabuleiro[0][0] = 'a';

    CU_ASSERT_TRUE(cmdRiscar('r', "a0", e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], '#');
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
    e->linhas = 2; e->colunas = 2;

    CU_ASSERT_FALSE(cmdRiscar('r', "a5", e));
    CU_ASSERT_FALSE(cmdRiscar('r', "z0", e));
    liberarEstados(e);
}

void testar_cmdUndo_valido() {
    Estado *e = malloc(sizeof(Estado));
    memset(e, 0, sizeof(Estado));

    e->carregouTabuleiro = true;
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

    CU_ASSERT_TRUE(cmdUndo('d', NULL, e));
    CU_ASSERT_EQUAL(e->tabuleiro[0][0], 'a');

    liberarEstados(e);
}

void testar_cmdVerificar_verifValida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_FALSE(e.temInfracoes);
}


void testar_cmdVerificar_linhaInvalida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'A';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_colunaInvalida() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = 'A'; e.tabuleiro[0][1] = 'C';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_riscadaDireita() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = '#';
    e.tabuleiro[1][0] = 'b'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

void testar_cmdVerificar_riscadaBaixo() {
    Estado e = {0};
    e.carregouTabuleiro = true;
    e.linhas = 2; e.colunas = 2;
    e.tabuleiro[0][0] = '#'; e.tabuleiro[0][1] = 'a';
    e.tabuleiro[1][0] = '#'; e.tabuleiro[1][1] = 'C';

    CU_ASSERT_TRUE(cmdVerificarRestricoes('v', NULL, &e));
    CU_ASSERT_TRUE(e.temInfracoes);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Testes do Projeto", NULL, NULL); // primeiro NULL é a funcao de inicializacao(setup), segundo é a de limpeza

    CU_add_test(suite, "cmdLer - Arquivo valido", testar_cmdLer_valido);
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
    CU_add_test(suite, "cmdPintar - Comando inválido", testar_cmdPintar_invalido);
    CU_add_test(suite, "cmdPintar - Coordenadas inválidas", testar_cmdPintar_coordsInvalida);
    CU_add_test(suite, "cmdRiscar - Comando válido", testar_cmdRiscar_valido);
    CU_add_test(suite, "cmdRiscar - Comando inválido", testar_cmdRiscar_invalido);
    CU_add_test(suite, "cmdRiscar - Coordenadas inválidas", testar_cmdRiscar_coordsInvalida);
    CU_add_test(suite, "cmdUndo - Comando válido", testar_cmdUndo_valido);
    CU_add_test(suite, "cmdUndo - Último estado NULL", testar_cmdUndo_ultimoEstadoNULL);
    CU_add_test(suite, "cmdVerificarRestricoes - Tabuleiro Válido", testar_cmdVerificar_verifValida);
    CU_add_test(suite, "cmdVerificarRestricoes - Linha com duas brancas", testar_cmdVerificar_linhaInvalida);
    CU_add_test(suite, "cmdVerificarRestricoes - Coluna com duas brancas", testar_cmdVerificar_colunaInvalida);
    CU_add_test(suite, "cmdVerificarRestricoes - Casa em baixo riscada", testar_cmdVerificar_riscadaBaixo);
    CU_add_test(suite, "cmdVerificarRestricoes - Casa a direita riscada", testar_cmdVerificar_riscadaDireita);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}