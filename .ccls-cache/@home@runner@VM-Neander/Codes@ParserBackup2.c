#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Protótipos das funções
int expressao();
int termo();
int fator();

Token token_atual;

int VALORES[256];
int valores_idx = 0;
char INSTRUCOES[1024] = "";

int registrar_valor(int valor) {
  VALORES[valores_idx] = valor;
  return valores_idx++;
}

void adicionar_instrucao(const char *instrucao, int idx) {
  char buffer[50];
  sprintf(buffer, "%s %d\n", instrucao, idx);
  strcat(INSTRUCOES, buffer);
}

// Função para erro
void erro(const char *msg) {
  printf("Erro: %s\n", msg);
  exit(1);
}

// Avançar para o próximo token
void avancar_token(int token_type_esperado) {
  if (token_atual.type == token_type_esperado)
    token_atual = next_token();
  else
    erro("Token inesperado");
}

// Fator: Número ou Expressão entre parênteses
int fator() {
  int resultado = 0;
  if (token_atual.type == TOKEN_INT) {
    resultado = token_atual.value;
    avancar_token(TOKEN_INT);
  } else if (token_atual.type == TOKEN_LPAREN) {
    avancar_token(TOKEN_LPAREN);
    resultado = expressao();
    avancar_token(TOKEN_RPAREN);
  } else {
    erro("Fator inesperado");
  }
  return resultado;
}

// Termo: Fator seguido por operações de multiplicação ou divisão
int termo() {
  int resultado = fator();
  while (token_atual.type == TOKEN_MULT || token_atual.type == TOKEN_DIV) {
    if (token_atual.type == TOKEN_MULT) {
      avancar_token(TOKEN_MULT);
      resultado *= fator();
    } else if (token_atual.type == TOKEN_DIV) {
      avancar_token(TOKEN_DIV);
      int divisor = fator();
      if (divisor != 0)
        resultado /= divisor;
      else
        erro("Divisão por zero");
    }
  }
  return resultado;
}

// Expressão: Termo seguido por operações de soma ou subtração
int expressao() {
  int resultado = termo();
  while (token_atual.type == TOKEN_MAIS || token_atual.type == TOKEN_MENOS) {
    int valor = resultado;
    if (token_atual.type == TOKEN_MAIS) {
      avancar_token(TOKEN_MAIS);
      int res = termo();
      resultado += termo();
    } else if (token_atual.type == TOKEN_MENOS) {
      avancar_token(TOKEN_MENOS);
      int t = termo();
      resultado = registrar_valor(VALORES[valor] - VALORES[t]);
    }
  }
  return resultado;
}

int ExecutarParser() {
  token_atual = next_token();
  int resultado = expressao();
  printf("r: %d\n", resultado);

  /*
  printf("Valores:\n");
  for (int i = 0; i < valores_idx; ++i) {
    printf("%d: %d\n", i, VALORES[i]);
  }
  printf("Instruções:\n%s", INSTRUCOES);
  */
  return 0;
}
