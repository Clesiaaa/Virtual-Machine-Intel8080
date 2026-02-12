#include <stdio.h>
#include <stdint.h>
#include "flags.h"

#ifndef CPU_H
#define CPU_H

typedef struct cpu
{
	uint8_t A, B, C, D, E, H, L;
	uint16_t PC;
	uint16_t SP;
	FLAGS f;
	uint8_t memory[0x100000];
	uint8_t interrupt_enable;
	uint8_t cycles;
} cpu;

void init_cpu(cpu *c);

#endif
