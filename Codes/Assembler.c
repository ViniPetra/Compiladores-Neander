#include "Assembler.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Recebe um mneumonico, um numero de posição de memória e
// duas posições da memória virtal.
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

void escrever_arquivo(uint16_t m[256]) {
  FILE *file = fopen("Arquivos/memoria_gerada.mem", "w");
  if (file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return;
  }

  uint8_t headers[4] = {0x03, 0x4E, 0x44, 0x52};

  for (int i = 0; i < 8; i++) {
    fwrite(&headers[i], sizeof(uint8_t), 1, file);
  }

  for (int i = 0; i < 256; i++) {
    fwrite(&m[i], sizeof(uint16_t), 1, file);
  }

  fclose(file);
}

int ExecutarAssembler(void) {

  FILE *file = fopen("Arquivos/assembly.txt", "r");
  if (file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return 1;
  }

  char line[100];       // Linha de texto do assembly.txt
  uint16_t m[256] = {}; // Memória virtual
  int i = 0;            // Posição da memória virtual para instruções

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

        if (cont_palavras == 2) {
          uint16_t posicao = (uint16_t)atoi(palavras[0]);
          uint16_t valor = (uint16_t)atoi(palavras[1]);

          if (valor > 255) {
            printf("\t[A]: Valor da variável %hu maior que 255\n", valor);
            return -1;
          }

          m[posicao] = valor;

          printf("\t[A] Variável\n\t\tposição: %hu\n\t\tvalor: "
                 "%hu\n",
                 posicao, valor);
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
          uint16_t valor = (uint16_t)atoi(palavras[1]);
          escrever_instrucao_dupla(palavras[0], valor, &m[i], &m[i + 1]);
          i += 2;
        }
      }
      printf("\t[A]: Fim do bloco de código\n\n");
      continue;
    }
  }

  for (int i = 0; i < 256; i++) {
    printf("m[%d] = %d\n", i, m[i]);
  }

  fclose(file);

  escrever_arquivo(m);

  return 0;
}