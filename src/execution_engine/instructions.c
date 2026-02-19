#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "../../include/instructions.h"
#include "../../include/cpu.h"
#include "../../include/alu.h"

#define DEBUG_MODE 0

#if DEBUG_MODE
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif

static const uint8_t CYCLES_TABLE[256] = {
    // 0x00-0x0F
    4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,
    // 0x10-0x1F
    4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,
    // 0x20-0x2F
    4,  10, 16, 5,  5,  5,  7,  4,  4,  10, 16, 5,  5,  5,  7,  4,
    // 0x30-0x3F
    4,  10, 13, 5,  10, 10, 10, 4,  4,  10, 13, 5,  5,  5,  7,  4,
    // 0x40-0x4F
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
    // 0x50-0x5F
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
    // 0x60-0x6F
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
    // 0x70-0x7F
    7,  7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,
    // 0x80-0x8F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
    // 0x90-0x9F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
    // 0xA0-0xAF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
    // 0xB0-0xBF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
    // 0xC0-0xCF
    11, 10, 10, 10, 17, 11, 7,  11, 11, 10, 10, 10, 17, 17, 7,  11,
    // 0xD0-0xDF
    11, 10, 10, 10, 17, 11, 7,  11, 11, 10, 10, 10, 17, 17, 7,  11,
    // 0xE0-0xEF
    11, 10, 10, 18, 17, 11, 7,  11, 11, 5,  10, 4,  17, 17, 7,  11,
    // 0xF0-0xFF
    11, 10, 10, 4,  17, 11, 7,  11, 11, 5,  10, 4,  17, 17, 7,  11
};

uint8_t run(cpu *c)
{
    //printf("running rom...\n");

    while (c->PC < 0xFFFF)
    {

        switch (c->memory[c->PC])
        {
            // ===== CONTROL =====
            case 0x00: // NOP
                DEBUG_PRINT("0x00 NOP\n");
                c->PC++;
                break;

            case 0x76: // HLT
                DEBUG_PRINT("0x76 halt...\n");
                return 0x00;

            // ===== LXI (Load Extended Immediate) =====
            case 0x01: // LXI B, d16
                c->C = c->memory[c->PC + 1];
                c->B = c->memory[c->PC + 2];
                c->PC += 3;
                DEBUG_PRINT("0x01 lxi b\n");
                break;

            case 0x11: // LXI D, d16
                c->E = c->memory[c->PC + 1];
                c->D = c->memory[c->PC + 2];
                c->PC += 3;
                DEBUG_PRINT("0x11 lxi d\n");
                break;

            case 0x21: // LXI H, d16
                c->L = c->memory[c->PC + 1];
                c->H = c->memory[c->PC + 2];
                c->PC += 3;
                DEBUG_PRINT("0x21 lxi h\n");
                break;

            case 0x31: // LXI SP, d16
                c->SP = c->memory[c->PC + 1] | (c->memory[c->PC + 2] << 8);
                c->PC += 3;
                DEBUG_PRINT("0x31 lxi sp\n");
                break;

            // ===== MVI (Move Immediate) =====
            case 0x06: // MVI B, d8
                c->B = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x06 mvi b\n");
                break;

            case 0x0E: // MVI C, d8
                c->C = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x0E mvi c\n");
                break;

            case 0x16: // MVI D, d8
                c->D = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x16 mvi d\n");
                break;

            case 0x1E: // MVI E, d8
                c->E = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x1E mvi e\n");
                break;

            case 0x26: // MVI H, d8
                c->H = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x26 mvi h\n");
                break;

            case 0x2E: // MVI L, d8
                c->L = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x2E mvi l\n");
                break;

            case 0x36: // MVI M, d8
                c->memory[(c->H << 8) | c->L] = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x36 mvi m\n");
                break;

            case 0x3E: // MVI A, d8
                c->A = c->memory[c->PC + 1];
                c->PC += 2;
                DEBUG_PRINT("0x3E mvi a\n");
                break;

            // ===== STAX (Store Accumulator Indexed) =====
            case 0x02: // STAX B
                c->memory[(c->B << 8) | c->C] = c->A;
                c->PC++;
                DEBUG_PRINT("0x02 stax b\n");
                break;

            case 0x12: // STAX D
                c->memory[(c->D << 8) | c->E] = c->A;
                c->PC++;
                DEBUG_PRINT("0x12 stax d\n");
                break;

            // ===== LDAX (Load Accumulator Indexed) =====
            case 0x0A: // LDAX B
                c->A = c->memory[(c->B << 8) | c->C];
                c->PC++;
                DEBUG_PRINT("0x0A ldax b\n");
                break;

            case 0x1A: // LDAX D
                c->A = c->memory[(c->D << 8) | c->E];
                c->PC++;
                DEBUG_PRINT("0x1A ldax d\n");
                break;

            // ===== SHLD/LHLD (Store/Load H and L Direct) =====
            case 0x22: // SHLD a16
                c->memory[(c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]] = c->L;
                c->memory[((c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]) + 1] = c->H;
                c->PC += 3;
                DEBUG_PRINT("0x22 shld\n");
                break;

            case 0x2A: // LHLD a16
                c->L = c->memory[(c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]];
                c->H = c->memory[((c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]) + 1];
                c->PC += 3;
                DEBUG_PRINT("0x2A lhld\n");
                break;

            // ===== STA/LDA (Store/Load Accumulator Direct) =====
            case 0x32: // STA a16
                c->memory[(c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]] = c->A;
                c->PC += 3;
                DEBUG_PRINT("0x32 sta\n");
                break;

            case 0x3A: // LDA a16
                c->A = c->memory[(c->memory[c->PC + 2] << 8) | c->memory[c->PC + 1]];
                c->PC += 3;
                DEBUG_PRINT("0x3A lda\n");
                break;

            // ===== MOV B,r =====
            case 0x40: c->B = c->B; c->PC++; DEBUG_PRINT("0x40 mov b,b\n"); break;
            case 0x41: c->B = c->C; c->PC++; DEBUG_PRINT("0x41 mov b,c\n"); break;
            case 0x42: c->B = c->D; c->PC++; DEBUG_PRINT("0x42 mov b,d\n"); break;
            case 0x43: c->B = c->E; c->PC++; DEBUG_PRINT("0x43 mov b,e\n"); break;
            case 0x44: c->B = c->H; c->PC++; DEBUG_PRINT("0x44 mov b,h\n"); break;
            case 0x45: c->B = c->L; c->PC++; DEBUG_PRINT("0x45 mov b,l\n"); break;
            case 0x46: c->B = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x46 mov b,m\n"); break;
            case 0x47: c->B = c->A; c->PC++; DEBUG_PRINT("0x47 mov b,a\n"); break;

            // ===== MOV C,r =====
            case 0x48: c->C = c->B; c->PC++; DEBUG_PRINT("0x48 mov c,b\n"); break;
            case 0x49: c->C = c->C; c->PC++; DEBUG_PRINT("0x49 mov c,c\n"); break;
            case 0x4A: c->C = c->D; c->PC++; DEBUG_PRINT("0x4A mov c,d\n"); break;
            case 0x4B: c->C = c->E; c->PC++; DEBUG_PRINT("0x4B mov c,e\n"); break;
            case 0x4C: c->C = c->H; c->PC++; DEBUG_PRINT("0x4C mov c,h\n"); break;
            case 0x4D: c->C = c->L; c->PC++; DEBUG_PRINT("0x4D mov c,l\n"); break;
            case 0x4E: c->C = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x4E mov c,m\n"); break;
            case 0x4F: c->C = c->A; c->PC++; DEBUG_PRINT("0x4F mov c,a\n"); break;

            // ===== MOV D,r =====
            case 0x50: c->D = c->B; c->PC++; DEBUG_PRINT("0x50 mov d,b\n"); break;
            case 0x51: c->D = c->C; c->PC++; DEBUG_PRINT("0x51 mov d,c\n"); break;
            case 0x52: c->D = c->D; c->PC++; DEBUG_PRINT("0x52 mov d,d\n"); break;
            case 0x53: c->D = c->E; c->PC++; DEBUG_PRINT("0x53 mov d,e\n"); break;
            case 0x54: c->D = c->H; c->PC++; DEBUG_PRINT("0x54 mov d,h\n"); break;
            case 0x55: c->D = c->L; c->PC++; DEBUG_PRINT("0x55 mov d,l\n"); break;
            case 0x56: c->D = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x56 mov d,m\n"); break;
            case 0x57: c->D = c->A; c->PC++; DEBUG_PRINT("0x57 mov d,a\n"); break;

            // ===== MOV E,r =====
            case 0x58: c->E = c->B; c->PC++; DEBUG_PRINT("0x58 mov e,b\n"); break;
            case 0x59: c->E = c->C; c->PC++; DEBUG_PRINT("0x59 mov e,c\n"); break;
            case 0x5A: c->E = c->D; c->PC++; DEBUG_PRINT("0x5A mov e,d\n"); break;
            case 0x5B: c->E = c->E; c->PC++; DEBUG_PRINT("0x5B mov e,e\n"); break;
            case 0x5C: c->E = c->H; c->PC++; DEBUG_PRINT("0x5C mov e,h\n"); break;
            case 0x5D: c->E = c->L; c->PC++; DEBUG_PRINT("0x5D mov e,l\n"); break;
            case 0x5E: c->E = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x5E mov e,m\n"); break;
            case 0x5F: c->E = c->A; c->PC++; DEBUG_PRINT("0x5F mov e,a\n"); break;

            // ===== MOV H,r =====
            case 0x60: c->H = c->B; c->PC++; DEBUG_PRINT("0x60 mov h,b\n"); break;
            case 0x61: c->H = c->C; c->PC++; DEBUG_PRINT("0x61 mov h,c\n"); break;
            case 0x62: c->H = c->D; c->PC++; DEBUG_PRINT("0x62 mov h,d\n"); break;
            case 0x63: c->H = c->E; c->PC++; DEBUG_PRINT("0x63 mov h,e\n"); break;
            case 0x64: c->H = c->H; c->PC++; DEBUG_PRINT("0x64 mov h,h\n"); break;
            case 0x65: c->H = c->L; c->PC++; DEBUG_PRINT("0x65 mov h,l\n"); break;
            case 0x66: c->H = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x66 mov h,m\n"); break;
            case 0x67: c->H = c->A; c->PC++; DEBUG_PRINT("0x67 mov h,a\n"); break;

            // ===== MOV L,r =====
            case 0x68: c->L = c->B; c->PC++; DEBUG_PRINT("0x68 mov l,b\n"); break;
            case 0x69: c->L = c->C; c->PC++; DEBUG_PRINT("0x69 mov l,c\n"); break;
            case 0x6A: c->L = c->D; c->PC++; DEBUG_PRINT("0x6A mov l,d\n"); break;
            case 0x6B: c->L = c->E; c->PC++; DEBUG_PRINT("0x6B mov l,e\n"); break;
            case 0x6C: c->L = c->H; c->PC++; DEBUG_PRINT("0x6C mov l,h\n"); break;
            case 0x6D: c->L = c->L; c->PC++; DEBUG_PRINT("0x6D mov l,l\n"); break;
            case 0x6E: c->L = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x6E mov l,m\n"); break;
            case 0x6F: c->L = c->A; c->PC++; DEBUG_PRINT("0x6F mov l,a\n"); break;

            // ===== MOV M,r =====
            case 0x70: c->memory[(c->H << 8) | c->L] = c->B; c->PC++; DEBUG_PRINT("0x70 mov m,b\n"); break;
            case 0x71: c->memory[(c->H << 8) | c->L] = c->C; c->PC++; DEBUG_PRINT("0x71 mov m,c\n"); break;
            case 0x72: c->memory[(c->H << 8) | c->L] = c->D; c->PC++; DEBUG_PRINT("0x72 mov m,d\n"); break;
            case 0x73: c->memory[(c->H << 8) | c->L] = c->E; c->PC++; DEBUG_PRINT("0x73 mov m,e\n"); break;
            case 0x74: c->memory[(c->H << 8) | c->L] = c->H; c->PC++; DEBUG_PRINT("0x74 mov m,h\n"); break;
            case 0x75: c->memory[(c->H << 8) | c->L] = c->L; c->PC++; DEBUG_PRINT("0x75 mov m,l\n"); break;
            case 0x77: c->memory[(c->H << 8) | c->L] = c->A; c->PC++; DEBUG_PRINT("0x77 mov m,a\n"); break;

            // ===== MOV A,r =====
            case 0x78: c->A = c->B; c->PC++; DEBUG_PRINT("0x78 mov a,b\n"); break;
            case 0x79: c->A = c->C; c->PC++; DEBUG_PRINT("0x79 mov a,c\n"); break;
            case 0x7A: c->A = c->D; c->PC++; DEBUG_PRINT("0x7A mov a,d\n"); break;
            case 0x7B: c->A = c->E; c->PC++; DEBUG_PRINT("0x7B mov a,e\n"); break;
            case 0x7C: c->A = c->H; c->PC++; DEBUG_PRINT("0x7C mov a,h\n"); break;
            case 0x7D: c->A = c->L; c->PC++; DEBUG_PRINT("0x7D mov a,l\n"); break;
            case 0x7E: c->A = c->memory[(c->H << 8) | c->L]; c->PC++; DEBUG_PRINT("0x7E mov a,m\n"); break;
            case 0x7F: c->A = c->A; c->PC++; DEBUG_PRINT("0x7F mov a,a\n"); break;

            // ===== POP r =====
            case 0xC1: //POP B
                c->C = c->memory[c->SP];
                c->B = c->memory[c->SP + 1];
                c->SP += 2;
                c->PC++;
                DEBUG_PRINT("0xC1 pop b\n");
                break;

            case 0xD1: //POP D
                c->E = c->memory[c->SP];
                c->D = c->memory[c->SP + 1];
                c->SP += 2;
                c->PC++;
                DEBUG_PRINT("0xD1 pop d\n");
                break;

            case 0xE1: //POP H
                c->L = c->memory[c->SP];
                c->H = c->memory[c->SP + 1];
                c->SP += 2;
                c->PC++;
                DEBUG_PRINT("0xE1 pop h\n");
                break;

            case 0xF1: //POP PSW
                c->f.flags = c->memory[c->SP];
                c->A = c->memory[c->SP + 1];
                c->SP += 2;
                c->PC++;
                DEBUG_PRINT("0xF1 pop psw\n");
                break;

            // ===== PUSH, r =====
            case 0xC5: //PUSH B
                c->SP -= 2;
                c->memory[c->SP] = c->C;
                c->memory[c->SP + 1] = c->B;
                c->PC++;
                DEBUG_PRINT("0xC5 push b\n");
                break;

            case 0xD5: //PUSH D
                c->SP -= 2;
                c->memory[c->SP] = c->E;
                c->memory[c->SP + 1] = c->D;
                c->PC++;
                DEBUG_PRINT("0xD5 push d\n");
                break;

            case 0xE5: //PUSH H
                c->SP -= 2;
                c->memory[c->SP] = c->L;
                c->memory[c->SP + 1] = c->H;
                c->PC++;
                DEBUG_PRINT("0xE5 push h\n");
                break;

            case 0xF5: //PUSH PSW
                c->SP -= 2;
                c->memory[c->SP] = c->f.flags;
                c->memory[c->SP + 1] = c->A;
                c->PC++;
                DEBUG_PRINT("0xF5 push psw\n");
                break;

            // ===== XHTL =====
            case 0xE3:
            {
                //swap L with memory[sp]
                uint8_t temp_L = c->L;
                c->L = c->memory[c->SP];
                c->memory[c->SP] = temp_L;

                //swap h with memory[sp + 1]
                uint8_t temp_H = c->H;
                c->H = c->memory[c->SP + 1];
                c->memory[c->SP + 1] = temp_H;

                c->PC++;
                DEBUG_PRINT("0xE3 xhtl\n");
                break;
            };

            // ===== SPHL =====
            case 0xF9:
                c->SP = (c->H << 8) | c->L;
                c->PC++;
                DEBUG_PRINT("0xF9 sphl\n");
                break;

            // ===== XCHG =====
            case 0xEB:
            {
                //swap h and d
                uint8_t temp_h = c->H;
                c->H = c->D;
                c->D = temp_h;

                //swap l and e
                uint8_t temp_l = c->L;
                c->L = c->E;
                c->E = temp_l;

                c->PC++;
                DEBUG_PRINT("0xEB xchg\n");
                break;
            };

            // ===== OUT =====
            case 0xD3:
            {
                printf("%c", c->A);
                c->PC += 2;
                break;
            };

            case 0xDB: // IN port
            {
                uint8_t port_num = c->memory[c->PC + 1];
                c->A = 0x00;
                printf("0xDB in : read 0x%02X from port 0x%02X (simulated)\n", c->A, port_num);
                c->PC += 2;
                break;
            };

            // ====== DI ======
            case 0xF3:
                c->interrupt_enable = 0;
                DEBUG_PRINT("0xF3 di\n");
                c->PC++;
                break;

            // ===== EI =====
            case 0xFB:
                c->interrupt_enable = 1;
                DEBUG_PRINT("0xFB ei\n");
                c->PC++;
                break;


            // ============  ALU  ============

            // ===== ADD, r ======
            case 0x80: // ADD B
            {
                uint16_t result = c->A + c->B;
                set_flag_add(c, c->B, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x80 add b\n");
                break;
            }

            case 0x81: // ADD C
            {
                uint16_t result = c->A + c->C;
                set_flag_add(c, c->C, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x81 add c\n");
                break;
            }

            case 0x82: // ADD D
            {
                uint16_t result = c->A + c->D;
                set_flag_add(c, c->D, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x82 add d\n");
                break;
            }

            case 0x83: // ADD E
            {
                uint16_t result = c->A + c->E;
                set_flag_add(c, c->E, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x83 add e\n");
                break;
            }

            case 0x84: // ADD H
            {
                uint16_t result = c->A + c->H;
                set_flag_add(c, c->H, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x84 add h\n");
                break;
            }

            case 0x85: // ADD L
            {
                uint16_t result = c->A + c->L;
                set_flag_add(c, c->L, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x85 add l\n");
                break;
            }

            case 0x86: // ADD M
            {
                uint8_t value = c->memory[(c->H << 8) | c->L];
                uint16_t result = c->A + value;
                set_flag_add(c, value, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x86 add m\n");
                break;
            }

            case 0x87: // ADD A
            {
                uint16_t result = c->A + c->A;
                set_flag_add(c, c->A, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x87 add a\n");
                break;
            }

            case 0xC6: // ADI d8
            {
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A + value;
                set_flag_add(c, value, result);
                c->A = result & 0xFF;
                c->PC += 2;  // ← 2 bytes !
                DEBUG_PRINT("0xC6 adi\n");
                break;
            }

            // ===== ADC, r =====
            case 0x88: //ADC B
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->B + carry;
                set_flag_adc(c, c->B, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x88 adc b\n");
                break;
            }

            case 0x89: //ADC C
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->C + carry;
                set_flag_adc(c, c->C, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x89 adc c\n");
                break;
            }

            case 0x8A: //ADC D
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->D + carry;
                set_flag_adc(c, c->D, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8A adc d\n");
                break;
            }

            case 0x8B: //ADC E
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->E + carry;
                set_flag_adc(c, c->E, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8B adc e\n");
                break;
            }

            case 0x8C: //ADC H
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->H + carry;
                set_flag_adc(c, c->H, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8C adc h\n");
                break;
            }

            case 0x8D: //ADC L
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->L + carry;
                set_flag_adc(c, c->L, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8D adc l\n");
                break;
            }

            case 0x8E: //ADC M
            {
                uint8_t carry = c->f.flags & 1;
                uint8_t value = c->memory[(c->H << 8) | c->L];
                uint16_t result = c->A + value + carry;
                set_flag_adc(c, value, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8E adc m\n");
                break;
            }

            case 0x8F: //ADC A
            {
                uint8_t carry = c->f.flags & 1;
                uint16_t result = c->A + c->A + carry;
                set_flag_adc(c, c->A, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x8F adc a\n");
                break;
            }

            // ===== ACI d8 (ADC Immediate) =====
            case 0xCE: //ACI d8
            {
                uint8_t carry = c->f.flags & 1;
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A + value + carry;
                set_flag_adc(c, value, result);
                c->A = result & 0xFF;
                c->PC += 2;
                DEBUG_PRINT("0xCE aci\n");
                break;
            }

            // ===== SUB, r =====
            case 0x90: //SUB B
            {
                uint16_t result = c->A - c->B;
                set_flag_sub(c, c->B, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x90 sub b\n");
                break;
            }

            case 0x91: //SUB C
            {
                uint16_t result = c->A - c->C;
                set_flag_sub(c, c->C, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x91 sub c\n");
                break;
            }

            case 0x92: //SUB D
            {
                uint16_t result = c->A - c->D;
                set_flag_sub(c, c->D, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x92 sub d\n");
                break;
            }

            case 0x93: //SUB E
            {
                uint16_t result = c->A - c->E;
                set_flag_sub(c, c->E, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x93 sub e\n");
                break;
            }

            case 0x94: //SUB H
            {
                uint16_t result = c->A - c->H;
                set_flag_sub(c, c->H, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x94 sub h\n");
                break;
            }

            case 0x95: //SUB L
            {
                uint16_t result = c->A - c->L;
                set_flag_sub(c, c->L, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x95 sub l\n");
                break;
            }

            case 0x96: //SUB M
            {
                uint8_t value = c->memory[(c->H << 8) | c->L];
                uint16_t result = c->A - value;
                set_flag_sub(c, value, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x96 sub m\n");
                break;
            }

            case 0x97: //SUB A
            {
                uint16_t result = c->A - c->A;
                set_flag_sub(c, c->A, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x97 sub a\n");
                break;
            }

            // ===== SUI d8 (SUB Immediate) =====
            case 0xD6: //SUI d8
            {
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A - value;
                set_flag_sub(c, value, result);
                c->A = result & 0xFF;
                c->PC += 2;
                DEBUG_PRINT("0xD6 sui\n");
                break;
            }

            // ====== SBB, r ======
            case 0x98: //sbb b
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->B - borrow;
                set_flag_sbb(c, c->B, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x98 sbb b\n");
                break;
            }

            case 0x99: //sbb c
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->C - borrow;
                set_flag_sbb(c, c->C, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x99 sbb c\n");
                break;
            }

            case 0x9B: //sbb e
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->E - borrow;
                set_flag_sbb(c, c->E, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x9B sbb e\n");
                break;
            }

            case 0x9A: //sbb d
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->D - borrow;
                set_flag_sbb(c, c->D, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x9A sbb d\n");
                break;
            }

            case 0x9C: //sbb h
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->H - borrow;
                set_flag_sbb(c, c->H, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x9C sbb h\n");
                break;
            }

            case 0x9D: //sbb l
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->L - borrow;
                set_flag_sbb(c, c->L, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x98 sbb l\n");
                break;
            }

            case 0x9F: //sbb a
            {
                uint8_t borrow = c->f.flags & 1;
                uint16_t result = c->A - c->A - borrow;
                set_flag_sbb(c, c->B, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x9F sbb a\n");
                break;
            }

            case 0x9E: //sbb m
            {
                uint8_t borrow = c->f.flags & 1;
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A - value - borrow;
                set_flag_sbb(c, c->B, result, borrow);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x98 sbb b\n");
                break;
            }

            // ======= SBI =======
            case 0xDE: //sbi d8
            {
                uint8_t borrow = c->f.flags & 1;
                uint8_t value = c->memory[c->PC + 1];
                uint8_t result = c->A - value - borrow;
                set_flag_sbb(c, value, result, borrow);
                c->PC += 2;
                DEBUG_PRINT("0xDE sbi\n");
                break;
            }

            // ======= ANA, r ======
            case 0xA0: //ana b
            {
                uint16_t result = c->A & c->B;
                set_flag_ana(c, c->B, result);
                c->A &= c->B;
                c->PC++;
                DEBUG_PRINT("0xA0 ana b\n");
                break;
            }
            case 0xA1: // ana c
            {
                uint16_t result = c->A & c->C;
                set_flag_ana(c, c->C, result);
                c->A &= c->C;
                c->PC++;
                DEBUG_PRINT("0xA1 ana c\n");
                break;
            }

            case 0xA2: // ana d
            {
                uint16_t result = c->A & c->D;
                set_flag_ana(c, c->D, result);
                c->A &= c->D;
                c->PC++;
                DEBUG_PRINT("0xA2 ana d\n");
                break;
            }

            case 0xA3: // ana e
            {
                uint16_t result = c->A & c->E;
                set_flag_ana(c, c->E, result);
                c->A &= c->E;
                c->PC++;
                DEBUG_PRINT("0xA3 ana e\n");
                break;
            }

            case 0xA4: // ana h
            {
                uint16_t result = c->A & c->H;
                set_flag_ana(c, c->H, result);
                c->A &= c->H;
                c->PC++;
                DEBUG_PRINT("0xA4 ana h\n");
                break;
            }

            case 0xA5: // ana l
            {
                uint16_t result = c->A & c->L;
                set_flag_ana(c, c->L, result);
                c->A &= c->L;
                c->PC++;
                DEBUG_PRINT("0xA5 ana l\n");
                break;
            }

            case 0xA6: // ana m
            {
                uint16_t addr = (c->H << 8) | c->L;
                uint8_t value = c->memory[addr];
                uint16_t result = c->A & value;
                set_flag_ana(c, value, result);
                c->A &= value;
                c->PC++;
                DEBUG_PRINT("0xA6 ana m\n");
                break;
            }

            case 0xA7: // ana a
            {
                uint16_t result = c->A & c->A;
                set_flag_ana(c, c->A, result);
                c->PC++;
                DEBUG_PRINT("0xA7 ana a\n");
                break;
            }

            case 0xE6: //ani d8
            {
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A & value;
                set_flag_ana(c, value, result);
                c->A = result & 0xFF;
                c->PC += 2;
                DEBUG_PRINT("0xE6 ani\n");
                break;
            }

            // ====== XRA, r ======
            case 0xA8: // xra b
            {
                uint8_t value = c->B;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xA8 xra b\n");
                break;
            }

            case 0xA9: // xra c
            {
                uint8_t value = c->C;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xA9 xra c\n");
                break;
            }

            case 0xAA: // xra d
            {
                uint8_t value = c->D;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAA xra d\n");
                break;
            }

            case 0xAB: // xra e
            {
                uint8_t value = c->E;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAB xra e\n");
                break;
            }

            case 0xAC: // xra h
            {
                uint8_t value = c->H;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAC xra h\n");
                break;
            }

            case 0xAD: // xra l
            {
                uint8_t value = c->L;
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAD xra l\n");
                break;
            }

            case 0xAE: // xra m
            {
                uint16_t addr = (c->H << 8) | c->L;
                uint8_t value = c->memory[addr];
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAE xra m\n");
                break;
            }

            case 0xAF: // xra a
            {
                uint16_t result = c->A ^ c->A;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xAF xra a\n");
                break;
            }

            // ====== ORA,r ======
            case 0xB0: // ora, b
            {
                uint16_t result = c->A | c->B;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB0 ora b\n");
                break;
            }

            case 0xB1: // ora c
            {
                uint16_t result = c->A | c->C;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB1 ora c\n");
                break;
            }

            case 0xB2: // ora d
            {
                uint16_t result = c->A | c->D;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB2 ora d\n");
                break;
            }

            case 0xB3: // ora e
            {
                uint16_t result = c->A | c->E;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB3 ora e\n");
                break;
            }

            case 0xB4: // ora h
            {
                uint16_t result = c->A | c->H;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB4 ora h\n");
                break;
            }

            case 0xB5: // ora l
            {
                uint16_t result = c->A | c->L;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB5 ora l\n");
                break;
            }

            case 0xB6: // ora m
            {
                uint16_t addr = (c->H << 8) | c->L;
                uint8_t value = c->memory[addr];
                uint16_t result = c->A | value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB6 ora m\n");
                break;
            }

            case 0xB7: // ora a
            {
                uint16_t result = c->A | c->A;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC++;
                DEBUG_PRINT("0xB7 ora a\n");
                break;
            }

            case 0xF6: // ori d8
            {
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A | value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC += 2;
                DEBUG_PRINT("0xF6 ori d8\n");
                break;
            }

            case 0xEE: // xri d8
            {
                uint8_t value = c->memory[c->PC + 1];
                uint16_t result = c->A ^ value;
                set_flag_xra(c, result);
                c->A = result & 0xFF;
                c->PC += 2;
                DEBUG_PRINT("0xEE xri d8\n");
                break;
            }

            // ====== CMP, r =====
            case 0xB8: // cmp b
            {
                set_flag_cmp(c, c->B);
                DEBUG_PRINT("0xB8 cmp b\n");
                c->PC++;
                break;
            }

            case 0xB9: // cmp c
            {
                set_flag_cmp(c, c->C);
                DEBUG_PRINT("0xB9 cmp c\n");
                c->PC++;
                break;
            }

            case 0xBA: // cmp d
            {
                set_flag_cmp(c, c->D);
                DEBUG_PRINT("0xBA cmp d\n");
                c->PC++;
                break;
            }

            case 0xBB: // cmp e
            {
                set_flag_cmp(c, c->E);
                DEBUG_PRINT("0xBB cmp e\n");
                c->PC++;
                break;
            }

            case 0xBC: // cmp h
            {
                set_flag_cmp(c, c->H);
                DEBUG_PRINT("0xBC cmp h\n");
                c->PC++;
                break;
            }

            case 0xBD: // cmp l
            {
                set_flag_cmp(c, c->L);
                DEBUG_PRINT("0xBD cmp l\n");
                c->PC++;
                break;
            }

            case 0xBE: // cmp m
            {
                uint16_t addr = (c->H << 8) | c->L;
                uint8_t value = c->memory[addr];
                set_flag_cmp(c, value);
                DEBUG_PRINT("0xBE cmp m\n");
                c->PC++;
                break;
            }

            case 0xBF: // cmp a
            {
                set_flag_cmp(c, c->A);
                DEBUG_PRINT("0xBF cmp a\n");
                c->PC++;
                break;
            }

            case 0xFE: // cpi d8
            {
                uint8_t value = c->memory[c->PC + 1];
                set_flag_cmp(c, value);
                DEBUG_PRINT("0xFE cpi d8\n");
                c->PC += 2;
                break;
            }

            // ====== INX, r ======
            case 0x03: //INX B
            {
                uint16_t bc = (c->B << 8) | c->C;
                bc++;
                c->B = (bc >> 8) & 0xFF;
                c->C = bc & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x03 inx b\n");
                break;
            }

            case 0x13: //INX D
            {
                uint16_t de = (c->D << 8) | c->E;
                de++;
                c->D = (de >> 8) & 0xFF;
                c->E = de & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x13 inx d\n");
                break;
            }

            case 0x23: //INX H
            {
                uint16_t hl = (c->H << 8) | c->L;
                hl++;
                c->H = (hl >> 8) & 0xFF;
                c->L = hl & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x23 inx h\n");
                break;
            }

            case 0x33: //INX SP
            {
                c->SP++;
                c->PC++;
                DEBUG_PRINT("0x33 inx sp\n");
                break;
            }

            // ====== DCX, r ======
            case 0x0B: // dcx b
            {
                uint16_t bc = (c->B << 8) | c->C;
                bc--;
                c->B = (bc >> 8) & 0xFF;
                c->C = bc & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x0B dcx b\n");
                break;
            }

            case 0x1B: // dcx d
            {
                uint16_t de = (c->D << 8) | c->E;
                de--;
                c->B = (de >> 8) & 0xFF;
                c->C = de & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x1B dcx d\n");
                break;
            }

            case 0x2B: // dcx h
            {
                uint16_t hl = (c->H << 8) | c->L;
                hl--;
                c->B = (hl >> 8) & 0xFF;
                c->C = hl & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x2B dcx h\n");
                break;
            }

            case 0x3B: // dcx sp
            {
                c->SP--;
                c->PC++;
                DEBUG_PRINT("0x3D dcx sp\n");
                break;
            }

            // ===== DAD, r =====
            case 0x09: // DAD B
            {
                uint16_t bc = (c->B << 8) | c->C;
                uint16_t hl = (c->H << 8) | c->L;
                uint32_t res = hl + bc;

                if (res > 0xFFFF) {
                    c->f.flags |= (1 << 0);
                } else {
                    c->f.flags &= ~(1 << 0);
                }

                c->H = (res >> 8) & 0xFF;
                c->L = res & 0xFF;

                c->PC++;
                DEBUG_PRINT("0x09 dad b\n");
                break;
            }

            case 0x19: // DAD D
            {
                uint16_t de = (c->D << 8) | c->E;
                uint16_t hl = (c->H << 8) | c->L;
                uint32_t res = hl + de;

                if (res > 0xFFFF) {
                    c->f.flags |= (1 << 0);
                } else {
                    c->f.flags &= ~(1 << 0);
                }

                c->H = (res >> 8) & 0xFF;
                c->L = res & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x19 dad d\n");
                break;
            }

            case 0x29: // DAD h
            {
                uint16_t hl = (c->H << 8) | c->L;
                uint32_t res = hl + hl;

                if (res > 0xFFFF) {
                    c->f.flags |= (1 << 0);
                } else {
                    c->f.flags &= ~(1 << 0);
                }

                c->H = (res >> 8) & 0xFF;
                c->L = res & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x29 dad h\n");
                break;
            }

            case 0x39: // DAD SP
            {
                uint16_t hl = (c->H << 8) | c->L;
                uint32_t res = hl + c->SP;

                if (res > 0xFFFF) {
                    c->f.flags |= (1 << 0);
                } else {
                    c->f.flags &= ~(1 << 0);
                }

                c->H = (res >> 8) & 0xFF;
                c->L = res & 0xFF;
                c->PC++;
                DEBUG_PRINT("0x39 dad sp\n");
                break;
            }

            // ====== JUMP, addr ======
            case 0xC3: //JMP, addr
            {
                uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                c->PC = addr;
                DEBUG_PRINT("0xC3 jmp addr\n");
                break;
            }

            case 0xD2: //JNC, addr
            {
                if ((c->f.flags >> 0) == 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xD2 jc addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xD2 jc addr\n");
                    break;
                }
            }

            case 0xE2: //JPO, addr
            {
                if ((c->f.flags & (1 << 3)) != 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xE2 jpo addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xE2 jpo addr\n");
                    break;
                }
            }

            case 0xF2: //jp, adrr
            {
                if ((c->f.flags & (1 << 7)) != 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xF2 jc addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xF2 jc addr\n");
                    break;
                }
            }

            // JNZ (0xC2) - Jump if NOT Zero
            case 0xC2:
            {
                uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                
                if ((c->f.flags & (1 << 6)) == 0) {
                    c->PC = addr;
                    DEBUG_PRINT("0xC2 jnz - jumping to 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                    DEBUG_PRINT("0xC2 jnz - not jumping\n");
                }
                break;
            }

            // JZ (0xCA) - Jump if Zero
            case 0xCA:
            {
                uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                
                if ((c->f.flags & (1 << 6)) != 0) {
                    c->PC = addr;
                    DEBUG_PRINT("0xCA jz - jumping to 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                    DEBUG_PRINT("0xCA jz - not jumping\n");
                }
                break;
            }

            case 0xDA: //jc, addr
            {
                if ((c->f.flags & (1 << 0)) != 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xDA jc addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xDA jc addr\n");
                    break;
                }
            }

            case 0xEA: //jpe, addr
            {
                if ((c->f.flags & (1 << 7)) == 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xEA jpe addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xEA jpe addr\n");
                    break;
                }
            }

            case 0xFA: //jm, addr
            {
                if ((c->f.flags & (1 << 7)) == 0){
                    uint16_t addr = (c->memory[c->PC + 1]) | (c->memory[c->PC + 2] << 8);
                    c->PC = addr;
                    DEBUG_PRINT("0xFA jm addr\n");
                    break;
                }else{
                    c->PC += 3;
                    DEBUG_PRINT("0xFA jm addr\n");
                    break;
                }
            }

            // ======= CALL, addr =======
            case 0xC4: // CALL NZ, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x40) == 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xC4 CALL NZ, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xCC: // CALL Z, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x40) != 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xCC CALL Z, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xD4: // CALL NC, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x01) == 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xD4 CALL NC, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xDC: // CALL C, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x01) != 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xDC CALL C, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xE4: // CALL PO, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x04) == 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xE4 CALL PO, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xEC: // CALL PE, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x04) != 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xEC CALL PE, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xF4: // CALL P, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x02) == 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xF4 CALL P, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            case 0xFC: // CALL M, addr
            {
                uint16_t addr = (c->memory[c->PC + 2] << 8) | (c->memory[c->PC + 1]);

                if ((c->f.flags & 0x02) != 0) {
                    c->memory[--c->SP] = (c->PC + 3) & 0xFF;
                    c->memory[--c->SP] = (c->PC + 3) >> 8;
                    c->PC = addr;
                    DEBUG_PRINT("0xFC CALL M, addr: 0x%04X\n", addr);
                } else {
                    c->PC += 3;
                }
                break;
            }

            // ======= RET, addr =======
            case 0xC0: // RET NZ
            {
                if ((c->f.flags & 0x40) == 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xC0 RET NZ: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xC8: // RET Z
            {
                if ((c->f.flags & 0x40) != 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xC8 RET Z: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xD0: // RET NC
            {
                if ((c->f.flags & 0x01) == 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xD0 RET NC: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xD8: // RET C
            {
                if ((c->f.flags & 0x01) != 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xD8 RET C: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xE0: // RET PO
            {
                if ((c->f.flags & 0x04) == 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xE0 RET PO: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xE8: // RET PE
            {
                if ((c->f.flags & 0x04) != 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xE8 RET PE: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xF0: // RET P
            {
                if ((c->f.flags & 0x02) == 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xF0 RET P: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            case 0xF8: // RET M
            {
                if ((c->f.flags & 0x02) != 0) {
                    uint16_t addr = (c->memory[c->SP + 1] << 8) | c->memory[c->SP + 1];
                    c->PC = addr;
                    DEBUG_PRINT("0xF8 RET M: 0x%04X\n", addr);
                } else {
                    c->PC += 1;
                }
                break;
            }

            // ======= RST, addr =======
            case 0xC7: // RST 0
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0000;  // Jump to address 0x0000
                DEBUG_PRINT("0xC7 RST 0\n");
                break;
            }

            case 0xCF: // RST 1
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0008;  // Jump to address 0x0008
                DEBUG_PRINT("0xCF RST 1\n");
                break;
            }

            case 0xD7: // RST 2
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0010;  // Jump to address 0x0010
                DEBUG_PRINT("0xD7 RST 2\n");
                break;
            }

            case 0xDF: // RST 3
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0018;  // Jump to address 0x0018
                DEBUG_PRINT("0xDF RST 3\n");
                break;
            }

            case 0xE7: // RST 4
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0020;  // Jump to address 0x0020
                DEBUG_PRINT("0xE7 RST 4\n");
                break;
            }

            case 0xEF: // RST 5
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0028;  // Jump to address 0x0028
                DEBUG_PRINT("0xEF RST 5\n");
                break;
            }

            case 0xF7: // RST 6
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0030;  // Jump to address 0x0030
                DEBUG_PRINT("0xF7 RST 6\n");
                break;
            }

            case 0xFF: // RST 7
            {
                c->memory[--c->SP] = (c->PC + 1) & 0xFF;
                c->memory[--c->SP] = (c->PC + 1) >> 8;
                c->PC = 0x0038;  // Jump to address 0x0038
                DEBUG_PRINT("0xFF RST 7\n");
                break;
            }

            case 0xE9: // PCHL
            {
                // Set the PC to the value of the HL register pair
                c->PC = (c->H << 8) | c->L;
                DEBUG_PRINT("0xE9 PCHL: Jump to 0x%04X\n", c->PC);
                break;
            }

            // ===== INR (Increment Register) =====

            case 0x04: // INR B
                c->B++;
                // Set flags
                c->f.flags = (c->B == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));  // Z
                c->f.flags = (c->B & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));  // S
                c->f.flags = ((c->B & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));  // AC
                // Parity
                {
                    uint8_t p = c->B;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x04 inr b\n");
                break;

            case 0x0C: // INR C
                c->C++;
                c->f.flags = (c->C == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->C & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->C & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->C;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x0C inr c\n");
                break;

            case 0x14: // INR D
                c->D++;
                c->f.flags = (c->D == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->D & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->D & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->D;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x14 inr d\n");
                break;

            case 0x1C: // INR E
                c->E++;
                c->f.flags = (c->E == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->E & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->E & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->E;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x1C inr e\n");
                break;

            case 0x24: // INR H
                c->H++;
                c->f.flags = (c->H == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->H & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->H & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->H;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x24 inr h\n");
                break;

            case 0x2C: // INR L
                c->L++;
                c->f.flags = (c->L == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->L & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->L & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->L;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x2C inr l\n");
                break;

            case 0x3C: // INR A
                c->A++;
                c->f.flags = (c->A == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->A & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->A & 0x0F) == 0) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->A;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x3C inr a\n");
                break;

            // ===== DCR (Decrement Register) =====

            case 0x05: // DCR B
                c->B--;
                // Set flags  
                c->f.flags = (c->B == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));  // Z
                c->f.flags = (c->B & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));  // S
                c->f.flags = ((c->B & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));  // AC
                // Parity
                {
                    uint8_t p = c->B;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x05 dcr b\n");
                break;

            case 0x0D: // DCR C
                c->C--;
                c->f.flags = (c->C == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->C & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->C & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->C;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x0D dcr c\n");
                break;

            case 0x15: // DCR D
                c->D--;
                c->f.flags = (c->D == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->D & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->D & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->D;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x15 dcr d\n");
                break;

            case 0x1D: // DCR E
                c->E--;
                c->f.flags = (c->E == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->E & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->E & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->E;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x1D dcr e\n");
                break;

            case 0x25: // DCR H
                c->H--;
                c->f.flags = (c->H == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->H & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->H & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->H;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x25 dcr h\n");
                break;

            case 0x2D: // DCR L
                c->L--;
                c->f.flags = (c->L == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->L & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->L & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->L;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x2D dcr l\n");
                break;

            case 0x3D: // DCR A
                c->A--;
                c->f.flags = (c->A == 0) ? (c->f.flags | (1 << 6)) : (c->f.flags & ~(1 << 6));
                c->f.flags = (c->A & 0x80) ? (c->f.flags | (1 << 7)) : (c->f.flags & ~(1 << 7));
                c->f.flags = ((c->A & 0x0F) == 0x0F) ? (c->f.flags | (1 << 4)) : (c->f.flags & ~(1 << 4));
                {
                    uint8_t p = c->A;
                    uint8_t count = 0;
                    for (int i = 0; i < 8; i++) {
                        if (p & 1) count++;
                        p >>= 1;
                    }
                    c->f.flags = (count % 2 == 0) ? (c->f.flags | (1 << 2)) : (c->f.flags & ~(1 << 2));
                }
                c->PC++;
                DEBUG_PRINT("0x3D dcr a\n");
                break;

            default: c->PC++; break;
        }
        c->cycles += CYCLES_TABLE[c->memory[c->PC]];
    }
    system("clear");
    printf("\n");
    return 0x01;
}
