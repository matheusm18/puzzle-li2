# Compilador e flags
CC = gcc
BASE_FLAGS = -Wall -Wextra -pedantic -O1 -g
SANITIZE_FLAGS = -fsanitize=address -fno-omit-frame-pointer
COVERAGE_FLAGS = --coverage
CFLAGS = $(BASE_FLAGS) $(SANITIZE_FLAGS) $(COVERAGE_FLAGS)
LDFLAGS = -lcunit

# Arquivos
SRC = comandos.c estado.c
OBJ = $(SRC:.c=.o)
TEST_OBJ = testar.o $(OBJ)
MAIN_OBJ = main.o $(OBJ)

all: jogo testar

# Programa principal
jogo: $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Testes com filtro para relatório de cobertura
testar: $(TEST_OBJ)
	$(CC) $(CFLAGS) -DTESTING -o $@ $^ $(LDFLAGS)
	./testar
	gcov -b -c comandos.c | grep -A 3 "File 'comandos.c'"

# Regras de compilação (compilar os arquivos.c para .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f jogo testar *.o *.gcov *.gcda *.gcno