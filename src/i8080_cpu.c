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
                case 0xC6: ADD(cpu, *read_memory(cpu, cpu->PC + 1), 0); cpu->PC += 2; cycles = 7; break;

                // ADD instructions
                case 0x80: ADD(cpu, cpu->B, 0); cpu->PC++; cycles = 4; break;
                case 0x81: ADD(cpu, cpu->C, 0); cpu->PC++; cycles = 4; break;
                case 0x82: ADD(cpu, cpu->D, 0); cpu->PC++; cycles = 4; break;
                case 0x83: ADD(cpu, cpu->E, 0); cpu->PC++; cycles = 4; break;
                case 0x84: ADD(cpu, cpu->H, 0); cpu->PC++; cycles = 4; break;
                case 0x85: ADD(cpu, cpu->L, 0); cpu->PC++; cycles = 4; break;
                case 0x87: ADD(cpu, cpu->A, 0); cpu->PC++; cycles = 4; break;
                case 0x86: ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)), 0); cpu->PC++; cycles = 7; break;

                // ADC instructions
                case 0x88: ADD(cpu, cpu->B, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x89: ADD(cpu, cpu->C, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8A: ADD(cpu, cpu->D, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8B: ADD(cpu, cpu->E, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8C: ADD(cpu, cpu->H, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8D: ADD(cpu, cpu->L, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8F: ADD(cpu, cpu->A, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x8E: ADD(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)), cpu->flags.cy); cpu->PC++; cycles = 7; break;

                // SUB instructions
                case 0x90: SUB(cpu, cpu->B, 0); cpu->PC++; cycles = 4; break;
                case 0x91: SUB(cpu, cpu->C, 0); cpu->PC++; cycles = 4; break;
                case 0x92: SUB(cpu, cpu->D, 0); cpu->PC++; cycles = 4; break;
                case 0x93: SUB(cpu, cpu->E, 0); cpu->PC++; cycles = 4; break;
                case 0x94: SUB(cpu, cpu->H, 0); cpu->PC++; cycles = 4; break;
                case 0x95: SUB(cpu, cpu->L, 0); cpu->PC++; cycles = 4; break;
                case 0x97: SUB(cpu, cpu->A, 0); cpu->PC++; cycles = 4; break;
                case 0x96: SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)), 0); cpu->PC++; cycles = 7; break;

                // SBB instructions
                case 0x98: SUB(cpu, cpu->B, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x99: SUB(cpu, cpu->C, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9A: SUB(cpu, cpu->D, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9B: SUB(cpu, cpu->E, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9C: SUB(cpu, cpu->H, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9D: SUB(cpu, cpu->L, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9F: SUB(cpu, cpu->A, cpu->flags.cy); cpu->PC++; cycles = 4; break;
                case 0x9E: SUB(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L)), cpu->flags.cy); cpu->PC++; cycles = 7; break;

                // SUI instruction
                case 0xD6: SUB(cpu, *read_memory(cpu, cpu->PC + 1), 0); cpu->PC += 2; cycles = 7; break;

                // ANA Instructions
                case 0xA0: ANA(cpu, cpu->B); cpu->PC++; cycles = 4; break;
                case 0xA1: ANA(cpu, cpu->C); cpu->PC++; cycles = 4; break;
                case 0xA2: ANA(cpu, cpu->D); cpu->PC++; cycles = 4; break;
                case 0xA3: ANA(cpu, cpu->E); cpu->PC++; cycles = 4; break;
                case 0xA4: ANA(cpu, cpu->H); cpu->PC++; cycles = 4; break;
                case 0xA5: ANA(cpu, cpu->L); cpu->PC++; cycles = 4; break;
                case 0xA7: ANA(cpu, cpu->A); cpu->PC++; cycles = 4; break;
                case 0xA6: ANA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 7; break;

                // ANI Instruction
                case 0xE6: ANA(cpu, *read_memory(cpu, cpu->PC + 1)); cpu->PC += 2; cycles = 7; break;

                // ORA Instructions
                case 0xB0: ORA(cpu, cpu->B); cpu->PC++; cycles = 4; break;
                case 0xB1: ORA(cpu, cpu->C); cpu->PC++; cycles = 4; break;
                case 0xB2: ORA(cpu, cpu->D); cpu->PC++; cycles = 4; break;
                case 0xB3: ORA(cpu, cpu->E); cpu->PC++; cycles = 4; break;
                case 0xB4: ORA(cpu, cpu->H); cpu->PC++; cycles = 4; break;
                case 0xB5: ORA(cpu, cpu->L); cpu->PC++; cycles = 4; break;
                case 0xB7: ORA(cpu, cpu->A); cpu->PC++; cycles = 4; break;
                case 0xB6: ORA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 7; break;

                // ORI Instruction
                case 0xF6: ORA(cpu, *read_memory(cpu, cpu->PC + 1)); cpu->PC += 2; cycles = 7; break;

                // XRA Instructions
                case 0xA8: XRA(cpu, cpu->B); cpu->PC++; cycles = 4; break;
                case 0xA9: XRA(cpu, cpu->C); cpu->PC++; cycles = 4; break;
                case 0xAA: XRA(cpu, cpu->D); cpu->PC++; cycles = 4; break;
                case 0xAB: XRA(cpu, cpu->E); cpu->PC++; cycles = 4; break;
                case 0xAC: XRA(cpu, cpu->H); cpu->PC++; cycles = 4; break;
                case 0xAD: XRA(cpu, cpu->L); cpu->PC++; cycles = 4; break;
                case 0xAF: XRA(cpu, cpu->A); cpu->PC++; cycles = 4; break;
                case 0xAE: XRA(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 7; break;

                // XRI Instruction
                case 0xEE: XRA(cpu, *read_memory(cpu, cpu->PC + 1)); cpu->PC += 2; cycles = 7; break;

                // CMP Instructions
                case 0xB8: CMP(cpu, cpu->B); cpu->PC++; cycles = 4; break;
                case 0xB9: CMP(cpu, cpu->C); cpu->PC++; cycles = 4; break;
                case 0xBA: CMP(cpu, cpu->D); cpu->PC++; cycles = 4; break;
                case 0xBB: CMP(cpu, cpu->E); cpu->PC++; cycles = 4; break;
                case 0xBC: CMP(cpu, cpu->H); cpu->PC++; cycles = 4; break;
                case 0xBD: CMP(cpu, cpu->L); cpu->PC++; cycles = 4; break;
                case 0xBF: CMP(cpu, cpu->A); cpu->PC++; cycles = 4; break;
                case 0xBE: CMP(cpu, *read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 7; break;

                // CPI Instruction
                case 0xFE: CMP(cpu, *read_memory(cpu, cpu->PC + 1)); cpu->PC += 2; cycles = 7; break;

                // INR Instructions
                case 0x04: INR(cpu, &cpu->B); cpu->PC++; cycles = 5; break;
                case 0x0C: INR(cpu, &cpu->C); cpu->PC++; cycles = 5; break;
                case 0x14: INR(cpu, &cpu->D); cpu->PC++; cycles = 5; break;
                case 0x1C: INR(cpu, &cpu->E); cpu->PC++; cycles = 5; break;
                case 0x24: INR(cpu, &cpu->H); cpu->PC++; cycles = 5; break;
                case 0x2C: INR(cpu, &cpu->L); cpu->PC++; cycles = 5; break;
                case 0x3C: INR(cpu, &cpu->A); cpu->PC++; cycles = 5; break;
                case 0x34: INR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 10; break;

                // INX Instructions
                case 0x03: INX(cpu, &cpu->B, &cpu->C); cpu->PC++; cycles = 5; break;
                case 0x13: INX(cpu, &cpu->D, &cpu->E); cpu->PC++; cycles = 5; break;
                case 0x23: INX(cpu, &cpu->H, &cpu->L); cpu->PC++; cycles = 5; break;
                case 0x33: cpu->SP++; cpu->PC++; cycles = 5; break;

                // DCR Instructions
                case 0x05: DCR(cpu, &cpu->B); cpu->PC++; cycles = 5; break;
                case 0x0D: DCR(cpu, &cpu->C); cpu->PC++; cycles = 5; break;
                case 0x15: DCR(cpu, &cpu->D); cpu->PC++; cycles = 5; break;
                case 0x1D: DCR(cpu, &cpu->E); cpu->PC++; cycles = 5; break;
                case 0x25: DCR(cpu, &cpu->H); cpu->PC++; cycles = 5; break;
                case 0x2D: DCR(cpu, &cpu->L); cpu->PC++; cycles = 5; break;
                case 0x3D: DCR(cpu, &cpu->A); cpu->PC++; cycles = 5; break;
                case 0x35: DCR(cpu, read_memory(cpu, TO16BIT(cpu->H, cpu->L))); cpu->PC++; cycles = 10; break;

                // DCX Instructions
                case 0x0B: DCX(cpu, &cpu->B, &cpu->C); cpu->PC++; cycles = 5; break;
                case 0x1B: DCX(cpu, &cpu->D, &cpu->E); cpu->PC++; cycles = 5; break;
                case 0x2B: DCX(cpu, &cpu->H, &cpu->L); cpu->PC++; cycles = 5; break;
                case 0x3B: cpu->SP--; cpu->PC++; cycles = 5; break;

                // DAD Instructions
                case 0x09: DAD(cpu, cpu->B, cpu->C); cpu->PC++; cycles = 10; break;
                case 0x19: DAD(cpu, cpu->D, cpu->E); cpu->PC++; cycles = 10; break;
                case 0x29: DAD(cpu, cpu->H, cpu->L); cpu->PC++; cycles = 10; break;
                case 0x39: DAD(cpu, HIGH_BYTE(cpu->SP), LOW_BYTE(cpu->SP)); cpu->PC++; cycles = 10; break;

                // MOV B Instructions
                case 0x40: cpu->B = cpu->B; cycles = 5; break;
                case 0x41: cpu->B = cpu->C; cycles = 5; break;
                case 0x42: cpu->B = cpu->D; cycles = 5; break;
                case 0x43: cpu->B = cpu->E; cycles = 5; break;
                case 0x44: cpu->B = cpu->H; cycles = 5; break;
                case 0x45: cpu->B = cpu->L; cycles = 5; break;
                case 0x47: cpu->B = cpu->A; cycles = 5; break;

                // MOV C Instructions
                case 0x48: cpu->C = cpu->B; cycles = 5; break;
                case 0x49: cpu->C = cpu->C; cycles = 5; break;
                case 0x4A: cpu->C = cpu->D; cycles = 5; break;
                case 0x4B: cpu->C = cpu->E; cycles = 5; break;
                case 0x4C: cpu->C = cpu->H; cycles = 5; break;
                case 0x4D: cpu->C = cpu->L; cycles = 5; break;
                case 0x4F: cpu->C = cpu->A; cycles = 5; break;

                // MOV D Instructions
                case 0x50: cpu->D = cpu->B; cycles = 5; break;
                case 0x51: cpu->D = cpu->C; cycles = 5; break;
                case 0x52: cpu->D = cpu->D; cycles = 5; break;
                case 0x53: cpu->D = cpu->E; cycles = 5; break;
                case 0x54: cpu->D = cpu->H; cycles = 5; break;
                case 0x55: cpu->D = cpu->L; cycles = 5; break;
                case 0x57: cpu->D = cpu->A; cycles = 5; break;

                // MOV D Instructions
                case 0x58: cpu->E = cpu->B; cycles = 5; break;
                case 0x59: cpu->E = cpu->C; cycles = 5; break;
                case 0x5A: cpu->E = cpu->D; cycles = 5; break;
                case 0x5B: cpu->E = cpu->E; cycles = 5; break;
                case 0x5C: cpu->E = cpu->H; cycles = 5; break;
                case 0x5D: cpu->E = cpu->L; cycles = 5; break;
                case 0x5F: cpu->E = cpu->A; cycles = 5; break;

                // MOV D Instructions
                case 0x60: cpu->H = cpu->B; cycles = 5; break;
                case 0x61: cpu->H = cpu->C; cycles = 5; break;
                case 0x62: cpu->H = cpu->D; cycles = 5; break;
                case 0x63: cpu->H = cpu->E; cycles = 5; break;
                case 0x64: cpu->H = cpu->H; cycles = 5; break;
                case 0x65: cpu->H = cpu->L; cycles = 5; break;
                case 0x67: cpu->H = cpu->A; cycles = 5; break;

                // MOV D Instructions
                case 0x68: cpu->L = cpu->B; cycles = 5; break;
                case 0x69: cpu->L = cpu->C; cycles = 5; break;
                case 0x6A: cpu->L = cpu->D; cycles = 5; break;
                case 0x6B: cpu->L = cpu->E; cycles = 5; break;
                case 0x6C: cpu->L = cpu->H; cycles = 5; break;
                case 0x6D: cpu->L = cpu->L; cycles = 5; break;
                case 0x6F: cpu->L = cpu->A; cycles = 5; break;

                // MOV D Instructions
                case 0x78: cpu->A = cpu->B; cycles = 5; break;
                case 0x79: cpu->A = cpu->C; cycles = 5; break;
                case 0x7A: cpu->A = cpu->D; cycles = 5; break;
                case 0x7B: cpu->A = cpu->E; cycles = 5; break;
                case 0x7C: cpu->A = cpu->H; cycles = 5; break;
                case 0x7D: cpu->A = cpu->L; cycles = 5; break;
                case 0x7F: cpu->A = cpu->A; cycles = 5; break;

                // MOV Instructions
                case 0x7E: cpu->A = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x6E: cpu->L = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x66: cpu->H = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x5E: cpu->E = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x56: cpu->D = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x4E: cpu->C = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;
                case 0x46: cpu->B = *read_memory(cpu, TO16BIT(cpu->H, cpu->L)); cycles = 7; break;

                case 0x70: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->B); cycles = 7; break;
                case 0x71: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->C); cycles = 7; break;
                case 0x72: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->D); cycles = 7; break;
                case 0x73: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->E); cycles = 7; break;
                case 0x74: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->H); cycles = 7; break;
                case 0x75: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->L); cycles = 7; break;
                case 0x77: write_memory(cpu, TO16BIT(cpu->H, cpu->L), cpu->A); cycles = 7; break;

                // MVI Instructions
                case 0x06: cpu->B = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x0E: cpu->C = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x16: cpu->D = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x1E: cpu->E = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x26: cpu->H = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x2E: cpu->L = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x3E: cpu->A = *read_memory(cpu, cpu->PC + 1); cpu->PC += 2; cycles = 7; break;
                case 0x36: *read_memory(cpu, TO16BIT(cpu->H, cpu->L)) = *read_memory(cpu, cpu->PC + 1);
                    cpu->PC += 2; cycles = 10; break;

                // PUSH Instructions
                case 0xC5: PUSH(cpu, cpu->B, cpu->C); cpu->PC++; cycles = 11; break;
                case 0xD5: PUSH(cpu, cpu->D, cpu->E); cpu->PC++; cycles = 11; break;
                case 0xE5: PUSH(cpu, cpu->H, cpu->L); cpu->PC++; cycles = 11; break;
                case 0xF5: PUSH_PSW(cpu); break;

                // POP Instructions
                case 0xC1: POP(cpu, &cpu->B, &cpu->C); cpu->PC++; cycles = 10; break;
                case 0xD1: POP(cpu, &cpu->D, &cpu->E); cpu->PC++; cycles = 10; break;
                case 0xE1: POP(cpu, &cpu->H, &cpu->L); cpu->PC++; cycles = 10; break;
                case 0xF1: POP_PSW(cpu); break;

                // CALL Instructions
                case 0xC4: cycles = Ccc(cpu, !cpu->flags.z, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); break;
                case 0xCC: cycles = Ccc(cpu, cpu->flags.z, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); break;
                case 0xD4: cycles = Ccc(cpu, !cpu->flags.cy, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); break;
                case 0xDC: cycles = Ccc(cpu, cpu->flags.cy, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); break;
                case 0xCD: CALL(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 17; break;

                // RET Instructions
                case 0xC0: cycles = Rcc(cpu, !cpu->flags.z); break;
                case 0xC8: cycles = Rcc(cpu, cpu->flags.z); break;
                case 0xD0: cycles = Rcc(cpu, !cpu->flags.cy); break;
                case 0xD8: cycles = Rcc(cpu, cpu->flags.cy); break;
                case 0xC9: RET(cpu); cycles = 10; break;

                // JMP Instructions
                case 0xC2: Jcc(cpu, !cpu->flags.z, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 10; break;
                case 0xCA: Jcc(cpu, cpu->flags.z, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 10; break;
                case 0xD2: Jcc(cpu, !cpu->flags.cy, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 10; break;
                case 0xDA: Jcc(cpu, cpu->flags.cy, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 10; break;
                case 0xC3: JMP(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 10; break;

                // LDA Instruction
                case 0x3A: cpu->A = *read_memory(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1))); cpu->PC += 3; cycles = 13; break;

                // LDAX Instructions
                case 0x0A: cpu->A = *read_memory(cpu, TO16BIT(cpu->B, cpu->C)); cpu->PC++; cycles = 7; break;
                case 0x1A: cpu->A = *read_memory(cpu, TO16BIT(cpu->D, cpu->E)); cpu->PC++; cycles = 7; break;

                // STA Instruction
                case 0x32: write_memory(cpu, TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)), cpu->A); cpu->PC += 3; cycles = 13; break;

                // STAX Instructions
                case 0x02: write_memory(cpu, TO16BIT(cpu->B, cpu->C), cpu->A); cpu->PC++; cycles = 7; break;
                case 0x12: write_memory(cpu, TO16BIT(cpu->D, cpu->E), cpu->A); cpu->PC++; cycles = 7; break;

                // LXI Instructions
                case 0x01: LXI(cpu, &cpu->B, &cpu->C, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)); cpu->PC += 3; cycles = 10; break;
                case 0x11: LXI(cpu, &cpu->D, &cpu->E, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)); cpu->PC += 3; cycles = 10; break;
                case 0x21: LXI(cpu, &cpu->H, &cpu->L, *read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)); cpu->PC += 3; cycles = 10; break;
                case 0x31: cpu->SP = TO16BIT(*read_memory(cpu, cpu->PC + 2), *read_memory(cpu, cpu->PC + 1)); cpu->PC += 3; cycles = 10; break;

                // NOP Instructions
                case 0x00: // NOP
                    NOP(cpu);
                    cpu->PC++;
                    cycles = 4;
                    break;

                case 0x07: cpu->flags.cy = (cpu->A & 0x80) >> 7; cpu->A = (cpu->A << 1) | cpu->flags.cy; cpu->PC++; cycles = 4; break;
                case 0x0F: cpu->flags.cy = cpu->A & 0x01; cpu->A = (cpu->A >> 1) | (cpu->flags.cy << 7); cpu->PC++; cycles = 4; break;
                case 0x17: msb = (cpu->A & 0x80) >> 7; cpu->A = (cpu->A << 1) | cpu->flags.cy; cpu->flags.cy = msb; cpu->PC++; cycles = 4; break;
                case 0x1F: lsb = (cpu->A & 0x01); cpu->A = (cpu->A >> 1) | (cpu->flags.cy << 7); cpu->flags.cy = lsb; cpu->PC++; cycles = 4; break;
                case 0x22:
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
                case 0x2F: cpu->A = ~cpu->A; cpu->PC++; cycles = 4; break;
                case 0x37: cpu->flags.cy = 1; cpu->PC++; cycles = 4; break;
                case 0x3F: cpu->flags.cy = !cpu->flags.cy; cpu->PC++; cycles = 4; break;


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

void ADD(i8080_t *cpu, uint8_t val1, uint8_t val2){
    uint16_t result = cpu->A + val1 + val2;
    set_flags(cpu, result);
    cpu->A = result & 0xFF;
}

void SUB(i8080_t *cpu, uint8_t val1, uint8_t val2){
    uint16_t result = cpu->A - val1 - val2;
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

uint8_t Ccc(i8080_t *cpu, bool condition, uint16_t address){
    if(condition){
        CALL(cpu, address);
        return 17;
    }
    else{
        cpu->PC += 3;
        return 11;
    }
}

uint8_t Rcc(i8080_t *cpu, bool condition){
    if(condition){
        RET(cpu);
        return 11;
    }
    else{
        cpu->PC++;
        return 5;
    }
}

void Jcc(i8080_t *cpu, bool condition, uint16_t address){
    if(condition){
        JMP(cpu, address);
    }
    else cpu->PC += 3;
}