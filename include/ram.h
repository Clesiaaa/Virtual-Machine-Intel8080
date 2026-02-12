#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cpu.h"

#ifndef RAM_H
#define RAM_H

void read_memory(cpu *c, uint16_t addr);
void write_memory(cpu *c, uint16_t addr, uint8_t opcode);
void init_ram(cpu *c);
void load_rom(cpu *c, char *rom_path);
void read_all_memory(cpu *c);

#endif
