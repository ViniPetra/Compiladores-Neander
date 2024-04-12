#include <stdint.h>
#include <stdio.h>

int ac = 0;
int pc = 0;
int n = 0;
int z = 0;

uint16_t m[256] = {};

void atualizar_flags() {
  if (ac < 0) {
    n = 1;
  } else {
    n = 0;
  }

  if (ac == 0) {
    z = 1;
  } else {
    z = 0;
  }
}

void nop() { pc++; }

void sta(unsigned char endereco) {
  m[endereco] = ac;
  atualizar_flags();
  pc += 2;
}

void lda(unsigned char endereco) {
  ac = m[endereco];
  atualizar_flags();
  pc += 2;
}

void add(unsigned char endereco) {
  ac += m[endereco];
  atualizar_flags();
  pc += 2;
}

void or (unsigned char endereco) {
  ac = ac | m[endereco];
  atualizar_flags();
  pc += 2;
}

void and (unsigned char endereco) {
  ac = ac & m[endereco];
  atualizar_flags();
  pc += 2;
}

void not() {
  ac = ~ac;
  atualizar_flags();
  pc++;
}

void jmp(unsigned char endereco) { pc = endereco; }

void jn(unsigned char endereco) {
  if (n == 1) {
    pc = endereco;
  } else {
    pc += 2;
  }
}

void jz(unsigned char endereco) {
  if (z == 1) {
    pc = endereco;
  } else {
    pc += 2;
  }
}

int processarInstrucao(uint16_t instrucao) {

  printf("Processando instrução: %u\n", instrucao);

  switch (instrucao) {
  case 0:
    nop();
    break;
  case 16:
    sta(m[pc + 1]);
    break;
  case 32:
    lda(m[pc + 1]);
    break;
  case 48:
    add(m[pc + 1]);
    break;
  case 64:
    or (m[pc + 1]);
    break;
  case 80:
    and(m[pc + 1]);
    break;
  case 96:
    not();
    break;
  case 128:
    jmp(m[pc + 1]);
    break;
  case 144:
    jn(m[pc + 1]);
    break;
  case 160:
    jz(m[pc + 1]);
    break;
  case 240:
    printf("AC: %d\n", ac);
    printf("PC: %d\n\n", pc);
    return 0;
    break;
  default:
    printf("Instrução desconhecida: %u\n", instrucao);
    return 0;
    break;
  }

  printf("AC: %d\n", ac);
  printf("PC: %d\n\n", pc);
  return 1;
}

int main1(void) {

  FILE *file = fopen("teste.mem", "r");
  if (file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return 1;
  }

  uint16_t temp[258] = {};
  uint16_t x = 0;

  fread(&x, sizeof(uint16_t), 2, file); // Joga fora os 2 primeiros (header)

  for (int i = 0; i < 258; i++) {
    fread(&x, sizeof(uint16_t), 1, file);
    m[i] = x;
  }

  fclose(file);

  /*
  for (int i = 0; i < 10; i++) {
    printf("m[%d] = %d\n", i, m[i]);
  }
  */

  int rodando = 1;

  while (rodando) {
    rodando = processarInstrucao(m[pc]);
  }

  return 0;
}
