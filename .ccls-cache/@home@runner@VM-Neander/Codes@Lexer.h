#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum {
  TOKEN_INT,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_MAIS,
  TOKEN_MENOS,
  TOKEN_DIV,
  TOKEN_MULT,
  TOKEN_EOF
};

typedef struct {
  int type;
  int value;
} Token;

Token next_token();
int ExecutarLexer();