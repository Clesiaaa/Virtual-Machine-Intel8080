#include <stdint.h>

#include "../../include/cpu.h"
#include "../../include/flags.h"

void init_cpu(cpu *c)
{
	c->A = 0x00;
	c->B = 0x00;
    c->C = 0x00;
    c->D = 0x00;
    c->E = 0x00;
    c->H = 0x00;
    c->L = 0x00;
    c->f.flags = 0b00000010;
    c->PC = 0x0000;
    c->SP = 0xFFFF;
    c->cycles = 0b00000000;
    //printf("cpu is initialized...\n");
};
