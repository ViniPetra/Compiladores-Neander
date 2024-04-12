#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
  char nome[50];
  uint16_t valor;
  struct No *proximo;
} No;

No *novoNo(char *nome, uint16_t valor);

void adicionarNo(No **cabeca, char *nome, uint16_t valor);

int buscarValorPorNome(No *cabeca, char *nome);

bool comentario(char *linha);

bool definicao_de_variavel(char *linha);

bool mneumonico_valido(char *mneumonico);

bool linha_vazia(char *linha);

void escrever_instrucao_dupla(char *mneumonico, uint16_t valor,
                              uint16_t *pos_memoria_codigo,
                              uint16_t *pos_memoria_valor);

void escrever_instrucao_simples(char *mneumonico, uint16_t *pos_memoria_codigo);

void definir_variavel(char *nome, uint16_t valor, No **cabeca);

int Executar(void);