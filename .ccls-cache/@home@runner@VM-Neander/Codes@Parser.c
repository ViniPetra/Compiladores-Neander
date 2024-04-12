#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBERS 256

int ExecutarParser() {
  int numbers[MAX_NUMBERS];
  int num_count = 0;

  Token token;
  while (1) {
    token = next_token();
    if (token.type == TOKEN_INT) {
      if (num_count >= MAX_NUMBERS) {
        printf("Erro: Muitos números na entrada.\n");
        exit(1);
      }
      numbers[num_count++] = token.value;
    } else if (token.type == TOKEN_EOF) {
      break;
    }
  }

  FILE *file = fopen("output.txt", "w");

  if (file == NULL) {
    printf("Erro ao abrir o arquivo.");
    return 1;
  }

  fprintf(file, "DATA\n");

  for (int i = 0; i < num_count; i++) {
    fprintf(file, "\t%d %d\n", i, numbers[i]);
  }

  fprintf(file, "END\n\n");

  fprintf(file, "CODE\n");

  fprintf(file, "\tLDA 0\n");

  for (int i = 0; i < num_count; i++) {
    fprintf(file, "\tADD %d\n", i + 1);
  }

  fprintf(file, "END\n\n");

  fclose(file);

  return 0;
}