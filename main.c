#include "comandos.h"

int main() {
    Estado estado = {0}; // inicializa todos os campos com zero e aloca na stack
    estado.guardarEstados = true;
    estado.printar = true;
    estado.looping = true;
    
    // array de comandos (ponteiros para funções)
    Comando comandos[] = {cmdLer,cmdGravar,cmdSair,cmdPintar,cmdRiscar,cmdUndo,cmdVerificarRestricoes,cmdAjudar,cmdA,cmdResolver,cmdHelp,cmdStatus,NULL};

    char input[100];
    
    printf("\n"
       COLOR_CYAN  "===============================================\n" COLOR_RESET
       COLOR_WHITE "             BEM-VINDO AO PUZZLE!              \n" COLOR_RESET
       COLOR_WHITE "       Digite 'h' para ver os comandos         \n" COLOR_RESET
       COLOR_WHITE "     Digite 'l <ficheiro>' para iniciar.       \n" COLOR_RESET
       COLOR_CYAN  "===============================================\n\n" COLOR_RESET);

    while (estado.looping) {
        estado.deuAviso = false; // reinicia a flag de aviso

        // le o que foi introduzido (stdin) e armazena em input (para quando chega no \n ou ultrapassa o sizeof) e retorna o ponteiro
        if (fgets(input, sizeof(input), stdin) == NULL) estado.looping = false;

        // Quando escrevemos b a1 e damos enter o ultimo caractere é o \n, precisamos remove-lo.
        int length = strlen(input);
        if (length > 0 && input[length-1] == '\n') input[length-1] = '\0';

        char cmd = '\0';
        char arg[100] = {0};
        // vai ler da string input (%99s para usar no máx 99 caracteres (precisa de espaço pro \0 no fim))
        int argsLido = sscanf(input, "%c %99s", &cmd, arg);

        bool temArgumento = (cmd == 'l' || cmd == 'g' || cmd == 'b' || cmd == 'r');

        // ver se leu o comando
        if (argsLido < 1) {
            printf("\nErro: Comando inválido!\n");
        }
        // ver se era necessário argumento e não havia
        else if (temArgumento && argsLido < 2) {
            printf("\nErro: Este comando requer um argumento!\n");
        }
        else {
            bool reconhecido = false; // é uma flag (reconhecido só vira true quando um comando é reconhecido (é bem passado))
            
            // enquanto não chegar ao fim da lista de comandos e não houver nenhum que foi reconhecido procura o comando
            for (int i = 0; comandos[i] != NULL && !reconhecido; i++) {
                if (argsLido > 1) {
                    reconhecido = comandos[i](cmd, arg, &estado);
                }
                else {
                    // se não tiver argumentos passa o ponteiro nulo
                    reconhecido = comandos[i](cmd, NULL, &estado);
                }
            }

            if (reconhecido) {
                if (cmd == 'b' || cmd == 'r' || cmd == 'a' || cmd == 'A' || cmd == 'l' || cmd == 'd' || cmd == 'R') {
                    mostrarTabuleiro(&estado);
                }
            }

            if (reconhecido == false && estado.deuAviso == false) { // se não houve nenhum reconhecido então é inválido
                printf("\nErro: Comando inválido!\n");
            }
        }
    }
    
    printf("\nJogo encerrado.\n");
    liberarEstados(estado.ultimoEstado); // libera a memoria alocada para os estados guardados
    return 0;
}