/*
#include "Assembler.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
  char nome[50];
  uint16_t posicao;
  uint16_t valor;
  struct No *proximo;
} No;

No *novoNo(char *nome, uint16_t posicao, uint16_t valor) {
  No *novo = (No *)malloc(sizeof(No));
  strcpy(novo->nome, nome);
  novo->posicao = posicao;
  novo->valor = valor;
  novo->proximo = NULL;
  return novo;
}

void adicionarNo(No **cabeca, char *nome, uint16_t posicao, uint16_t valor) {
  No *novo = novoNo(nome, posicao, valor);
  if (*cabeca == NULL) {
    *cabeca = novo;
    printf("\t[A]: Variável %s adicionada com valor %d\n\n", nome, valor);
    return;
  }
  No *atual = *cabeca;
  while (atual->proximo != NULL) {
    atual = atual->proximo;
  }
  atual->proximo = novo;

  printf("\t[A]: Variável %s adicionada com valor %d\n\n", nome, valor);
}

// Recebe o nome de uma variável e retorna o campo 'posição' dela.
// Usada para substituir na hora da montagem
int buscarPosicaoPorNome(No *cabeca, char *nome) {
  No *atual = cabeca;
  while (atual != NULL) {
    if (strcmp(atual->nome, nome) == 0) {
      printf("\t[A]: Variável encontrada: %s = %d\n", atual->nome,
             atual->posicao);
      return atual->posicao;
    }
    atual = atual->proximo;
  }

  return -1;
}

// Início do bloco de definição de variáveis começa com DATA
bool definicao_de_variavel(char *linha) {
  char *token = strtok(linha, " ");

  while (!strcmp(token, " ")) {
    token = strtok(NULL, " ");
  }

  return !strcmp(token, "DATA");
}

// Verifica se o mneumonico do assembly é válido
bool mneumonico_valido(char *mneumonico) {
  char *mneumonicos_validos[] = {"NOP", "STA", "LDA", "ADD", "OR", "AND",
                                 "NOT", "JMP", "JN",  "JZ",  "HLT"};

  for (int i = 0; i < 11; i++) {
    if (strcmp(mneumonico, mneumonicos_validos[i]) == 0) {
      return true;
    }
  }

  return false;
}

// Verifica se a linha está vazia
bool linha_vazia(char *linha) {
  char *token = strtok(linha, " ");

  while (!strcmp(token, " ")) {
    token = strtok(NULL, " ");
  }

  return (token[0] == '\n' || token[0] == '\0');
}

// Verifica se a linha demarca um final de bloco
bool encontrou_end(char *linha) {
  char *token = strtok(linha, " ");

  while (!strcmp(token, " ")) {
    token = strtok(NULL, " ");
  }

  return strcmp(token, "END");
}

// Recebe um mneumonico, um numero de posição de memória
// e duas posições da memória virtal.
// Adiciona o código do mneumonico na primeira posição da memória
// e o numero da posição na segunda posição da memória
void escrever_instrucao_dupla(char *mneumonico, uint16_t valor,
                              uint16_t *pos_memoria_codigo,
                              uint16_t *pos_memoria_valor) {
  char *mneumonicos[] = {"STA", "LDA", "ADD", "OR", "AND", "JMP", "JN", "JZ"};
  uint16_t codigos[] = {16, 32, 48, 64, 80, 128, 144, 160};

  for (int i = 0; i < 8; i++) {
    if (strcmp(mneumonico, mneumonicos[i]) == 0) {
      *pos_memoria_codigo = codigos[i];
      *pos_memoria_valor = valor;
      printf("\t[A]: Instrução %s %hu escrita na memória\n\n", mneumonico,
             valor);
      return;
    }
  }
}

// Recebe um mneumonico, um numero de posição de memória
// Adiciona o código do mneumonico na posição da memória
void escrever_instrucao_simples(char *mneumonico,
                                uint16_t *pos_memoria_codigo) {
  char *mneumonicos[] = {"NOP", "NOT", "HLT"};
  uint16_t codigos[] = {0, 96, 240};

  for (int i = 0; i < 4; i++) {
    if (strcmp(mneumonico, mneumonicos[i]) == 0) {
      *pos_memoria_codigo = codigos[i];
      printf("\t[A]: Instrução %s escrita na memória\n\n", mneumonico);
      return;
    }
  }
}

// Quebra uma linha em um vetor de palavras
int buscar_palavras(char *line, char *palavras[10]) {
  char *token;
  int wordCount = 0;

  // substitui o '\n' por '\0'
  int newline_pos = strcspn(line, "\n");
  if (line[newline_pos] == '\n') {
    line[newline_pos] = '\0';
  }

  token = strtok(line, " ");
  while (token != NULL && wordCount < 10) {
    palavras[wordCount] = malloc(strlen(token) + 1);
    if (palavras[wordCount] == NULL) {
      fprintf(stderr, "Falha ao alocar memória\n");
      return -1;
    }
    strcpy(palavras[wordCount], token);
    wordCount++;
    token = strtok(NULL, " ");
  }

  return wordCount;
}

int ExecutarAssembler(void) {

  FILE *file = fopen("Arquivos/assembly.txt", "r");
  if (file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return 1;
  }

  char line[100];       // Linha de texto do assembly.txt
  uint16_t m[256] = {}; // Memória virtual
  int i = 0;            // Posição da memória virtual

  // Cabeça da lista de variáveis
  No *cabeca = NULL;

  while (fgets(line, sizeof(line), file) != NULL) {

    printf("[L]: %s", line);

    char *palavras[10] = {0};
    int cont_palavras = buscar_palavras(line, palavras);
    if (cont_palavras == -1) {
      fclose(file);
      return -1;
    }

    if (cont_palavras == 0 || linha_vazia(palavras[0])) {
      printf("\t[A]: Linha vazia\n");
      continue;
    }

    if (palavras[0][0] == '/') {
      printf("\t[A]: Comentário\n");
      continue;
    }

    if (strcmp(palavras[0], "DATA") == 0) {
      printf("\t[A]: Início de bloco de variáveis\n");
      while (fgets(line, sizeof(line), file) != NULL) {
        printf("[L]: %s", line);
        cont_palavras = buscar_palavras(line, palavras);
        if (cont_palavras == -1) {
          fclose(file);
          return -1;
        }

        if (strcmp(palavras[0], "END") == 0)
          break;

        if (cont_palavras >= 3) {
          char *nome = palavras[0];
          uint16_t posicao = (uint16_t)atoi(palavras[1]);
          uint16_t valor = (uint16_t)atoi(palavras[2]);

          if (valor > 255) {
            printf("\t[A]: Valor da variável %s maior que 255\n", nome);
            return -1;
          }

          printf("\t[A] Variável\n\t\tnome: %s\n\t\tposição: %hu\n\t\tvalor: "
                 "%hu\n",
                 nome, posicao, valor);

          adicionarNo(&cabeca, nome, posicao, valor);
        }
      }
      printf("\t[A]: Fim do bloco de variáveis\n\n");
      continue;
    }

    if (strcmp(palavras[0], "CODE") == 0) {
      printf("\t[A]: Início de bloco de código\n\n");
      while (fgets(line, sizeof(line), file) != NULL) {
        printf("[L]: %s", line);
        cont_palavras = buscar_palavras(line, palavras);
        if (cont_palavras == -1) {
          fclose(file);
          return -1;
        }

        if (strcmp(palavras[0], "END") == 0)
          break;

        if (!mneumonico_valido(palavras[0])) {
          return -1;
        }

        printf("\t[A]: Mneumonico válido: %s\n", palavras[0]);

        if (cont_palavras == 1) {
          printf("\t[A]: Instrução simples\n");
          escrever_instrucao_simples(palavras[0], &m[i]);
          i++;
        }

        if (cont_palavras == 2) {
          printf("\t[A]: Instrução dupla\n");
          // Segunda palavra é um número
          if (isdigit(palavras[1][0])) {
            printf("\t[A]: Parâmetro é um número\n");
            uint16_t valor = (uint16_t)atoi(palavras[1]);
            escrever_instrucao_dupla(palavras[0], valor, &m[i], &m[i + 1]);
          } else // Segunda palavra é uma variável
          {
            printf("\t[A]: Parâmetro é uma variável\n");
            int valorInt = buscarPosicaoPorNome(cabeca, palavras[1]);
            escrever_instrucao_dupla(palavras[0], (uint16_t)valorInt, &m[i],
                                     &m[i + 1]);
          }
          i += 2;
        }
      }
      printf("\t[A]: Fim do bloco de código\n\n");
      continue;
    }
  }

  printf("Memória final:\n");
  for (int i = 0; i < 255; i++) {
    printf("m[%d] = %d\n", i, m[i]);
  }

  fclose(file);
  return 0;
}
*/