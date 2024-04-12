#ifndef NEANDER_VM_H
#define NEANDER_VM_H

#include <stdint.h>

extern int ac;
extern int pc;
extern int n;
extern int z;

extern uint16_t m[256];

void atualizar_flags();
void nop();
void sta(unsigned char endereco);
void lda(unsigned char endereco);
void add(unsigned char endereco);
void or_op(unsigned char endereco);
void and_op(unsigned char endereco);
void not_op();
void jmp(unsigned char endereco);
void jn(unsigned char endereco);
void jz(unsigned char endereco);
int processarInstrucao(uint16_t instrucao);
int LerEExecutar(void);

#endif /* NEANDER_VM_H */