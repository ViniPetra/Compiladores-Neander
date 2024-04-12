#include "Assembler.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

No *novoNo(char *nome, uint16_t valor) {
  No *novo = (No *)malloc(sizeof(No));
  strcpy(novo->nome, nome);
  novo->valor = valor;
  novo->proximo = NULL;
  return novo;
}

void adicionarNo(No **cabeca, char *nome, uint16_t valor) {
  No *novo = novoNo(nome, valor);
  if (*cabeca == NULL) {
    *cabeca = novo;
    printf("[A]: Variável %s adicionada com valor %d\n\n", nome, valor);
    return;
  }
  No *atual = *cabeca;
  while (atual->proximo != NULL) {
    atual = atual->proximo;
  }
  atual->proximo = novo;

  printf("[A]: Variável %s adicionada com valor %d\n\n", nome, valor);
}

int buscarValorPorNome(No *cabeca, char *nome) {
  No *atual = cabeca;
  while (atual != NULL) {
    if (strcmp(atual->nome, nome) == 0) {
      printf("[A]: Variável encontrada: %s = %d\n", atual->nome, atual->valor);
      return atual->valor;
    }
    atual = atual->proximo;
  }

  return -1;
}

// Comentário = //
bool comentario(char *linha) { return (linha[0] == '/' && linha[1] == '/'); }

// A variável vai guardar um valor em um espaço de memória
// ex: @variavel 10
bool definicao_de_variavel(char *linha) { return (linha[0] == '@'); }

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

bool linha_vazia(char *linha) { return (linha[0] == '\n' || linha[0] == '\0'); }

void escrever_instrucao_dupla(char *mneumonico, uint16_t valor,
                              uint16_t *pos_memoria_codigo,
                              uint16_t *pos_memoria_valor) {
  char *mneumonicos[] = {"STA", "LDA", "ADD", "OR", "AND", "JMP", "JN", "JZ"};
  uint16_t codigos[] = {16, 32, 48, 64, 80, 128, 144, 160};

  for (int i = 0; i < 8; i++) {
    if (strcmp(mneumonico, mneumonicos[i]) == 0) {
      *pos_memoria_codigo = codigos[i];
      *pos_memoria_valor = valor;
      printf("Instrução %s %hu escrita na memória\n\n", mneumonico, valor);
      return;
    }
  }
}

void escrever_instrucao_simples(char *mneumonico,
                                uint16_t *pos_memoria_codigo) {
  char *mneumonicos[] = {"NOP", "NOT", "HLT"};
  uint16_t codigos[] = {0, 96, 240};

  for (int i = 0; i < 4; i++) {
    if (strcmp(mneumonico, mneumonicos[i]) == 0) {
      *pos_memoria_codigo = codigos[i];
      printf("Instrução %s escrita na memória\n\n", mneumonico);
      return;
    }
  }
}

void definir_variavel(char *nome, uint16_t valor, No **cabeca) {
  int i;
  for (i = 0; nome[i] != '\0'; ++i) {
    nome[i] = nome[i + 1];
  } // Remove o primeiro caractere da string (nesse caso, o @)
  adicionarNo(cabeca, nome, valor);
}

int Executar(void) {

  FILE *file = fopen("assembly.txt", "r");
  if (file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return 1;
  }

  char line[100];
  uint16_t m[256] = {};
  int i = 0;

  No *cabeca = NULL;

  while (fgets(line, sizeof(line), file) != NULL) {

    printf("[L]: %s -> ", line);

    if (comentario(line)) {
      printf("[A]: Comentário\n\n");
      continue;
    }
    if (linha_vazia(line)) {
      printf("[A]: Linha vazia\n\n");
      continue;
    }

    char instrucao[100];
    uint16_t valor;

    int qnt_vals = sscanf(line, "%s %hu", instrucao, &valor);

    if (qnt_vals == 2) {
      if (definicao_de_variavel(instrucao)) {

        printf("[A]: Definição de variável\n");
        definir_variavel(instrucao, (uint16_t)valor, &cabeca);
        continue;
      }
      if (mneumonico_valido(instrucao)) {

        printf("[A]: Mneumonico válido\n");
        printf("[A]: Instrução dupla\n");

        escrever_instrucao_dupla(instrucao, (uint16_t)valor, &m[i], &m[i + 1]);
        i += 2;
        continue;
      } else {
        printf("[A]: Mneumonico inválido: %s\n", instrucao);
        break;
      }
    } else if (qnt_vals == 1) {

      char valorString[100];

      sscanf(line, "%s %s", instrucao, valorString);

      int valorInt = buscarValorPorNome(cabeca, valorString);

      if (valorInt != -1) {
        if (mneumonico_valido(instrucao)) {

          printf("[A]: Mneumonico válido\n");
          printf("[A]: Instrução dupla\n");

          escrever_instrucao_dupla(instrucao, (uint16_t)valorInt, &m[i],
                                   &m[i + 1]);
          i += 2;
          continue;
        } else {
          printf("[A]: Mneumonico inválido: %s\n", instrucao);
          break;
        }
      }

      if (mneumonico_valido(instrucao)) {

        printf("[A]: Mneumonico válido\n");
        printf("[A]: Instrução simples\n");

        escrever_instrucao_simples(instrucao, &m[i]);
        i++;
        continue;
      } else {
        printf("[A]: Mneumonico inválido: %s\n", instrucao);
        break;
      }
    } else {
    }
  }

  printf("");
  for (int i = 0; i < 10; i++) {
    printf("m[%d] = %d\n", i, m[i]);
  }

  fclose(file);
  return 0;
}