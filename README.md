# Laboratórios de Informática II - Hitori-Like Puzzle Solver

Este puzzle é um jogo do gênero [Hitori](https://en.wikipedia.org/wiki/Hitori), desenvolvido em [C](https://www.c-language.org/about) no âmbito da cadeira Laboratórios de Informática II, do curso de Engenharia Informática na Universidade do Minho (ano letivo 2024/25).

## 🏆 Nota final do projeto: 20/20

## Enunciado do projeto

O enunciado do projeto pode ser consultado [aqui](https://eos.di.uminho.pt/~rui/projeto/enunciado.html).

## 🎯 Objetivo

O objetivo do programa é permitir que o utilizador **resolva interativamente um puzzle lógico**, aplicando um conjunto de regras que determinam como cada casa do tabuleiro deve ser marcada.

Cada casa contém um **símbolo** (uma letra minúscula), e o jogador deve decidir quais as casas que devem ser:

- **Pintadas a branco** → a letra passa para **maiúscula**
- **Riscadas** → substituídas por um **cardinal (`#`)**

## 🧠 Regras do Puzzle

1. Em cada **linha e coluna**, só pode existir **uma única réplica pintada a branco** de cada símbolo.
2. Todas as outras réplicas desse símbolo devem ser **riscadas** (`#`).
3. Se uma casa for riscada, **todas as suas vizinhas ortogonais** (acima, abaixo, esquerda, direita) devem ser **pintadas a branco**.
4. Deve existir **um caminho ortogonal** entre todas as casas brancas do tabuleiro — nenhuma pode ficar isolada.

## ⚙️ Funcionalidades

Principais funcionalidades implementadas:

- **Leitura e gravação de tabuleiros** em ficheiros de texto
- **Manipulação de casas** — pintar (branco) e riscar (cardinal)
- **Desfazer ações anteriores** (histórico de jogadas)
- **Verificação automática** de todas as restrições do puzzle
- **Sugestão de jogadas** com base em deduções lógicas
- **Aplicação iterativa de dicas** até não haver mais alterações possíveis
- **Resolução automática completa** do tabuleiro
- **Validação da conectividade** das células brancas
- **Testes unitários** abrangendo todas as funcionalidades com cobertura de código

## 🕹️ Comandos Disponíveis

O jogo suporta os seguintes comandos:

| Comando | Descrição |
|----------|------------|
| `l [ficheiro]`        | Carregar um tabuleiro a partir de um ficheiro |
| `g [ficheiro]`        | Gravar o tabuleiro atual num ficheiro |
| `b [coluna][linha]`   | Pintar a célula indicada (converter em maiúscula) |
| `r [coluna][linha]`   | Riscar a célula indicada (substituir por `#`) |
| `d`                   | Desfazer a última ação realizada |
| `v`                   | Verificar as restrições e indicar infrações |
| `a`                   | Aplicar deduções automáticas simples |
| `A`                   | Aplicar deduções até não haver mais alterações |
| `R`                   | Resolver o tabuleiro automaticamente |
| `h`                   | Mostrar novamente a lista de comandos |
| `S`                   | Mostrar o estado atual do jogo |
| `s`                   | Sair do programa |

## 🧰 Instalação e Execução

Para compilar o projeto, é necessário ter o **gcc** instalado no sistema.
O projeto utiliza uma **Makefile** com os seguintes *targets* principais:

### 🔹 Compilar e correr o jogo
```sh
make jogo
./jogo
```

### 🔹 Executar testes

Para correr todos os testes automatizados:

```sh
make testar
```

## Desenvolvido por

- [Matheus Monteiro Azevedo](https://github.com/matheusm18) - a111430
- [Francisco Luciano Martins](https://github.com/XicoMartins54) - a111775
