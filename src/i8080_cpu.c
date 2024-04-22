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

uint8_t emulate_cycle(i8080_t *cpu){
    uint8_t cycles = 0;
    if(cpu != NULL){
        uint8_t* opcode = read_memory(cpu, cpu->PC);
        uint16_t address;
        bool condition = false;
        if(opcode != NULL){
            switch(*opcode){

                case 0x80: // ADD B
                    ADD(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x81: // ADD C
                    ADD(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x82: // ADD D
                    ADD(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x83: // ADD E
                    ADD(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x84: // ADD H
                    ADD(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x85: // ADD L
                    ADD(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x86: // ADD M
                    ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x87: // ADD A
                    ADD(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xC6: // ADI
                    ADD(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x88: // ADC B
                    ADD(cpu, cpu->B + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x89: // ADC C
                    ADD(cpu,cpu->C + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8A: // ADC D
                    ADD(cpu,cpu->D + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8B: // ADC E
                    ADD(cpu, cpu->E + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8C: // ADC H
                    ADD(cpu, cpu->H + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8D: // ADC L
                    ADD(cpu, cpu->L + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x8E: // ADC M
                    ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x8F: // ADC A
                    ADD(cpu, cpu->A + cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;

                case 0x90: // SUB B
                    SUB(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x91: // SUB C
                    SUB(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x92: // SUB D
                    SUB(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x93: // SUB E
                    SUB(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x94: // SUB H
                    SUB(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x95: // SUB L
                    SUB(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x96: // SUB M
                    SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x97: // SUB A
                    SUB(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x98: // SBB B
                    SUB(cpu, cpu->B - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x99: // SBB C
                    SUB(cpu, cpu->C - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9A: // SBB D
                    SUB(cpu, cpu->D - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9B: // SBB E
                    SUB(cpu, cpu->E - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9C: // SBB H
                    SUB(cpu, cpu->H - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9D: // SBB L
                    SUB(cpu, cpu->L - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0x9E: // SBB M
                    SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x9F: // SBB A
                    SUB(cpu, cpu->A - cpu->flags.cy);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xD6: // SUI
                    SUB(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                case 0xA0: // ANA B
                    ANA(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA1: // ANA C
                    ANA(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA2: // ANA D
                    ANA(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA3: // ANA E
                    ANA(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA4: // ANA H
                    ANA(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA5: // ANA L
                    ANA(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA6: // ANA M
                    ANA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0xA7: // ANA A
                    ANA(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xE6: // ANI
                    ANA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                case 0xB0: // ORA B
                    ORA(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB1: // ORA C
                    ORA(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB2: // ORA D
                    ORA(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB3: // ORA E
                    ORA(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB4: // ORA H
                    ORA(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB5: // ORA L
                    ORA(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB6: // ORA M
                    ORA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0xB7: // ORA A
                    ORA(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xF6: // ORI
                    ORA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                case 0xA8: // XRA B
                    XRA(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xA9: // XRA C
                    XRA(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAA: // XRA D
                    XRA(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAB: // XRA E
                    XRA(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAC: // XRA H
                    XRA(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAD: // XRA L
                    XRA(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xAE: // XRA M
                    XRA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0xAF: // XRA A
                    XRA(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xEE: // XRI
                    XRA(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                case 0xB8: // CMP B
                    CMP(cpu, cpu->B);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xB9: // CMP C
                    CMP(cpu, cpu->C);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBA: // CMP D
                    CMP(cpu, cpu->D);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBB: // CMP E
                    CMP(cpu, cpu->E);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBC: // CMP H
                    CMP(cpu, cpu->H);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBD: // CMP L
                    CMP(cpu, cpu->L);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xBE: // CMP M
                    CMP(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0xBF: // CMP A
                    CMP(cpu, cpu->A);
                    cpu->PC++;
                    cycles = 4;
                    break;
                case 0xFE: // CPI
                    CMP(cpu, *read_memory(cpu, cpu->PC + 1));
                    cpu->PC += 2;
                    cycles = 7;
                    break;

                case 0x04: // INR B
                    INR(cpu, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x0C: // INR C
                    INR(cpu, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x14: // INR D
                    INR(cpu, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x1C: // INR E
                    INR(cpu, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x24: // INR H
                    INR(cpu, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x2C: // INR L
                    INR(cpu, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x34: // INR M
                    INR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0x3C: // INR A
                    INR(cpu, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;

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

                case 0x05: // DCR B
                    DCR(cpu, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x0D: // DCR C
                    DCR(cpu, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x15: // DCR D
                    DCR(cpu, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x1D: // DCR E
                    DCR(cpu, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x25: // DCR H
                    DCR(cpu, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x2D: // DCR L
                    DCR(cpu, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x35: // DCR M
                    DCR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 10;
                    break;
                case 0x3D: // DCR A
                    DCR(cpu, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;

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



                case 0x40: // MOV B, B
                    MOV(cpu, &cpu->B, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x41: // MOV B, C
                    MOV(cpu, &cpu->B, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x42: // MOV B, D
                    MOV(cpu, &cpu->B, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x43: // MOV B, E
                    MOV(cpu, &cpu->B, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x44: // MOV B, H
                    MOV(cpu, &cpu->B, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x45: // MOV B, L
                    MOV(cpu, &cpu->B, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x46: // MOV B, M
                    MOV(cpu, &cpu->B, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x47: // MOV B, A
                    MOV(cpu, &cpu->B, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x48: // MOV C, B
                    MOV(cpu, &cpu->C, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x49: // MOV C, C
                    MOV(cpu, &cpu->C, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x4A: // MOV C, D
                    MOV(cpu, &cpu->C, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x4B: // MOV C, E
                    MOV(cpu, &cpu->C, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x4C: // MOV C, H
                    MOV(cpu, &cpu->C, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x4D: // MOV C, L
                    MOV(cpu, &cpu->C, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x4E: // MOV C, M
                    MOV(cpu, &cpu->C, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x4F: // MOV C, A
                    MOV(cpu, &cpu->C, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x50: // MOV D, B
                    MOV(cpu, &cpu->D, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x51: // MOV D, C
                    MOV(cpu, &cpu->D, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x52: // MOV D, D
                    MOV(cpu, &cpu->D, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x53: // MOV D, E
                    MOV(cpu, &cpu->D, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x54: // MOV D, H
                    MOV(cpu, &cpu->D, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x55: // MOV D, L
                    MOV(cpu, &cpu->D, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x56: // MOV D, M
                    MOV(cpu, &cpu->D, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x57: // MOV D, A
                    MOV(cpu, &cpu->D, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x58: // MOV E, B
                    MOV(cpu, &cpu->E, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x59: // MOV E, C
                    MOV(cpu, &cpu->E, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x5A: // MOV E, D
                    MOV(cpu, &cpu->E, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x5B: // MOV E, E
                    MOV(cpu, &cpu->E, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x5C: // MOV E, H
                    MOV(cpu, &cpu->E, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x5D: // MOV E, L
                    MOV(cpu, &cpu->E, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x5E: // MOV E, M
                    MOV(cpu, &cpu->E, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x5F: // MOV E, A
                    MOV(cpu, &cpu->E, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x60: // MOV H, B
                    MOV(cpu, &cpu->H, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x61: // MOV H, C
                    MOV(cpu, &cpu->H, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x62: // MOV H, D
                    MOV(cpu, &cpu->H, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x63: // MOV H, E
                    MOV(cpu, &cpu->H, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x64: // MOV H, H
                    MOV(cpu, &cpu->H, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x65: // MOV H, L
                    MOV(cpu, &cpu->H, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x66: // MOV H, M
                    MOV(cpu, &cpu->H, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x67: // MOV H, A
                    MOV(cpu, &cpu->H, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x68: // MOV L, B
                    MOV(cpu, &cpu->L, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x69: // MOV L, C
                    MOV(cpu, &cpu->L, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x6A: // MOV L, D
                    MOV(cpu, &cpu->L, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x6B: // MOV L, E
                    MOV(cpu, &cpu->L, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x6C: // MOV L, H
                    MOV(cpu, &cpu->L, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x6D: // MOV L, L
                    MOV(cpu, &cpu->L, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x6E: // MOV L, M
                    MOV(cpu, &cpu->L, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x6F: // MOV L, A
                    MOV(cpu, &cpu->L, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x70: // MOV M, B
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->B);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x71: // MOV M, C
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->C);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x72: // MOV M, D
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->D);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x73: // MOV M, E
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->E);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x74: // MOV M, H
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->H);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x75: // MOV M, L
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->L);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x77: // MOV M, A
                    MOV(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L)), &cpu->A);
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x78: // MOV A, B
                    MOV(cpu, &cpu->A, &cpu->B);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x79: // MOV A, C
                    MOV(cpu, &cpu->A, &cpu->C);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x7A: // MOV A, D
                    MOV(cpu, &cpu->A, &cpu->D);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x7B: // MOV A, E
                    MOV(cpu, &cpu->A, &cpu->E);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x7C: // MOV A, H
                    MOV(cpu, &cpu->A, &cpu->H);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x7D: // MOV A, L
                    MOV(cpu, &cpu->A, &cpu->L);
                    cpu->PC++;
                    cycles = 5;
                    break;
                case 0x7E: // MOV A, M
                    MOV(cpu, &cpu->A, read_memory(cpu, TO16BIT(cpu->H, cpu->L)));
                    cpu->PC++;
                    cycles = 7;
                    break;
                case 0x7F: // MOV A, A
                    MOV(cpu, &cpu->A, &cpu->A);
                    cpu->PC++;
                    cycles = 5;
                    break;

                case 0x06: // MVI B
                    cpu->B = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x0E: // MVI C
                    cpu->C = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x16: // MVI D
                    cpu->D = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x1E: // MVI E
                    cpu->E = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x26: // MVI H
                    cpu->H = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x2E: // MVI L
                    cpu->L = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;
                case 0x36: // MVI M
                    *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 10;
                    break;
                case 0x3E: // MVI A
                    cpu->A = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2;
                    cycles = 7;
                    break;

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
                        address = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                        CALL(cpu, address);
                        cycles = 17;
                    }
                    else{
                        cpu->PC += 3;
                        cycles = 11;
                    }
                    break;

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
                        address = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1));
                        JMP(cpu, address);
                    }
                    else cpu->PC += 3;
                    cycles = 10;
                    break;


                case 0xCD: // CALL
                    CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)));
                    cpu->PC += 3;
                    cycles = 17;
                    break;

                case 0xC9: // RET
                    RET(cpu);
                    cycles = 10;
                    break;


                default:
                    printf("Unknown opcode: 0x%02x\n", *opcode);
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
