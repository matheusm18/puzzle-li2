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
        printf("\nErro: erro ao abrir %s\n", arg);
        e->deuAviso = true;
        return false; // erro
    }
    
    if (e->carregouTabuleiro == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)

    // armazena no estado -> linhas
    if (fscanf(ficheiro, "%d %d", &(e->linhas), &(e->colunas)) != 2) {
        fclose(ficheiro);
        e->deuAviso = true;
        printf("\nErro: Formato inválido no ficheiro\n");
        return false;
    }

    if (e->linhas <= 0 || e->colunas <= 0 || e->linhas > TMAX_MATRIZ || e->colunas > TMAX_MATRIZ) {
        printf("\nErro: As dimensões do tabuleiro são inválidas\n");
        e->deuAviso = true;
        fclose(ficheiro);
        return false;
    }

    while (fgetc(ficheiro) != '\n'); // retira os espaços e passa o "cursor" para depois do \n

    char linha[TMAX_MATRIZ + 2]; // adiciona espaço p/ \n e o \0
    for (int i = 0; i < e->linhas; i++) {
        if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
            e->deuAviso = true;
            printf("\nErro ao ler a linha %d\n", i+1);
            fclose(ficheiro);
            return false;
        }
        // remove o '\n' no final (strcspn retorna o indice do \n) p possibilitar futura comparacao
        linha[strcspn(linha, "\n")] = '\0';

        for (int j = 0; j < e->colunas; j++) {
            if (linha[j] == '\0') {
                e->deuAviso = true;
                printf("\nErro: linha %d menor que o esperado\n", i); // se introduziu uma dimensao e depois a linha era dif
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
        printf("\nTabuleiro gravado em %s\n", arg);
        e->deuAviso = true;
    }

    return true;
}

bool cmdSair(char cmd, char *arg, Estado *e) {
    if (cmd == 's' && arg == NULL) {
        e->looping = false;
        return true; // comando foi reconhecido
    }
    return false; // comando não foi reconhecido
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
        memcpy(e, estadoAnterior, sizeof(Estado)); // restaura o estado anterior (duplica e copia)

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
                        if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas brancas\n",'a'+j,i,'a'+k,i);
                        e->temInfracoes = true;
                    }
                }
                // verificar mesma coluna
                for (int k = i+1; k < e->linhas; k++) {
                    if (k != i && e->tabuleiro[k][j] == celula) {
                        if (e->printar == true) printf("\nInfração: '%c%d' e '%c%d' são ambas brancas\n",'a'+j,i,'a'+j,k);
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

int contaLetras (Estado *e) {
    int contador = 0;
    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] != '#') { // se a casa for uma letra
                contador++;
            }
        }
    }
    return contador;
}

void ajudaIsolada (Estado *e) {

    for (int i = 0; i < e->linhas; i++) {
        for (int j = 0; j < e->colunas; j++) {
            if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') { // se a casa for minuscula
                char charAntigo = e->tabuleiro[i][j]; // guarda a letra
                e->tabuleiro[i][j] = '#'; // converte para maiuscula

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
                        if (e->tabuleiro[i][j] != '#') {
                            linha = i; coluna = j;
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
        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)

        // pinta de branco uma casa quando seria impossivel que esta fosse riscada
        ajudaIsolada(e);


        // riscar todas as letras iguais a uma letra branca na mesma linha e/ou coluna
        for (int i = 0; i < e->linhas; i++) {
            for (int j = 0; j < e->colunas; j++) {
                if (e->tabuleiro[i][j] >= 'A' && e->tabuleiro[i][j] <= 'Z') { // Casa branca
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

        if (e->printar == true) printf("\nDicas aplicadas ao tabuleiro.\n");
        return true;
    }
    return false;
}

bool cmdA (char cmd, char *arg, Estado *e) {
    if (cmd == 'A' && arg == NULL && e->carregouTabuleiro == true) {

        // guarda estado se não tiver sido chamado pela função resolver
        if (e->guardarEstados == true) guardarEstado(e); // guarda o estado atual (antes de fazer a alteração)

        bool flagGuardarAntes = e->guardarEstados; // guarda o estado atual da flag

        e->guardarEstados = false; // ativa a flag para não guardar os estados enquanto executar o cmd 'a'

        Estado estadoAnterior;
        memcpy(&estadoAnterior, e, sizeof(Estado)); // copia o estado atual para comparação posterior

        bool flagContinuar = true; // flag para continuar a correr o loop
        while (flagContinuar == true) {
            cmdAjudar('a', NULL, e); // atualiza o tabuleiro e consequentemente o estado

            if (comparaTabuleiros(e, &estadoAnterior)) { // se forem iguais
                flagContinuar = false; // para o loop
            }

            memcpy(&estadoAnterior, e, sizeof(Estado)); // atualiza o estado anterior
        }

        if (e->printar == true) printf("\nO comando A foi executado com sucesso!\n");
        e->guardarEstados = flagGuardarAntes; // restaura o estado da flag
        return true;
    }
    return false;
}

int resolverTabuleiro(Estado *e) {

    cmdA('A', NULL, e); // atualiza o tabuleiro com todas as dicas possiveis de deduzir
    if (tabuleiroResolvido (e) == 1) return 1; // se esta resolvido retorna 1
    if (tabuleiroResolvido (e) == 0) return 0; // se não pode ser resolvido retorna 0
    
    // tem letras minusculas ainda

        for (int i = 0; i < e->linhas; i++) {
            for (int j = 0; j < e->colunas; j++) {
                if (e->tabuleiro[i][j] >= 'a' && e->tabuleiro[i][j] <= 'z') { // se a casa for minuscula

                    char charAtual = e->tabuleiro[i][j]; // guarda a letra
                    e->tabuleiro[i][j] = '#'; // risca a letra

                    Estado estadoBackup;
                    memcpy(&estadoBackup, e, sizeof(Estado)); // copia o estado de agora com o char riscado para salvar como backup

                    int resultado = resolverTabuleiro(e); // chama a função recursivamente para resolver o tabuleiro para este novo caso

                    if (resultado == 0) { // se não pode ser resolvido com a letra riscada volta pro backup
                        memcpy(e, &estadoBackup, sizeof(Estado)); // restaura o estado original para antes das chamadas recursivas

                        e->tabuleiro[i][j] = toupper(charAtual); // pinta de branco
                        e->temInfracoes = false; // reinicia a flag de infrações (visto que pode ter sido alterada na recursão)
                    }

                    if (resultado == 1) return 1; // se o tabuleiro foi resolvido na recursão

                }
            }
        }

    // após mudar todas as letras mesmo assim não for possível resolver, então é impossível
    if (tabuleiroResolvido(e) == 0) return 0; // tabuleiro não pode ser resolvido
    else return 1;
}

bool cmdResolver(char cmd, char *arg, Estado *e) {
    if (cmd == 'R' && arg == NULL && e->carregouTabuleiro == true) {
        // guarda o estado atual antes de começar a resolver
        guardarEstado(e); 

        e->guardarEstados = false; // ativa a flag para não guardar os estados enquanto executar os cmds
        e->printar = false; // desativa a flag de printar

        int resultado = resolverTabuleiro(e); // chama a função para resolver o tabuleiro

        e->guardarEstados = true; // reativa a flag de guardar estados
        e->printar = true; // reativa a flag de printar

        if (resultado == 0) { // se não pode ser resolvido
            printf("\nAviso: O tabuleiro não pode ser resolvido.\n");
            return true;
        }
        else {
            printf("\nO tabuleiro está resolvido!\n");
            mostrarTabuleiro(e);
        }

        return true;
    }
    return false;
}

bool cmdHelp(char cmd, char *arg, Estado *e) {
    (void)e; // p/ n dar warning de variavel não utilizada
    if (cmd == 'h' && arg == NULL) {
        printf("\nComandos disponíveis:\n");
        printf("l [ficheiro] - Carregar tabuleiro\n");
        printf("g [ficheiro] - Gravar tabuleiro\n");
        printf("b [coluna][linha] - Pintar célula\n");
        printf("r [coluna][linha] - Riscar célula\n");
        printf("d - Desfazer a última ação\n");
        printf("v - Verificar se há infrações no tabuleiro\n");
        printf("a - Aplicar dicas ao tabuleiro\n");
        printf("A - Aplicar dicas ao tabuleiro até não haver mais alterações\n");
        printf("R - Resolver o tabuleiro\n");
        printf("h - Mostrar ajuda\n");
        printf("s - Sair do programa\n");
        return true;
    }
    return false;
}