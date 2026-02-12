#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void compile(char *path_in, char *path_out)
{
    FILE *fhexptr;
    FILE *fasmptr;
    fasmptr = fopen(path_in, "r");
    fhexptr = fopen(path_out, "w");

    if (fasmptr == NULL || fhexptr == NULL) {
        perror("File could not be opened\n");
        return;
    }

    char line[256];
    int bytes_written = 0;

    while (fgets(line, sizeof(line), fasmptr)) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0 || line[0] == ';' || line[0] == '#') {
            continue;
        }

        char opcode[32];
        char operand1[32] = {0};
        char operand2[32] = {0};

        sscanf(line, "%s %s %s", opcode, operand1, operand2);

        for (int i = 0; opcode[i]; i++) {
            opcode[i] = toupper(opcode[i]);
        }

        // NOP
        if (strcmp(opcode, "NOP") == 0) {
            fprintf(fhexptr, "00 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "HLT") == 0) {
            fprintf(fhexptr, "76 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "EI") == 0) {
            fprintf(fhexptr, "FB ");
            bytes_written++;
        }
        else if (strcmp(opcode, "DI") == 0) {
            fprintf(fhexptr, "F3 ");
            bytes_written++;
        }

        // Rotation
        else if (strcmp(opcode, "RLC") == 0) {
            fprintf(fhexptr, "07 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RRC") == 0) {
            fprintf(fhexptr, "0F ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RAL") == 0) {
            fprintf(fhexptr, "17 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RAR") == 0) {
            fprintf(fhexptr, "1F ");
            bytes_written++;
        }

        // Carry et complément
        else if (strcmp(opcode, "CMC") == 0) {
            fprintf(fhexptr, "3F ");
            bytes_written++;
        }
        else if (strcmp(opcode, "STC") == 0) {
            fprintf(fhexptr, "37 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "CMA") == 0) {
            fprintf(fhexptr, "2F ");
            bytes_written++;
        }
        else if (strcmp(opcode, "DAA") == 0) {
            fprintf(fhexptr, "27 ");
            bytes_written++;
        }

        // Transfert HL et SP
        else if (strcmp(opcode, "XCHG") == 0) {
            fprintf(fhexptr, "EB ");
            bytes_written++;
        }
        else if (strcmp(opcode, "XTHL") == 0) {
            fprintf(fhexptr, "E3 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "SPHL") == 0) {
            fprintf(fhexptr, "F9 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "PCHL") == 0) {
            fprintf(fhexptr, "E9 ");
            bytes_written++;
        }

        // Retour
        else if (strcmp(opcode, "RET") == 0) {
            fprintf(fhexptr, "C9 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RZ") == 0) {
            fprintf(fhexptr, "C8 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RNZ") == 0) {
            fprintf(fhexptr, "C0 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RC") == 0) {
            fprintf(fhexptr, "D8 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RNC") == 0) {
            fprintf(fhexptr, "D0 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RP") == 0) {
            fprintf(fhexptr, "F0 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RM") == 0) {
            fprintf(fhexptr, "F8 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RPE") == 0) {
            fprintf(fhexptr, "E8 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "RPO") == 0) {
            fprintf(fhexptr, "E0 ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // MOV
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "MOV") == 0) {
            operand1[strcspn(operand1, ",")] = 0;
            operand2[strcspn(operand2, ",")] = 0;

            if (strcmp(operand1, "A") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "7F ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "78 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "79 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "7A ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "7B ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "7C ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "7D ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "7E ");
                bytes_written++;
            }
            else if (strcmp(operand1, "B") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "47 ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "40 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "41 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "42 ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "43 ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "44 ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "45 ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "46 ");
                bytes_written++;
            }
            else if (strcmp(operand1, "C") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "4F ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "48 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "49 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "4A ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "4B ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "4C ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "4D ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "4E ");
                bytes_written++;
            }
            else if (strcmp(operand1, "D") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "57 ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "50 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "51 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "52 ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "53 ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "54 ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "55 ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "56 ");
                bytes_written++;
            }
            else if (strcmp(operand1, "E") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "5F ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "58 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "59 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "5A ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "5B ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "5C ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "5D ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "5E ");
                bytes_written++;
            }
            else if (strcmp(operand1, "H") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "67 ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "60 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "61 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "62 ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "63 ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "64 ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "65 ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "66 ");
                bytes_written++;
            }
            else if (strcmp(operand1, "L") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "6F ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "68 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "69 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "6A ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "6B ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "6C ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "6D ");
                else if (strcmp(operand2, "M") == 0) fprintf(fhexptr, "6E ");
                bytes_written++;
            }
            else if (strcmp(operand1, "M") == 0) {
                if (strcmp(operand2, "A") == 0) fprintf(fhexptr, "77 ");
                else if (strcmp(operand2, "B") == 0) fprintf(fhexptr, "70 ");
                else if (strcmp(operand2, "C") == 0) fprintf(fhexptr, "71 ");
                else if (strcmp(operand2, "D") == 0) fprintf(fhexptr, "72 ");
                else if (strcmp(operand2, "E") == 0) fprintf(fhexptr, "73 ");
                else if (strcmp(operand2, "H") == 0) fprintf(fhexptr, "74 ");
                else if (strcmp(operand2, "L") == 0) fprintf(fhexptr, "75 ");
                bytes_written++;
            }
        }

        // ═══════════════════════════════════════════════════════════
        // MVI
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "MVI") == 0) {
            operand1[strcspn(operand1, ",")] = 0;
            int value = (int)strtol(operand2, NULL, 0);

            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "3E %02X ", value);
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "06 %02X ", value);
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "0E %02X ", value);
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "16 %02X ", value);
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "1E %02X ", value);
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "26 %02X ", value);
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "2E %02X ", value);
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "36 %02X ", value);
            bytes_written += 2;
        }

        // ═══════════════════════════════════════════════════════════
        // LXI
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "LXI") == 0) {
            operand1[strcspn(operand1, ",")] = 0;
            int value = (int)strtol(operand2, NULL, 0);
            uint8_t low = value & 0xFF;
            uint8_t high = (value >> 8) & 0xFF;

            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "01 %02X %02X ", low, high);
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "11 %02X %02X ", low, high);
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "21 %02X %02X ", low, high);
            else if (strcmp(operand1, "SP") == 0) fprintf(fhexptr, "31 %02X %02X ", low, high);
            bytes_written += 3;
        }

        // ═══════════════════════════════════════════════════════════
        // ADD, ADC, SUB, SBB, ANA, XRA, ORA, CMP
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "ADD") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "87 ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "80 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "81 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "82 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "83 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "84 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "85 ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "86 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "ADC") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "8F ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "88 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "89 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "8A ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "8B ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "8C ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "8D ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "8E ");
            bytes_written++;
        }
        else if (strcmp(opcode, "SUB") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "97 ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "90 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "91 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "92 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "93 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "94 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "95 ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "96 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "SBB") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "9F ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "98 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "99 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "9A ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "9B ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "9C ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "9D ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "9E ");
            bytes_written++;
        }
        else if (strcmp(opcode, "ANA") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "A7 ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "A0 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "A1 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "A2 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "A3 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "A4 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "A5 ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "A6 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "XRA") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "AF ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "A8 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "A9 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "AA ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "AB ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "AC ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "AD ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "AE ");
            bytes_written++;
        }
        else if (strcmp(opcode, "ORA") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "B7 ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "B0 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "B1 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "B2 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "B3 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "B4 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "B5 ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "B6 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "CMP") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "BF ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "B8 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "B9 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "BA ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "BB ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "BC ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "BD ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "BE ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // Operations
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "ADI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "C6 %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "ACI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "CE %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "SUI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "D6 %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "SBI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "DE %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "ANI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "E6 %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "XRI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "EE %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "ORI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "F6 %02X ", value);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "CPI") == 0) {
            int value = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "FE %02X ", value);
            bytes_written += 2;
        }

        // ═══════════════════════════════════════════════════════════
        // INR & DCR
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "INR") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "3C ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "04 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "0C ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "14 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "1C ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "24 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "2C ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "34 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "DCR") == 0) {
            if (strcmp(operand1, "A") == 0) fprintf(fhexptr, "3D ");
            else if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "05 ");
            else if (strcmp(operand1, "C") == 0) fprintf(fhexptr, "0D ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "15 ");
            else if (strcmp(operand1, "E") == 0) fprintf(fhexptr, "1D ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "25 ");
            else if (strcmp(operand1, "L") == 0) fprintf(fhexptr, "2D ");
            else if (strcmp(operand1, "M") == 0) fprintf(fhexptr, "35 ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // INX et DCX (16-bit)
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "INX") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "03 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "13 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "23 ");
            else if (strcmp(operand1, "SP") == 0) fprintf(fhexptr, "33 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "DCX") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "0B ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "1B ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "2B ");
            else if (strcmp(operand1, "SP") == 0) fprintf(fhexptr, "3B ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // DAD (double add)
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "DAD") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "09 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "19 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "29 ");
            else if (strcmp(operand1, "SP") == 0) fprintf(fhexptr, "39 ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // PUSH & POP
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "PUSH") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "C5 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "D5 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "E5 ");
            else if (strcmp(operand1, "PSW") == 0) fprintf(fhexptr, "F5 ");
            bytes_written++;
        }
        else if (strcmp(opcode, "POP") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "C1 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "D1 ");
            else if (strcmp(operand1, "H") == 0) fprintf(fhexptr, "E1 ");
            else if (strcmp(operand1, "PSW") == 0) fprintf(fhexptr, "F1 ");
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // JUMPS
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "JMP") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "C3 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JZ") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "CA %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JNZ") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "C2 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JC") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "DA %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JNC") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "D2 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JP") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "F2 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JM") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "FA %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JPE") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "EA %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "JPO") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "E2 %02X %02X ", low, high);
            bytes_written += 3;
        }

        // ═══════════════════════════════════════════════════════════
        // CALL
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "CALL") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "CD %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CZ") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "CC %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CNZ") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "C4 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CC") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "DC %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CNC") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "D4 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CP") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "F4 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CM") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "FC %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CPE") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "EC %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "CPO") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "E4 %02X %02X ", low, high);
            bytes_written += 3;
        }

        // ═══════════════════════════════════════════════════════════
        // RST
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "RST") == 0) {
            int n = atoi(operand1);
            fprintf(fhexptr, "%02X ", 0xC7 + (n * 8));
            bytes_written++;
        }

        // ═══════════════════════════════════════════════════════════
        // I/O
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "IN") == 0) {
            int port = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "DB %02X ", port);
            bytes_written += 2;
        }
        else if (strcmp(opcode, "OUT") == 0) {
            int port = (int)strtol(operand1, NULL, 0);
            fprintf(fhexptr, "D3 %02X ", port);
            bytes_written += 2;
        }

        // ═══════════════════════════════════════════════════════════
        // MEMORY
        // ═══════════════════════════════════════════════════════════

        else if (strcmp(opcode, "LDA") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "3A %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "STA") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "32 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "LHLD") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "2A %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "SHLD") == 0) {
            int addr = (int)strtol(operand1, NULL, 0);
            uint8_t low = addr & 0xFF;
            uint8_t high = (addr >> 8) & 0xFF;
            fprintf(fhexptr, "22 %02X %02X ", low, high);
            bytes_written += 3;
        }
        else if (strcmp(opcode, "LDAX") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "0A ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "1A ");
            bytes_written++;
        }
        else if (strcmp(opcode, "STAX") == 0) {
            if (strcmp(operand1, "B") == 0) fprintf(fhexptr, "02 ");
            else if (strcmp(operand1, "D") == 0) fprintf(fhexptr, "12 ");
            bytes_written++;
        }

        if (bytes_written % 16 == 0) {
            fprintf(fhexptr, "\n");
        }
    }

    fprintf(fhexptr, "\n");

    fclose(fhexptr);
    fclose(fasmptr);

    printf("✓ Compilation completed !\n");
    printf("  File : %s\n", path_out);
    printf("  Bytes : %d\n", bytes_written);
}
