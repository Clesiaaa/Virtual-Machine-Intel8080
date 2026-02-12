#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../include/cpu.h"
#include "../../include/ram.h"

void read_memory(cpu *c, uint16_t addr)
{
	uint8_t opcode;
	opcode = c->memory[addr];
	printf("| address : 0x%X | contains : 0x%X |\n", addr, opcode);
};

void write_memory(cpu *c, uint16_t addr, uint8_t opcode)
{
	c->memory[addr] = opcode;
	//printf("| 0x%X : 0x%X |\n", opcode, addr);
};

void init_ram(cpu *c)
{
    for (uint16_t addr = 0x0000; addr < 0xFFFF; addr++)
    {
        c->memory[addr] = 0x10;
    }
    //printf("ram is initialized...\n");
};

void load_rom(cpu *c, char *rom_path)
{
    FILE* rptr;

    rptr = fopen(rom_path, "r");

    if (rptr == NULL)
    {
        printf("rom could not be open.\n");
    }else{
        //printf("loading rom in memory...\n");
        uint16_t addr = 0x0000;
        unsigned int byte;
        while (addr < 0xFFFF && fscanf(rptr, "%x", &byte) == 1) {
             write_memory(c, addr, byte);
             addr++;
         }
        //printf("|opcode : addr|\n");
    }
    //printf("loaded !\n");
    fclose(rptr);
};

void read_all_memory(cpu *c)
{
    printf("reading memory...\n");
    for (uint16_t addr = 0x0000; addr < 0xFFFF; addr++) read_memory(c, addr);
};
