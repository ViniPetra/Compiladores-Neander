#ifndef NEANDER_VM_H
#define NEANDER_VM_H

#include <stdint.h>

extern int ac;
extern int pc;
extern int n;
extern int z;

extern uint16_t m[256];

int LerEExecutar(void);

#endif /* NEANDER_VM_H */