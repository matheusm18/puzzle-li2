#include "comandos.h"

bool cmdLer(char cmd, char *arg, Estado *e) {
    if (cmd != 'l') return false;
    
    if (arg == NULL) {
        printf("\nErro: o comando ler precisa de um argumento!\n");
        e->deuAviso = true;
        return false; // erro
    }

    // ficheiro é ponteiro para o tipo file (fopen retorna endereco)
    FILE *ficheiro = fopen(arg, "r");
    if (ficheiro == NULL) {
        printf("\nErro: erro ao abrir '%s'.\n", arg);
        e->deuAviso = true;
        return false; // erro
    }
    
    if (e->carregouTabuleiro == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)

    // armazena no estado -> linhas
    if (fscanf(ficheiro, "%d %d", &(e->linhas), &(e->colunas)) != 2) {
        fclose(ficheiro);
        e->deuAviso = true;
        printf("\nErro: Formato inválido no ficheiro.\n");
        return false;
    }

    if (e->linhas <= 0 || e->colunas <= 0 || e->linhas > TMAX_MATRIZ || e->colunas > TMAX_MATRIZ) {
        printf("\nErro: As dimensões do tabuleiro são inválidas.\n");
        e->deuAviso = true;
        fclose(ficheiro);
        return false;
    }

    while (fgetc(ficheiro) != '\n'); // retira os espaços e passa o "cursor" para depois do \n

    char linha[TMAX_MATRIZ + 2]; // adiciona espaço para o \n e o \0
    for (int i = 0; i < e->linhas; i++) {
        if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
            e->deuAviso = true;
            printf("\nErro ao ler a linha '%d'.\n", i+1);
            fclose(ficheiro);
            return false;
        }
        // remove o '\n' no final (strcspn retorna o indice do \n) p possibilitar futura comparacao
        linha[strcspn(linha, "\n")] = '\0';

        for (int j = 0; j < e->colunas; j++) {
            if (linha[j] == '\0') {
                e->deuAviso = true;
                printf("\nErro: linha %d menor que o esperado.\n", i); // se introduziu uma dimensao e depois a linha era dif
                fclose(ficheiro);
                return false;
            }
            e->tabuleiro[i][j] = linha[j];
        }
    }

    fclose(ficheiro);
    if (e->printar == true) {
        printf("\nTabuleiro inicial %dx%d carregado:\n", e->linhas, e->colunas);
        e->deuAviso = true;
    }

    e->carregouTabuleiro = true;
    return true;
}

bool cmdGravar(char cmd, char *arg, Estado *e) {
    if (cmd != 'g') return false;

    if (arg == NULL) {
        printf("\nErro: o comando gravar precisa de um argumento!\n");
        e->deuAviso = true;
        return false; // erro
    }

    // ficheiro é ponteiro para o tipo file (fopen retorna endereco)
    FILE *ficheiro = fopen(arg, "w");
    fprintf(ficheiro, "%d %d\n", e->linhas, e->colunas);
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            fprintf(ficheiro, "%c", e->tabuleiro[i][j]);
        }
        fprintf(ficheiro, "\n"); // a cada linha que escrever escreve o \n
    }

    fclose(ficheiro);

    if (e->printar == true) {
        printf("\nTabuleiro gravado em '%s'.\n", arg);
        e->deuAviso = true;
    }

    return true;
}

bool cmdSair(char cmd, char *arg, Estado *e) {
    if (cmd == 's' && arg == NULL) {
        e->looping = false;
        return true;
    }
    return false;
}


bool cmdPintar(char cmd, char *arg, Estado *e) {
    if (cmd != 'b' || e->carregouTabuleiro == false) return false;
    
    int x;
    char y;
    if (sscanf(arg, "%c%d", &y, &x) != 2) return false;

    int coluna = y - 'a'; // 'a' corresponde a 0, 'b' a 1 etc
    if (x >= 0 && x < e->linhas && coluna >= 0 && coluna < e->colunas) {

        if (e->tabuleiro[x][coluna] == '#') {
            printf("\nErro: não é possível pintar uma célula já riscada.\n");
            e->deuAviso = true;
            return false;
        }

        if (e->tabuleiro[x][coluna] >= 'A' && e->tabuleiro[x][coluna] <= 'Z') {
            printf("\nErro: não é possível pintar uma célula já pintada.\n");
            e->deuAviso = true;
            return false;
        }

        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)
        e->tabuleiro[x][coluna] = toupper(e->tabuleiro[x][coluna]);
        return true;
    }
    return false;
}

bool cmdRiscar(char cmd, char *arg, Estado *e) {
    if (cmd != 'r' || e->carregouTabuleiro == false) return false;
    
    int x;
    char y;
    if (sscanf(arg, "%c%d", &y, &x) != 2) return false;
    
    int coluna = y - 'a';
    if (x >= 0 && x < e->linhas && coluna >= 0 && coluna < e->colunas) {

        if (e->tabuleiro[x][coluna] == '#') {
            printf("\nErro: não é possível riscar uma célula já riscada.\n");
            e->deuAviso = true;
            return false;
        }

        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)
        e->tabuleiro[x][coluna] = '#';
        return true;
    }
    return false;
}

bool cmdUndo(char cmd, char *arg, Estado *e) {
    if (cmd == 'd' && arg == NULL) {
        if (e->ultimoEstado == NULL) {
            printf("\nErro: não há comandos anteriores para desfazer.\n");
            e->deuAviso = true;
            return false;
        }
        // fazer pop da stack (remove o que esta mais em cima (estado e))
        Estado *estadoAnterior = e->ultimoEstado; // pega o estado anterior
        memcpy(e, estadoAnterior, sizeof(Estado)); // restaura o estado anterior

        free(estadoAnterior); // libera a memória do estado anterior
        return true;
    }
    return false;
}

int casasPossiveisVisitar(Estado *e, int l, int c, int visitadas[e->linhas][e->colunas]) {
    if (l < 0 || l >= e->linhas || c < 0 || c >= e->colunas || visitadas[l][c] == 1 || e->tabuleiro[l][c] == '#') return 0;

    visitadas[l][c] = 1; // marca a casa como visitada

    return (1 + casasPossiveisVisitar(e, l - 1, c, visitadas) + casasPossiveisVisitar(e, l + 1, c, visitadas) + casasPossiveisVisitar(e, l, c - 1, visitadas) + casasPossiveisVisitar(e, l, c + 1, visitadas));
}

bool cmdVerificarRestricoes(char cmd, char *arg, Estado *e) {
    if (cmd != 'v' || arg != NULL || e->carregouTabuleiro == false) return false;

    e->temInfracoes = false; // reinicia a flag de infrações

    // verificar letras brancas repetidas na mesma linha/coluna
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            char celula = e->tabuleiro[i][j];
            
            // verificar apenas células brancas (letras maiúsculas)
            if (isupper(celula)) {
                // verificar mesma linha
                for (int k = j+1; k < e->colunas; k++) {
                    if (k != j && e->tabuleiro[i][k] == celula) {
                        if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas brancas.\n",'a'+j,i,'a'+k,i);
                        e->temInfracoes = true;
                    }
                }
                // verificar mesma coluna
                for (int k = i+1; k < e->linhas; k++) {
                    if (k != i && e->tabuleiro[k][j] == celula) {
                        if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas brancas.\n",'a'+j,i,'a'+j,k);
                        e->temInfracoes = true;
                    }
                }
            }
        }
    }
    
    // verificar celulas riscadas vizinhas ortogonal
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] == '#') {
                // verificar em baixo e a direita
                if (i < (e->linhas - 1) && e->tabuleiro[i+1][j] == '#') { // verifica em baixo
                    if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas riscadas.\n",'a'+j, i, 'a'+j, i+1);
                    e->temInfracoes = true;
                }
                if (j < (e->colunas - 1) && e->tabuleiro[i][j+1] == '#') { // verifica a direita
                    if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas riscadas.\n",'a'+j, i, 'a'+j+1, i);
                    e->temInfracoes = true;
                }
            }
        }
    }
    
    // verificar se todas as casas estão ligadas por um caminho ortogonal
    int contadorLetras = 0;
    int visitadas[e->linhas][e->colunas];

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            visitadas[i][j] = 0; // inicializa todas as casas como não visitadas
        }
    }

    int linha = 0, coluna = 0;
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] != '#') {
                linha = i; coluna = j;
                contadorLetras++;
            }
        }
}

    if (contadorLetras != casasPossiveisVisitar(e,linha,coluna,visitadas)) {
        if (e->printar == true) {
            printf("\nInfração: não há um caminho entre todas as casas do tabuleiro.\n");
        }
        e->temInfracoes = true;
    }

    if (e->temInfracoes == false && e->printar == true) printf("\nNenhuma infração encontrada. Tabuleiro válido!\n");
    return true;
}

// pintar de branco uma casa quando seria impossível que esta fosse riscada por isolar casas brancas
void ajudaIsolada (Estado *e) {

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') { // se a casa for minuscula

                char charAntigo = e->tabuleiro[i][j]; // guarda a letra
                e->tabuleiro[i][j] = '#'; // risca a casa

                int visitadas[e->linhas][e->colunas];
                for (int i = 0; i < e->linhas; i++) {
                    for (int j = 0; j < e->colunas; j++) {
                        visitadas[i][j] = 0; // inicializa todas as casas como não visitadas
                    }
                }

                int contadorLetras = 0;
                int linha = 0, coluna = 0;
                for (int i = 0; i < e->linhas; i++) {
                    for (int j = 0; j < e->colunas; j++) {
                        if (e->tabuleiro[i][j] != '#') { // se for uma letra
                            linha = i; coluna = j; // guarda a posição de uma letra para comparar depois
                            contadorLetras++;
                        }
                    }
                }
                if (contadorLetras != casasPossiveisVisitar(e,linha,coluna,visitadas)) {
                    e->tabuleiro[i][j] = toupper(charAntigo); // letra tem que ser pintada de branco
                }
                else {
                    e->tabuleiro[i][j] = charAntigo; // volta a letra original
                }
            }
        }
    }
}

bool cmdAjudar(char cmd, char *arg, Estado *e) {
    if (cmd == 'a' && arg == NULL && e->carregouTabuleiro == true) {

        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer as alterações)

        bool ePrintarAntes = e->printar; // guarda o estado atual da flag

        // se tiver infrações não ajuda porque pode piorar
        e->printar = false; // desativa a flag para não printar enquanto executar o cmd 'v'
        cmdVerificarRestricoes('v', NULL, e);
        e->printar = ePrintarAntes;

        if (e->temInfracoes == true){
            if (e->printar == true) printf("\nO tabuleiro apresenta infrações, logo não é possível ajudar.\n");
            // desfaz o guardarEstado do inicio já que nada mudou (se este foi executado)
            if (e->guardarEstados == true) {
                cmdUndo('d', NULL, e);
            }
            return true;
        }

        // riscar todas as letras iguais a uma letra branca na mesma linha e/ou coluna
        for (int i = 0; i < e->linhas; i++) {
            for (int j = 0; j < e->colunas; j++) {
                if (e->tabuleiro[i][j] >= 'A' && e->tabuleiro[i][j] <= 'Z') { // casa branca
                    char letra = e->tabuleiro[i][j];

                    // percorrer a linha
                    for (int k = 0; k < e->colunas; k++) {
                        if (k != j && (e->tabuleiro[i][k] == letra || e->tabuleiro[i][k] == tolower(letra))) {
                            e->tabuleiro[i][k] = '#'; // riscar
                        }
                    }

                    // percorrer a coluna
                    for (int k = 0; k < e->linhas; k++) {
                        if (k != i && (e->tabuleiro[k][j] == letra || e->tabuleiro[k][j] == tolower(letra))) {
                            e->tabuleiro[k][j] = '#'; // riscar
                        }
                    }
                }
            }
        }

        // pintar de branco todas as letras que sao vizinhas de um #
        for (int i = 0; i < e->linhas; i++) {
            for (int j = 0; j < e->colunas; j++) {
                if (e->tabuleiro[i][j] == '#') { // casa riscada
                    if (i > 0 && e->tabuleiro[i-1][j] >= 'a' && e->tabuleiro[i-1][j] <= 'z') { // cima
                        e->tabuleiro[i-1][j] = toupper(e->tabuleiro[i-1][j]);
                    }
                    if (i < e->linhas - 1 && e->tabuleiro[i+1][j] >= 'a' && e->tabuleiro[i+1][j] <= 'z') { // baixo
                        e->tabuleiro[i+1][j] = toupper(e->tabuleiro[i+1][j]);
                    }
                    if (j > 0 && e->tabuleiro[i][j-1] >= 'a' && e->tabuleiro[i][j-1] <= 'z') { // esquerda
                        e->tabuleiro[i][j-1] = toupper(e->tabuleiro[i][j-1]);
                    }
                    if (j < e->colunas - 1 && e->tabuleiro[i][j+1] >= 'a' && e->tabuleiro[i][j+1] <= 'z') { // direita
                        e->tabuleiro[i][j+1] = toupper(e->tabuleiro[i][j+1]);
                    }
                }
            }
        }


        // pinta de branco uma casa quando seria impossivel que esta fosse riscada
        ajudaIsolada(e);
        

        if (e->guardarEstados == true) { // se o estado foi guardado no inicio verifica se houve alterações (se for chamado pelo 'A' não é necessário)

            if (comparaTabuleiros(e, e->ultimoEstado) == false) { // se o tabuleiro foi alterado
                e->printar = false;
                cmdVerificarRestricoes('v', NULL, e); // verifica se o tabuleiro continua válido
                e->printar = ePrintarAntes; // restaura o estado da flag
                
                if (e->temInfracoes == true) { // se o tabuleiro ficou inválido
                    printf("\nNão é possível ajudar nestas condições!\n");
                    cmdUndo('d', NULL, e); // volta para o tabuleiro inicial
                }

                else {
                    if (e->printar == true) printf("\nDicas aplicadas ao tabuleiro.\n");
                }
            }

            else {
                if (e->printar == true) printf("\nNenhuma dica encontrada.\n");
                cmdUndo('d', NULL, e); // desfaz o guardarEstado do inicio já que nada mudou
            }

        }

        return true;
    }
    return false;
}

bool cmdA (char cmd, char *arg, Estado *e) {
    if (cmd == 'A' && arg == NULL && e->carregouTabuleiro == true) {

        // guarda estado se não tiver sido chamado pela função resolver
        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)

        bool flagGuardarAntes = e->guardarEstados; // guarda o estado atual da flag
        bool flagPrintarAntes = e->printar;

        e->guardarEstados = false; // ativa a flag para não guardar os estados enquanto executar o cmd 'a'
        e->printar = false;

        Estado estadoAnterior;
        memcpy(&estadoAnterior, e, sizeof(Estado)); // copia o estado atual para comparação posterior

        bool flagContinuar = true; // flag para continuar a correr o loop
        bool houveMudancasInicio = false; // flag para verificar se houve mudanças no tabuleiro desde que o comando A foi chamado

        while (flagContinuar == true) {
            cmdAjudar('a', NULL, e); // faz a ajuda

            if (comparaTabuleiros(e, &estadoAnterior)) { // se forem iguais
                flagContinuar = false; // para o loop (n houve mudanças)
            }

            else {

                cmdVerificarRestricoes('v', NULL, e); // verifica se o tabuleiro continua válido

                if (e->temInfracoes == true && flagGuardarAntes == true) { // se o tabuleiro ficou inválido e o comando 'A' não foi chamado pelo 'R'
                    e->printar = flagPrintarAntes; // restaura o estado da flag
                    e->guardarEstados = flagGuardarAntes; // restaura o estado da flag

                    if (e->printar == true) printf("\nNão é possível ajudar nestas condições!\n");
                    cmdUndo('d', NULL, e); // volta para o tabuleiro inicial
                    return true;
                }

                // se o tabuleiro foi alterado e não tem infrações
                houveMudancasInicio = true;
                memcpy(&estadoAnterior, e, sizeof(Estado)); // atualiza o estado anterior (o que criamos e não o da struct)

            }
        }

        if (houveMudancasInicio == false && flagGuardarAntes == true) {
            // se não houve mudancas e a flag de guardar estados estava ativa, desfaz o guardarEstado do inicio ja que nada mudou
            cmdUndo('d', NULL, e);
        }

        e->printar = flagPrintarAntes;
        if (e->printar == true && houveMudancasInicio == false) printf("\nNenhuma dica encontrada.\n");
        if (e->printar == true && houveMudancasInicio == true) printf("\nO comando A foi executado com sucesso!\n");
        e->guardarEstados = flagGuardarAntes; // restaura o estado da flag
        return true;
    }
    return false;
}

int resolverTabuleiro(Estado *e) {

    cmdA('A', NULL, e); // atualiza o tabuleiro com todas as dicas possiveis de deduzir
    if (tabuleiroResolvido (e) == 1) return 1; // se esta resolvido retorna 1
    if (tabuleiroResolvido (e) == 0) return 0; // se não pode ser resolvido retorna 0
    
    // tem letras minusculas ainda (tabuleiroResolvido retornou 2)

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') { // se a casa for minuscula

                // backup do estado
                Estado estadoBackup;
                memcpy(&estadoBackup, e, sizeof(Estado)); // copia o estado de agora com o char pintado para salvar como backup

                // tenta pintar
                e->tabuleiro[i][j] = toupper(e->tabuleiro[i][j]);
                int resultado = resolverTabuleiro(e);
                if (resultado == 1) return 1;

                // volta ao estado anterior
                memcpy(e, &estadoBackup, sizeof(Estado));

                // tenta riscar
                e->tabuleiro[i][j] = '#';
                e->temInfracoes = false; // reseta flag
                resultado = resolverTabuleiro(e);
                if (resultado == 1) return 1;

                // volta ao estado original de novo
                memcpy(e, &estadoBackup, sizeof(Estado));

                return 0;
            }
        }
    }

    return 0;
}

bool cmdResolver(char cmd, char *arg, Estado *e) {
    if (cmd == 'R' && arg == NULL && e->carregouTabuleiro == true) {

        if (tabuleiroResolvido(e) == 1) { // se o tabuleiro já estiver resolvido
            printf("\nO tabuleiro já está resolvido!\n");
            return true;
        }

        // guarda o estado atual antes de começar a resolver
        guardarEstado(e);

        e->guardarEstados = false; // ativa a flag para não guardar os estados enquanto executar os cmds
        e->printar = false; // desativa a flag de printar

        int resultado = resolverTabuleiro(e); // chama a função para resolver o tabuleiro

        e->guardarEstados = true; // reativa a flag de guardar estados
        e->printar = true; // reativa a flag de printar

        if (resultado == 0) { // se não pode ser resolvido
            printf("\nAviso: O tabuleiro não pode ser resolvido.\n");
            cmdUndo('d', NULL, e); // volta ao estado anterior
            return true;
        }
        else {
            printf("\nO tabuleiro está resolvido!\n");
        }

        return true;
    }
    return false;
}

bool cmdHelp(char cmd, char *arg, Estado *e) {
    (void)e; // p/ n dar warning de variavel não utilizada
    if (cmd == 'h' && arg == NULL) {
    
        printf("\n"
            COLOR_CYAN "==================== COMANDOS DISPONÍVEIS ====================\n\n" COLOR_RESET
            COLOR_YELLOW " l [ficheiro]        " COLOR_RESET "- Carregar tabuleiro\n"
            COLOR_YELLOW " g [ficheiro]        " COLOR_RESET "- Gravar tabuleiro\n"
            COLOR_YELLOW " b [coluna][linha]   " COLOR_RESET "- Pintar célula\n"
            COLOR_YELLOW " r [coluna][linha]   " COLOR_RESET "- Riscar célula\n"
            COLOR_YELLOW " d                   " COLOR_RESET "- Desfazer a última ação\n"
            COLOR_YELLOW " v                   " COLOR_RESET "- Verificar infrações no tabuleiro\n"
            COLOR_YELLOW " a                   " COLOR_RESET "- Aplicar dicas ao tabuleiro\n"
            COLOR_YELLOW " A                   " COLOR_RESET "- Aplicar dicas até não haver alterações\n"
            COLOR_YELLOW " R                   " COLOR_RESET "- Resolver o tabuleiro\n"
            COLOR_YELLOW " h                   " COLOR_RESET "- Mostrar todos os comandos\n"
            COLOR_YELLOW " S                   " COLOR_RESET "- Mostrar status do jogo\n"
            COLOR_YELLOW " s                   " COLOR_RESET "- Sair do programa\n\n"
            COLOR_CYAN "==============================================================\n" COLOR_RESET);

        return true;
    }
    return false;
}

bool cmdStatus(char cmd, char *arg, Estado *e) {
    if (cmd != 'S' || arg != NULL || e->carregouTabuleiro == false) return false;

    int brancas = 0, riscadas = 0, minusculas = 0;
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            char c = e->tabuleiro[i][j];
            if (c == '#') riscadas++;
            else if (isupper(c)) brancas++;
            else if (islower(c)) minusculas++;
        }
    }
    e->printar = false;
    cmdVerificarRestricoes('v', NULL, e); // verifica se o tabuleiro está válido
    e->printar = true;

    int total = e->linhas * e->colunas;
    printf("\nEstatísticas do tabuleiro:\n");
    printf("  - Casas pintadas (brancas): %d\n", brancas);
    printf("  - Casas riscadas (\"#\"):     %d\n", riscadas);
    printf("  - Casas restantes:          %d\n", minusculas);
    if (e->temInfracoes == false) printf("  - Progresso:               %.1f%%\n", 100.0 * (brancas + riscadas) / total);
    else printf("  - Progresso: 0.0%% - há infraçoes\n");
    return true;
}