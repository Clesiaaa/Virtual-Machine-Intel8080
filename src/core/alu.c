#include <stdint.h>

#include "../../include/alu.h"
#include "../../include/cpu.h"
#include "../../include/flags.h"

void set_flag_add(cpu *c, uint8_t value, uint16_t result)
{
    if (result > 0xFF) {
        c->f.flags |= (1 << 0);
    } else {
        c->f.flags &= ~(1 << 0);
    }

    if ((result & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((result & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    if (((c->A & 0x0F) + (value & 0x0F)) > 0x0F) {
        c->f.flags |= (1 << 4);
    } else {
        c->f.flags &= ~(1 << 4);
    }

    uint8_t count_p = 0;
    uint8_t temp = result & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }
};

void set_flag_adc(cpu *c, uint8_t value, uint16_t result)
{
    if (result > 0xFF) {
        c->f.flags |= (1 << 0);
    } else {
        c->f.flags &= ~(1 << 0);
    }

    if ((result & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((result & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    if (((c->A & 0x0F) + (value & 0x0F) + ((c->f.flags >> 0) & 1)) > 0x0F) {
        c->f.flags |= (1 << 4);
    } else {
        c->f.flags &= ~(1 << 4);
    }

    uint8_t count_p = 0;
    uint8_t temp = result & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }
};

void set_flag_sub(cpu *c, uint8_t value, uint16_t result)
{

    if (c->A < value) {
        c->f.flags |= (1 << 0);
    } else {
        c->f.flags &= ~(1 << 0);
    }

    if ((result & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((result & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    if ((c->A & 0x0F) < (value & 0x0F)) {
        c->f.flags |= (1 << 4);
    } else {
        c->f.flags &= ~(1 << 4);
    }

    uint8_t count_p = 0;
    uint8_t temp = result & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }

};

void set_flag_sbb(cpu *c, uint8_t value, uint16_t result, uint8_t borrow)
{

    if (c->A < value + borrow) {
        c->f.flags |= (1 << 0);
    } else {
        c->f.flags &= ~(1 << 0);
    }

    if ((result & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((result & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    if ((c->A & 0x0F) < (value & 0x0F)) {
        c->f.flags |= (1 << 4);
    } else {
        c->f.flags &= ~(1 << 4);
    }

    uint8_t count_p = 0;
    uint8_t temp = result & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }

};

void set_flag_ana(cpu *c, uint8_t value, uint16_t arg)
{
    c->f.flags &= ~(1 << 0); //cy
    c->f.flags |= (1 << 4); //ac

    if ((arg & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((arg & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    uint8_t count_p = 0;
    uint8_t temp = arg & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }
};

void set_flag_xra(cpu *c, uint16_t arg)
{
    c->f.flags |= (1 << 4); //ac

    if ((arg & 0xFF) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((arg & 0x80) != 0) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    uint8_t count_p = 0;
    uint8_t temp = arg & 0xFF;

    for (uint8_t i = 0; i < 8; i++) {
        if (temp & 1) {
            count_p++;
        }
        temp >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }

};


void set_flag_cmp(cpu *c, uint8_t value)
{
    if (c->A < value) {
        c->f.flags |= (1 << 0);
    } else {
        c->f.flags &= ~(1 << 0);
    }

    if ((c->A - value) == 0x00) {
        c->f.flags |= (1 << 6);
    } else {
        c->f.flags &= ~(1 << 6);
    }

    if ((c->A - value) & 0x80) {
        c->f.flags |= (1 << 7);
    } else {
        c->f.flags &= ~(1 << 7);
    }

    if (((c->A & 0x0F) - (value & 0x0F)) < 0x00) {
        c->f.flags |= (1 << 4);
    } else {
        c->f.flags &= ~(1 << 4);
    }

    uint8_t result = c->A - value;
    uint8_t count_p = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (result & 1) {
            count_p++;
        }
        result >>= 1;
    }

    if (count_p % 2 == 0) {
        c->f.flags |= (1 << 2);
    } else {
        c->f.flags &= ~(1 << 2);
    }
}
