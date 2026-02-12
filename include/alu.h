#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"

#ifndef ALU_H
#define ALU_H

void set_flag_add(cpu *c, uint8_t value, uint16_t arg);
void set_flag_adc(cpu *c, uint8_t value, uint16_t arg);
void set_flag_sub(cpu *c, uint8_t value, uint16_t arg);
void set_flag_sbb(cpu *c, uint8_t value, uint16_t arg, uint8_t borrow);
void set_flag_ana(cpu *c, uint8_t value, uint16_t arg);
void set_flag_xra(cpu *c, uint16_t arg);
void set_flag_cmp(cpu *c, uint8_t arg);
#endif
