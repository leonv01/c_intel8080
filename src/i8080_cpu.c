//
// Created by leonv on 4/19/2024.
//

#include "i8080_cpu.h"

#include <stdio.h>

i8080_t* init_i8080(void){
    i8080_t *cpu = (i8080_t*)malloc(sizeof(i8080_t));
    if(cpu != NULL){
        cpu->A = 0;
        cpu->B = 0;
        cpu->C = 0;
        cpu->D = 0;
        cpu->E = 0;
        cpu->H = 0;
        cpu->L = 0;
        cpu->SP = 0;
        cpu->PC = 0x100;
        cpu->flags.z = 0;
        cpu->flags.s = 0;
        cpu->flags.p = 0;
        cpu->flags.cy = 0;
        cpu->flags.ac = 0;
        cpu->flags.pad = 0;

        cpu->memory = (uint8_t*)malloc(0x10000);

        printf("CPU initialized\n");
    }
    else {
        cpu = NULL;
    }
    return cpu;
}

void destroy_i8080(i8080_t *cpu){
    if(cpu != NULL){
        free(cpu->memory);
        free(cpu);
        printf("CPU destroyed\n");
    }
}

uint8_t *read_memory(i8080_t *cpu, uint16_t address){
    uint8_t* value = NULL;

    if(cpu != NULL){
        value = &cpu->memory[address];
    }

    return value;
}

void write_memory(i8080_t *cpu, uint16_t address, uint8_t value){
    if(cpu != NULL){
        cpu->memory[address] = value;
    }
}

void print_state(i8080_t *cpu){
    if(cpu != NULL){
        printf("A: 0x%02x\n", cpu->A);
        printf("B: 0x%02x\n", cpu->B);
        printf("C: 0x%02x\n", cpu->C);
        printf("D: 0x%02x\n", cpu->D);
        printf("E: 0x%02x\n", cpu->E);
        printf("H: 0x%02x\n", cpu->H);
        printf("L: 0x%02x\n", cpu->L);
        printf("SP: 0x%04x\n", cpu->SP);
        printf("PC: 0x%04x\n", cpu->PC);
        printf("Flags: Z:%d S:%d P:%d CY:%d AC:%d\n", cpu->flags.z, cpu->flags.s, cpu->flags.p, cpu->flags.cy, cpu->flags.ac);
    }
}

static void set_flags(i8080_t *cpu, uint16_t result);

uint8_t emulate_cycle(i8080_t *cpu){
    uint8_t cycles = 0;
    if(cpu != NULL){
        uint8_t* opcode = read_memory(cpu, cpu->PC);
        uint16_t word;
        bool condition = false;
        uint8_t reg, msb, lsb;
        uint8_t *reg1, *reg2;
        if(opcode != NULL){
            switch(*opcode){

                // ADI instruction
                case 0xC6: // ADI
                    ADD(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // ADD instructions
                case 0x80: // ADD B
                case 0x81: // ADD C
                case 0x82: // ADD D
                case 0x83: // ADD E
                case 0x84: // ADD H
                case 0x85: // ADD L
                case 0x87: // ADD A
                    switch(*opcode){
                        case 0x80: // ADD B
                            reg = cpu->B;
                            break;
                        case 0x81: // ADD C
                            reg = cpu->C;
                            break;
                        case 0x82: // ADD D
                            reg = cpu->D;
                            break;
                        case 0x83: // ADD E
                            reg = cpu->E;
                            break;
                        case 0x84: // ADD H
                            reg = cpu->H;
                            break;
                        case 0x85: // ADD L
                            reg = cpu->L;
                            break;
                        case 0x87: // ADD A
                            reg = cpu->A;
                            break;
                    }
                    ADD(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x86: // ADD M
                    ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // ADC instructions
                case 0x88: // ADC B
                case 0x89: // ADC C
                case 0x8A: // ADC D
                case 0x8B: // ADC E
                case 0x8C: // ADC H
                case 0x8D: // ADC L
                case 0x8F: // ADC A
                    reg = cpu->flags.cy;
                    switch(*opcode){
                        case 0x88: // ADC B
                            reg += cpu->B;
                        case 0x89: // ADC C
                            reg += cpu->C;
                        case 0x8A: // ADC D
                            reg += cpu->D;
                        case 0x8B: // ADC E
                            reg += cpu->E;
                        case 0x8C: // ADC H
                            reg += cpu->H;
                        case 0x8D: // ADC L
                            reg += cpu->L;
                        case 0x8F: // ADC A
                            reg += cpu->A;
                    }
                    ADD(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8E: // ADC M
                    ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 7;
                    break;

                // SUB instructions
                case 0x90: // SUB B
                case 0x91: // SUB C
                case 0x92: // SUB D
                case 0x93: // SUB E
                case 0x94: // SUB H
                case 0x95: // SUB L
                case 0x97: // SUB A
                    switch (*opcode) {
                        case 0x90: // SUB B
                            reg = cpu->B;
                            break;
                        case 0x91: // SUB C
                            reg = cpu->C;
                            break;
                        case 0x92: // SUB D
                            reg = cpu->D;
                            break;
                        case 0x93: // SUB E
                            reg = cpu->E;
                            break;
                        case 0x94: // SUB H
                            reg = cpu->H;
                            break;
                        case 0x95: // SUB L
                            reg = cpu->L;
                            break;
                        case 0x97: // SUB A
                            reg = cpu->A;
                            break;
                    }
                    SUB(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x96: // SUB M
                    SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                case 0x98: // SBB B
                case 0x99: // SBB C
                case 0x9A: // SBB D
                case 0x9B: // SBB E
                case 0x9C: // SBB H
                case 0x9D: // SBB L
                case 0x9F: // SBB A
                    switch (*opcode) {
                        case 0x98: // SBB B
                            reg = cpu->B;
                            break;
                        case 0x99: // SBB C
                            reg = cpu->C;
                            break;
                        case 0x9A: // SBB D
                            reg = cpu->D;
                            break;
                        case 0x9B: // SBB E
                            reg = cpu->E;
                            break;
                        case 0x9C: // SBB H
                            reg = cpu->H;
                            break;
                        case 0x9D: // SBB L
                            reg = cpu->L;
                            break;
                        case 0x9F: // SBB A
                            reg = cpu->A;
                            break;
                    }
                    reg -= cpu->flags.cy;
                    SUB(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9E: // SBB M
                    SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 7;
                    break;
                // SUI instruction
                case 0xD6: // SUI
                    SUB(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // ANA Instructions
                case 0xA0: // ANA B
                case 0xA1: // ANA C
                case 0xA2: // ANA D
                case 0xA3: // ANA E
                case 0xA4: // ANA H
                case 0xA5: // ANA L
                case 0xA7: // ANA A
                    switch (*opcode) {
                        case 0xA0: // ANA B
                            reg = cpu->B;
                            break;
                        case 0xA1: // ANA C
                            reg = cpu->C;
                            break;
                        case 0xA2: // ANA D
                            reg = cpu->D;
                            break;
                        case 0xA3: // ANA E
                            reg = cpu->E;
                            break;
                        case 0xA4: // ANA H
                            reg = cpu->H;
                            break;
                        case 0xA5: // ANA L
                            reg = cpu->L;
                            break;
                        case 0xA7: // ANA A
                            reg = cpu->A;
                            break;
                    }
                    ANA(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA6: // ANA M
                    ANA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // ANI Instruction
                case 0xE6: // ANI
                    ANA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // ORA Instructions
                case 0xB0: // ORA B
                case 0xB1: // ORA C
                case 0xB2: // ORA D
                case 0xB3: // ORA E
                case 0xB4: // ORA H
                case 0xB5: // ORA L
                case 0xB7: // ORA A
                    switch (*opcode) {
                        case 0xB0: // ORA B
                            reg = cpu->B;
                            break;
                        case 0xB1: // ORA C
                            reg = cpu->C;
                            break;
                        case 0xB2: // ORA D
                            reg = cpu->D;
                            break;
                        case 0xB3: // ORA E
                            reg = cpu->E;
                            break;
                        case 0xB4: // ORA H
                            reg = cpu->H;
                            break;
                        case 0xB5: // ORA L
                            reg = cpu->L;
                            break;
                        case 0xB7: // ORA A
                            reg = cpu->A;
                            break;
                    }
                    ORA(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB6: // ORA M
                    ORA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // ORI Instruction
                case 0xF6: // ORI
                    ORA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // XRA Instructions
                case 0xA8: // XRA B
                case 0xA9: // XRA C
                case 0xAA: // XRA D
                case 0xAB: // XRA E
                case 0xAC: // XRA H
                case 0xAD: // XRA L
                case 0xAF: // XRA A
                    switch(*opcode){
                        case 0xA8: // XRA B
                            reg = cpu->B;
                            break;
                        case 0xA9: // XRA C
                            reg = cpu->C;
                            break;
                        case 0xAA: // XRA D
                            reg = cpu->D;
                            break;
                        case 0xAB: // XRA E
                            reg = cpu->E;
                            break;
                        case 0xAC: // XRA H
                            reg = cpu->H;
                            break;
                        case 0xAD: // XRA L
                            reg = cpu->L;
                            break;
                        case 0xAF: // XRA A
                            reg = cpu->A;
                            break;
                    }
                    XRA(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAE: // XRA M
                    XRA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // XRI Instruction
                case 0xEE: // XRI
                    XRA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // CMP Instructions
                case 0xB8: // CMP B
                case 0xB9: // CMP C
                case 0xBA: // CMP D
                case 0xBB: // CMP E
                case 0xBC: // CMP H
                case 0xBD: // CMP L
                case 0xBF: // CMP A
                    switch(*opcode){
                        case 0xB8: // CMP B
                            reg = cpu->B;
                            break;
                        case 0xB9: // CMP C
                            reg = cpu->C;
                            break;
                        case 0xBA: // CMP D
                            reg = cpu->D;
                            break;
                        case 0xBB: // CMP E
                            reg = cpu->E;
                            break;
                        case 0xBC: // CMP H
                            reg = cpu->H;
                            break;
                        case 0xBD: // CMP L
                            reg = cpu->L;
                            break;
                        case 0xBF: // CMP A
                            reg = cpu->A;
                            break;
                    }
                    CMP(cpu, reg);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBE: // CMP M
                    CMP(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // CPI Instruction
                case 0xFE: // CPI
                    CMP(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                // INR Instructions
                case 0x04: // INR B
                case 0x0C: // INR C
                case 0x14: // INR D
                case 0x1C: // INR E
                case 0x24: // INR H
                case 0x2C: // INR L
                case 0x3C: // INR A
                    switch(*opcode){
                        case 0x04: // INR B
                            INR(cpu, &cpu->B);
                            break;
                        case 0x0C: // INR C
                            INR(cpu, &cpu->C);
                            break;
                        case 0x14: // INR D
                            INR(cpu, &cpu->D);
                            break;
                        case 0x1C: // INR E
                            INR(cpu, &cpu->E);
                            break;
                        case 0x24: // INR H
                            INR(cpu, &cpu->H);
                            break;
                        case 0x2C: // INR L
                            INR(cpu, &cpu->L);
                            break;
                        case 0x3C: // INR A
                            INR(cpu, &cpu->A);
                            break;
                    }
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x34: // INR M
                    INR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 10;
                    break;

                // INX Instructions
                case 0x03: // INX B
                    INX(cpu, &cpu->B, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x13: // INX D
                    INX(cpu, &cpu->D, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x23: // INX H
                    INX(cpu, &cpu->H, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x33: // INX SP
                    cpu->SP++;
                    cpu->PC++;
                    cycles = 5;
                    break;

                // DCR Instructions
                case 0x05: // DCR B
                case 0x0D: // DCR C
                case 0x15: // DCR D
                case 0x1D: // DCR E
                case 0x25: // DCR H
                case 0x2D: // DCR L
                case 0x3D: // DCR A
                    switch(*opcode){
                        case 0x05: // DCR B
                            DCR(cpu, &cpu->B);
                            break;
                        case 0x0D: // DCR C
                            DCR(cpu, &cpu->C);
                            break;
                        case 0x15: // DCR D
                            DCR(cpu, &cpu->D);
                            break;
                        case 0x1D: // DCR E
                            DCR(cpu, &cpu->E);
                            break;
                        case 0x25: // DCR H
                            DCR(cpu, &cpu->H);
                            break;
                        case 0x2D: // DCR L
                            DCR(cpu, &cpu->L);
                            break;
                        case 0x3D: // DCR A
                            DCR(cpu, &cpu->A);
                            break;
                    }
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x35: // DCR M
                    DCR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 10;
                    break;

                // DCX Instructions
                case 0x0B: // DCX B
                    DCX(cpu, &cpu->B, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x1B: // DCX D
                    DCX(cpu, &cpu->D, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x2B: // DCX H
                    DCX(cpu, &cpu->H, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x3B: // DCX SP
                    cpu->SP--;
                    cpu->PC++;
                    cycles = 5;
                    break;

                // DAD Instructions
                case 0x09: // DAD B
                    DAD(cpu, cpu->B, cpu->C);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0x19: // DAD D
                    DAD(cpu, cpu->D, cpu->E);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0x29: // DAD H
                    DAD(cpu, cpu->H, cpu->L);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0x39: // DAD SP
                    DAD(cpu, HIGH_BYTE(cpu->SP), LOW_BYTE(cpu->SP));
                    cpu->PC++;
                    cycles = 10;
                    break;

                // MOV Instructions
                case 0x40: // MOV B, B
                case 0x41: // MOV B, C
                case 0x42: // MOV B, D
                case 0x43: // MOV B, E
                case 0x44: // MOV B, H
                case 0x45: // MOV B, L
                case 0x47: // MOV B, A
                case 0x48: // MOV C, B
                case 0x49: // MOV C, C
                case 0x4A: // MOV C, D
                case 0x4B: // MOV C, E
                case 0x4C: // MOV C, H
                case 0x4D: // MOV C, L
                case 0x4F: // MOV C, A
                case 0x50: // MOV D, B
                case 0x51: // MOV D, C
                case 0x52: // MOV D, D
                case 0x53: // MOV D, E
                case 0x54: // MOV D, H
                case 0x55: // MOV D, L
                case 0x57: // MOV D, A
                case 0x58: // MOV E, B
                case 0x59: // MOV E, C
                case 0x5A: // MOV E, D
                case 0x5B: // MOV E, E
                case 0x5C: // MOV E, H
                case 0x5D: // MOV E, L
                case 0x5F: // MOV E, A
                case 0x60: // MOV H, B
                case 0x61: // MOV H, C
                case 0x62: // MOV H, D
                case 0x63: // MOV H, E
                case 0x64: // MOV H, H
                case 0x65: // MOV H, L
                case 0x67: // MOV H, A
                case 0x68: // MOV L, B
                case 0x69: // MOV L, C
                case 0x6A: // MOV L, D
                case 0x6B: // MOV L, E
                case 0x6C: // MOV L, H
                case 0x6D: // MOV L, L
                case 0x6F: // MOV L, A
                case 0x78: // MOV A, B
                case 0x79: // MOV A, C
                case 0x7A: // MOV A, D
                case 0x7B: // MOV A, E
                case 0x7C: // MOV A, H
                case 0x7D: // MOV A, L
                case 0x7F: // MOV A, A
                    switch (*opcode) {
                        case 0x40: // MOV B, B
                            reg1 = &cpu->B;
                            reg2 = &cpu->B;
                            break;
                        case 0x41: // MOV B, C
                            reg1 = &cpu->B;
                            reg2 = &cpu->C;
                            break;
                        case 0x42: // MOV B, D
                            reg1 = &cpu->B;
                            reg2 = &cpu->D;
                            break;
                        case 0x43: // MOV B, E
                            reg1 = &cpu->B;
                            reg2 = &cpu->E;
                            break;
                        case 0x44: // MOV B, H
                            reg1 = &cpu->B;
                            reg2 = &cpu->H;
                            break;
                        case 0x45: // MOV B, L
                            reg1 = &cpu->B;
                            reg2 = &cpu->L;
                            break;
                        case 0x47: // MOV B, A
                            reg1 = &cpu->B;
                            reg2 = &cpu->A;
                            break;
                        case 0x48: // MOV C, B
                            reg1 = &cpu->C;
                            reg2 = &cpu->B;
                            break;
                        case 0x49: // MOV C, C
                            reg1 = &cpu->C;
                            reg2 = &cpu->C;
                            break;
                        case 0x4A: // MOV C, D
                            reg1 = &cpu->C;
                            reg2 = &cpu->D;
                            break;
                        case 0x4B: // MOV C, E
                            reg1 = &cpu->C;
                            reg2 = &cpu->E;
                            break;
                        case 0x4C: // MOV C, H
                            reg1 = &cpu->C;
                            reg2 = &cpu->H;
                            break;
                        case 0x4D: // MOV C, L
                            reg1 = &cpu->C;
                            reg2 = &cpu->L;
                            break;
                        case 0x4F: // MOV C, A
                            reg1 = &cpu->C;
                            reg2 = &cpu->A;
                            break;
                        case 0x50: // MOV D, B
                            reg1 = &cpu->D;
                            reg2 = &cpu->B;
                            break;
                        case 0x51: // MOV D, C
                            reg1 = &cpu->D;
                            reg2 = &cpu->C;
                            break;
                        case 0x52: // MOV D, D
                            reg1 = &cpu->D;
                            reg2 = &cpu->D;
                            break;
                        case 0x53: // MOV D, E
                            reg1 = &cpu->D;
                            reg2 = &cpu->E;
                            break;
                        case 0x54: // MOV D, H
                            reg1 = &cpu->D;
                            reg2 = &cpu->H;
                            break;
                        case 0x55: // MOV D, L
                            reg1 = &cpu->D;
                            reg2 = &cpu->L;
                            break;
                        case 0x57: // MOV D, A
                            reg1 = &cpu->D;
                            reg2 = &cpu->A;
                            break;
                        case 0x58: // MOV E, B
                            reg1 = &cpu->E;
                            reg2 = &cpu->B;
                            break;
                        case 0x59: // MOV E, C
                            reg1 = &cpu->E;
                            reg2 = &cpu->C;
                            break;
                        case 0x5A: // MOV E, D
                            reg1 = &cpu->E;
                            reg2 = &cpu->D;
                            break;
                        case 0x5B: // MOV E, E
                            reg1 = &cpu->E;
                            reg2 = &cpu->E;
                            break;
                        case 0x5C: // MOV E, H
                            reg1 = &cpu->E;
                            reg2 = &cpu->H;
                            break;
                        case 0x5D: // MOV E, L
                            reg1 = &cpu->E;
                            reg2 = &cpu->L;
                            break;
                        case 0x5F: // MOV E, A
                            reg1 = &cpu->E;
                            reg2 = &cpu->A;
                            break;
                        case 0x60: // MOV H, B
                            reg1 = &cpu->H;
                            reg2 = &cpu->B;
                            break;
                        case 0x61: // MOV H, C
                            reg1 = &cpu->H;
                            reg2 = &cpu->C;
                            break;
                        case 0x62: // MOV H, D
                            reg1 = &cpu->H;
                            reg2 = &cpu->D;
                            break;
                        case 0x63: // MOV H, E
                            reg1 = &cpu->H;
                            reg2 = &cpu->E;
                            break;
                        case 0x64: // MOV H, H
                            reg1 = &cpu->H;
                            reg2 = &cpu->H;
                            break;
                        case 0x65: // MOV H, L
                            reg1 = &cpu->H;
                            reg2 = &cpu->L;
                            break;
                        case 0x67: // MOV H, A
                            reg1 = &cpu->H;
                            reg2 = &cpu->A;
                            break;
                        case 0x68: // MOV L, B
                            reg1 = &cpu->L;
                            reg2 = &cpu->B;
                            break;
                        case 0x69: // MOV L, C
                            reg1 = &cpu->L;
                            reg2 = &cpu->C;
                            break;
                        case 0x6A: // MOV L, D
                            reg1 = &cpu->L;
                            reg2 = &cpu->D;
                            break;
                        case 0x6B: // MOV L, E
                            reg1 = &cpu->L;
                            reg2 = &cpu->E;
                            break;
                        case 0x6C: // MOV L, H
                            reg1 = &cpu->L;
                            reg2 = &cpu->H;
                            break;
                        case 0x6D: // MOV L, L
                            reg1 = &cpu->L;
                            reg2 = &cpu->L;
                            break;
                        case 0x6F: // MOV L, A
                            reg1 = &cpu->L;
                            reg2 = &cpu->A;
                            break;
                        case 0x78: // MOV A, B
                            reg1 = &cpu->A;
                            reg2 = &cpu->B;
                            break;
                        case 0x79: // MOV A, C
                            reg1 = &cpu->A;
                            reg2 = &cpu->C;
                            break;
                        case 0x7A: // MOV A, D
                            reg1 = &cpu->A;
                            reg2 = &cpu->D;
                            break;
                        case 0x7B: // MOV A, E
                            reg1 = &cpu->A;
                            reg2 = &cpu->E;
                            break;
                        case 0x7C: // MOV A, H
                            reg1 = &cpu->A;
                            reg2 = &cpu->H;
                            break;
                        case 0x7D: // MOV A, L
                            reg1 = &cpu->A;
                            reg2 = &cpu->L;
                            break;
                        case 0x7F: // MOV A, A
                            reg1 = &cpu->A;
                            reg2 = &cpu->A;
                            break;
                    }
                    MOV(cpu, reg1, reg2);
                    cpu->PC++;
                    cycles = 5;
                    break;

                // MOV Instructions
                case 0x7E: // MOV A, M
                case 0x70: // MOV M, B
                case 0x6E: // MOV L, M
                case 0x66: // MOV H, M
                case 0x5E: // MOV E, M
                case 0x56: // MOV D, M
                case 0x4E: // MOV C, M
                case 0x46: // MOV B, M
                case 0x71: // MOV M, C
                case 0x72: // MOV M, D
                case 0x73: // MOV M, E
                case 0x74: // MOV M, H
                case 0x75: // MOV M, L
                case 0x77: // MOV M, A
                    switch(*opcode){
                        case 0x7E: // MOV A, M
                            reg1 = &cpu->A;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x70: // MOV M, B
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->B;
                            break;
                        case 0x6E: // MOV L, M
                            reg1 = &cpu->L;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x66: // MOV H, M
                            reg1 = &cpu->H;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x5E: // MOV E, M
                            reg1 = &cpu->E;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x56: // MOV D, M
                            reg1 = &cpu->D;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x4E: // MOV C, M
                            reg1 = &cpu->C;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x46: // MOV B, M
                            reg1 = &cpu->B;
                            reg2 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            break;
                        case 0x71: // MOV M, C
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->C;
                            break;
                        case 0x72: // MOV M, D
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->D;
                            break;
                        case 0x73: // MOV M, E
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->E;
                            break;
                        case 0x74: // MOV M, H
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->H;
                            break;
                        case 0x75: // MOV M, L
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->L;
                            break;
                        case 0x77: // MOV M, A
                            reg1 = read_memory(cpu, TO16BIT(cpu->H, cpu->L));
                            reg2 = &cpu->A;
                            break;
                    }
                    MOV(cpu, reg1, reg2);
                    cpu->PC++;
                    cycles = 7;
                    break;

                // MVI Instructions
                case 0x06: // MVI B
                case 0x0E: // MVI C
                case 0x16: // MVI D
                case 0x1E: // MVI E
                case 0x26: // MVI H
                case 0x2E: // MVI L
                case 0x3E: // MVI A
                    switch (*opcode) {
                        case 0x06: // MVI B
                            reg1 = &cpu->B;
                            break;
                        case 0x0E: // MVI C
                            reg1 = &cpu->C;
                            break;
                        case 0x16: // MVI D
                            reg1 = &cpu->D;
                            break;
                        case 0x1E: // MVI E
                            reg1 = &cpu->E;
                            break;
                        case 0x26: // MVI H
                            reg1 = &cpu->H;
                            break;
                        case 0x2E: // MVI L
                            reg1 = &cpu->L;
                            break;
                        case 0x3E: // MVI A
                            reg1 = &cpu->A;
                            break;
                    }
                    *reg1 = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x36: // MVI M
                    *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 10;
                    break;

                // PUSH Instructions
                case 0xC5: // PUSH B
                    PUSH(cpu, cpu->B, cpu->C);
                    cpu->PC++;
                    cycles = 11;
                    break;
                case 0xD5: // PUSH D
                    PUSH(cpu, cpu->D, cpu->E);
                    cpu->PC++;
                    cycles = 11;
                    break;
                case 0xE5: // PUSH H
                    PUSH(cpu, cpu->H, cpu->L);
                    cpu->PC++;
                    cycles = 11;
                    break;
                case 0xF5: // PUSH PSW
                    PUSH_PSW(cpu);
                    break;

                // POP Instructions
                case 0xC1: // POP B
                    POP(cpu, &cpu->B, &cpu->C);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0xD1: // POP D
                    POP(cpu, &cpu->D, &cpu->E);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0xE1: // POP H
                    POP(cpu, &cpu->H, &cpu->L);
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0xF1: // POP PSW
                    POP_PSW(cpu);
                    break;

                // CALL Instructions
                case 0xC4: // CALL NZ
                case 0xCC: // CALL Z
                case 0xD4: // CALL NC
                case 0xDC: // CALL C
                    switch (*opcode) {
                        case 0xC4: condition = !cpu->flags.z; break;
                        case 0xCC: condition = cpu->flags.z; break;
                        case 0xD4: condition = !cpu->flags.cy; break;
                        case 0xDC: condition = cpu->flags.cy; break;
                        default: break;
                    }
                    if(condition){
                        word = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                        CALL(cpu, word);
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;
                case 0xCD: // CALL
                    CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    cpu->PC += 3;
                    cycles = 17;
                    break;

                // RET Instructions
                case 0xC0: // RET NZ
                case 0xC8: // RET Z
                case 0xD0: // RET NC
                case 0xD8: // RET C
                    switch (*opcode) {
                        case 0xC0: condition = !cpu->flags.z; break;
                        case 0xC8: condition = cpu->flags.z; break;
                        case 0xD0: condition = !cpu->flags.cy; break;
                        case 0xD8: condition = cpu->flags.cy; break;
                        default: break;
                    }
                    if(condition){
                        RET(cpu);
                        cycles = 11;
                    }
                    else{
                        cpu->PC++;
                        cycles = 5;
                    }
                    break;
                case 0xC9: // RET
                    RET(cpu);
                    cycles = 10;
                    break;

                // JMP Instructions
                case 0xC2: // JNZ
                case 0xCA: // JZ
                case 0xD2: // JNC
                case 0xDA: // JC
                    switch(*opcode){
                        case 0xC2: condition = !cpu->flags.z; break;
                        case 0xCA: condition = cpu->flags.z; break;
                        case 0xD2: condition = !cpu->flags.cy; break;
                        case 0xDA: condition = cpu->flags.cy; break;
                        default: break;
                    }
                    if(condition){
                        word = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                        JMP(cpu, word);
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0xC3: // JMP
                    JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    cpu->PC += 3;
                    cycles = 10;
                    break;

                // LDA Instruction
                case 0x3A: // LDA
                    cpu->A = *read_memory(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    cpu->PC += 3;
                    cycles = 13;
                    break;

                // LDAX Instructions
                case 0x0A: // LDAX B
                    cpu->A = *read_memory(cpu, TO16BIT(cpu->B, cpu->C));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x1A: // LDAX D
                    cpu->A = *read_memory(cpu, TO16BIT(cpu->D, cpu->E));
                    cpu->PC++;
                    cycles = 7;
                    break;

                // STA Instruction
                case 0x32: // STA
                    write_memory(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)), cpu->A);
                    cpu->PC += 3;
                    cycles = 13;
                    break;

                // STAX Instructions
                case 0x02: // STAX B
                    write_memory(cpu, TO16BIT(cpu->B, cpu->C), cpu->A);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x12: // STAX D
                    write_memory(cpu, TO16BIT(cpu->D, cpu->E), cpu->A);
                    cpu->PC++;
                    cycles = 7;
                    break;

                // LXI Instructions
                case 0x01: // LXI B
                    LXI(cpu, &cpu->B, &cpu->C, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0x11: // LXI D
                    LXI(cpu, &cpu->D, &cpu->E, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0x21: // LXI H
                    LXI(cpu, &cpu->H, &cpu->L, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0x31: // LXI SP
                    cpu->SP = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 3;
                    cycles = 10;
                    break;

                // NOP Instructions
                case 0x00: // NOP
                    NOP(cpu);
                    cpu->PC++;
                    cycles = 4;
                    break;

                case 0x07: // RLC
                    cpu->flags.cy = (cpu->A & 0x80) >> 7;
                    cpu->A = (cpu->A << 1) | cpu->flags.cy;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x0F: // RRC
                    cpu->flags.cy = cpu->A & 0x01;
                    cpu->A = (cpu->A >> 1) | (cpu->flags.cy << 7);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x17: // RAL
                    msb = (cpu->A & 0x80) >> 7;
                    cpu->A = (cpu->A << 1) | cpu->flags.cy;
                    cpu->flags.cy = msb;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x1F: // RAR
                    lsb = (cpu->A & 0x01);
                    cpu->A = (cpu->A >> 1) | (cpu->flags.cy << 7);
                    cpu->flags.cy = lsb;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x22: // SHLD
                    word = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    write_memory(cpu, word, cpu->L);
                    write_memory(cpu, word + 1, cpu->H);
                    cpu->PC += 3;
                    cycles = 16;
                    break;
                case 0x27: // DAA
                    if((cpu->A & 0x0F) > 9 || cpu->flags.ac){
                        if((cpu->A & 0x0F) > 9){
                            cpu->A += 6;
                        }
                        if((cpu->A & 0xF0) > 0x90){
                            cpu->A += 0x60;
                            cpu->flags.cy = 1;
                        }
                        cpu->flags.ac = 1;
                    }
                    else{
                        cpu->flags.ac = 0;
                    }
                case 0x2A: // LHLD
                    word = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                    cpu->L = *read_memory(cpu, word);
                    cpu->H = *read_memory(cpu, word + 1);
                    cpu->PC += 3;
                    cycles = 16;
                    break;
                case 0x2F: // CMA
                    cpu->A = ~cpu->A;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x37: // STC
                    cpu->flags.cy = 1;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x3F: // CMC
                    cpu->flags.cy = !cpu->flags.cy;
                    cpu->PC++;
                    cycles = 4;
                    break;


                case 0x76: // HLT
                    // TODO: cpu->halt = 1;
                    cpu->PC++;
                    cycles = 7;
                    break;

                case 0xC7: // RST 0
                case 0xCF: // RST 1
                case 0xD7: // RST 2
                case 0xDF: // RST 3
                case 0xE7: // RST 4
                case 0xEF: // RST 5
                case 0xF7: // RST 6
                case 0xFF: // RST 7
                    // TODO: RST(cpu, (*opcode & 0x38) >> 3);
                    cpu->PC++;
                    cycles = 11;
                    break;
                case 0xCE: // ACI
                    word = cpu->A + *read_memory(cpu, cpu->PC + 1) + cpu->flags.cy;
                    set_flags(cpu, word);
                    cpu->A = word & 0xFF;
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0xD3: // OUT
                    // TODO: OUT(cpu, *read_memory(cpu, cpu->PC + 1), cpu->A);
                    cpu->PC += 2;
                    cycles = 10;
                    break;
                case 0xDE: // SBI
                    word = cpu->A - *read_memory(cpu, cpu->PC + 1);
                    set_flags(cpu, word);
                    cpu->A = word & 0xFF;
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0xDB: // IN
                    // TODO: cpu->A = IN(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 10;
                    break;
                case 0xE0: // RPO
                    if(!cpu->flags.p){
                        RET(cpu);
                        cycles = 11;
                    }
                    else{
                        cpu->PC++;
                        cycles = 5;
                    }
                case 0xE2: // JPO
                    if(!cpu->flags.p){
                        JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0xE3: // XTHL
                    word = TO16BIT(cpu->H, cpu->L);
                    cpu->H = *read_memory(cpu, cpu->SP + 1);
                    cpu->L = *read_memory(cpu, cpu->SP);
                    write_memory(cpu, cpu->SP + 1, HIGH_BYTE(word));
                    write_memory(cpu, cpu->SP, LOW_BYTE(word));
                    cpu->PC++;
                    cycles = 18;
                    break;
                case 0xE4: // CPO
                    if(!cpu->flags.p){
                        CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;
                case 0xE8: // RPE
                    if(cpu->flags.p){
                        RET(cpu);
                        cycles = 11;
                    }
                    else{
                        cpu->PC++;
                        cycles = 5;
                    }
                    break;
                case 0xEA: // JPE
                    if(cpu->flags.p){
                        JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0xEB: // XCHG
                    word = TO16BIT(cpu->H, cpu->L);
                    cpu->H = cpu->D;
                    cpu->L = cpu->E;
                    cpu->D = HIGH_BYTE(word);
                    cpu->E = LOW_BYTE(word);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0xEC: // CPE
                    if(cpu->flags.p){
                        CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;
                case 0xF0: // RP
                    if(!cpu->flags.s){
                        RET(cpu);
                        cycles = 11;
                    }
                    else{
                        cpu->PC++;
                        cycles = 5;
                    }
                    break;
                case 0xF2: // JP
                    if(!cpu->flags.s){
                        JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0xF4: // CP
                    if(!cpu->flags.s){
                        CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;
                case 0xF8: // RM
                    if(cpu->flags.s){
                        RET(cpu);
                        cycles = 11;
                    }
                    else{
                        cpu->PC++;
                        cycles = 5;
                    }
                    break;
                case 0xFA: // JM
                    if(cpu->flags.s){
                        JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;
                case 0xFC: // CM
                    if(cpu->flags.s){
                        CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;
                case 0xFB: // EI
                    // TODO: cpu->interrupts = 1;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xF3: // DI
                    // TODO: cpu->interrupts = 0;
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xE9: // PCHL
                    cpu->PC = TO16BIT(cpu->H, cpu->L);
                    cycles = 5;
                    break;
                case 0xF9: // SPHL
                    cpu->SP = TO16BIT(cpu->H, cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;


                case 0xFD: // CALL
                case 0xED: // CALL
                case 0xDD: // CALL
                case 0xD9: // RET
                case 0xCB: // JMP
                    break;

                // NOP Instructions
                case 0x10: // NOP
                case 0x20: // NOP
                case 0x30: // NOP
                case 0x08: // NOP
                case 0x18: // NOP
                case 0x28: // NOP
                case 0x38: // NOP
                    //printf("Unimplemented opcode: 0x%02x\n", *opcode);
                    cpu->PC++;
                    break;
                default:
                    printf("Unknown opcode: 0x%02x\n", *opcode);
                    cpu->PC++;
            }
        }
    }
    return cycles;
}

static uint8_t check_parity(uint8_t value){
    uint8_t parity = 0;
    for(int i = 0; i < 8; i++){
        if(value & 0x01){
            parity++;
        }
        value >>= 1;
    }
    return (parity % 2 == 0);
}

static void set_flags(i8080_t *cpu, uint16_t result){
    cpu->flags.z = (result & 0xFF) == 0;
    cpu->flags.s = (result & 0x80) != 0;
    cpu->flags.p = check_parity(result & 0xFF);
    cpu->flags.cy = (result & 0xFF00) != 0;
    cpu->flags.ac = (result & 0x0F) != 0;
}

void ADD(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A + reg;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void SUB(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A - reg;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void ANA(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A & reg;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void ORA(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A | reg;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void XRA(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A ^ reg;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void CMP(i8080_t *cpu, uint8_t reg){
    uint16_t result = cpu->A - reg;
    set_flags(cpu, result);
}

void INR(i8080_t *cpu, uint8_t *reg){
    uint16_t result = *reg + 1;
    set_flags(cpu, result);
    *reg = result & 0xFF;
}

void INX(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2){
    uint16_t result = TO16BIT(*reg1, *reg2) + 1;
    *reg1 = (result >> 8) & 0xFF;
    *reg2 = result & 0xFF;
}

void DCR(i8080_t *cpu, uint8_t *reg){
    uint16_t result = *reg - 1;
    set_flags(cpu, result);
    *reg = result & 0xFF;
}

void DCX(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2){
    uint16_t result = TO16BIT(*reg1, *reg2) - 1;
    *reg1 = (result >> 8) & 0xFF;
    *reg2 = result & 0xFF;
}

void DAD(i8080_t *cpu, uint8_t reg1, uint8_t reg2){
    uint16_t result = TO16BIT(cpu->H, cpu->L) + TO16BIT(reg1, reg2);
    cpu->H = HIGH_BYTE(result);
    cpu->L = LOW_BYTE(result);
    cpu->flags.cy = (result & 0xFF00) != 0;
}

void MOV(i8080_t *cpu, uint8_t *reg1, const uint8_t *reg2){
    *reg1 = *reg2;
}

void POP(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2){
    *reg2 = *read_memory(cpu, cpu->SP);
    cpu->SP++;
    *reg1 = *read_memory(cpu, cpu->SP);
    cpu->SP++;
}

void POP_PSW(i8080_t *cpu){
    uint8_t psw = *read_memory(cpu, cpu->SP);

    cpu->flags.z = (psw & 0x40) != 0;
    cpu->flags.s = (psw & 0x80) != 0;
    cpu->flags.p = (psw & 0x04) != 0;
    cpu->flags.cy = (psw & 0x01) != 0;
    cpu->flags.ac = (psw & 0x10) != 0;

    cpu->A = *read_memory(cpu, cpu->SP + 1);
    cpu->SP += 2;
}

void PUSH(i8080_t *cpu, uint8_t reg1, uint8_t reg2){
    cpu->SP--;
    write_memory(cpu, cpu->SP, reg1);
    cpu->SP--;
    write_memory(cpu, cpu->SP, reg2);
}

void PUSH_PSW(i8080_t *cpu){
    uint8_t psw = 0;

    psw |= cpu->flags.z << 6;
    psw |= cpu->flags.s << 7;
    psw |= cpu->flags.p << 2;
    psw |= cpu->flags.cy;
    psw |= cpu->flags.ac << 4;

    cpu->SP--;
    write_memory(cpu, cpu->SP, psw);
    cpu->SP--;
    write_memory(cpu, cpu->SP, cpu->A);
}

void CALL(i8080_t *cpu, uint16_t address){
    PUSH(cpu, HIGH_BYTE(cpu->PC), LOW_BYTE(cpu->PC));
    cpu->PC = address;
}

void RET(i8080_t *cpu){
    uint8_t pc_low = LOW_BYTE(cpu->PC);
    uint8_t pc_high = HIGH_BYTE(cpu->PC);
    POP(cpu, &pc_high, &pc_low);
}

void JMP(i8080_t *cpu, uint16_t address){
    cpu->PC = address;
}

void LXI(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2, uint8_t high, uint8_t low){
    *reg1 = high;
    *reg2 = low;
}

void NOP(i8080_t *cpu){
    // Do nothing
}