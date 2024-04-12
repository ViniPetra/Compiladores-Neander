/*
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>

// Protótipos das funções
int expressao();
int termo();
int fator();

Token token_atual;

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
    if (token_atual.type == TOKEN_MAIS) {
      avancar_token(TOKEN_MAIS);
      resultado += termo();
    } else if (token_atual.type == TOKEN_MENOS) {
      avancar_token(TOKEN_MENOS);
      resultado -= termo();
    }
  }
  return resultado;
}

int ExecutarParser() {
  token_atual = next_token();
  int resultado = expressao();
  printf("r: %d", resultado);
  return 0;
}
*/