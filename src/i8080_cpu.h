//
// Created by leonv on 4/19/2024.
//

#ifndef INTEL8080_I8080_CPU_H
#define INTEL8080_I8080_CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define HIGH_BYTE(val) ((val & 0xFF00) >> 8)
#define LOW_BYTE(val) (val & 0x00FF)
#define TO16BIT(h, l) ((h << 8) | l)

typedef struct {
    uint8_t z:1; // Zero Flag
    uint8_t s:1; // Sign Flag
    uint8_t p:1; // Parity Flag
    uint8_t cy:1; // Carry Flag
    uint8_t ac:1; // Auxiliary Carry Flag
    uint8_t pad:3; // Not used
} flags_t;

typedef struct{
    // 8-bit registers
    uint8_t A; // Primary Accumulator
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;

    // 16-bit index registers
    uint16_t SP; // Stack Pointer

    // 16-bit program counter
    uint16_t PC; // Program Counter

    flags_t flags;

    uint8_t *memory;
} i8080_t;

i8080_t* init_i8080(void);
void destroy_i8080(i8080_t *cpu);

uint8_t *read_memory(i8080_t *cpu, uint16_t address);
void write_memory(i8080_t *cpu, uint16_t address, uint8_t value);

uint8_t emulate_cycle(i8080_t *cpu);

void print_state(i8080_t *cpu);

void NOP(i8080_t *cpu);

// Opcode functions
void ADD(i8080_t *cpu, uint8_t val1, uint8_t val2);
void SUB(i8080_t *cpu, uint8_t val1, uint8_t val2);
void ANA(i8080_t *cpu, uint8_t reg);
void ORA(i8080_t *cpu, uint8_t reg);
void XRA(i8080_t *cpu, uint8_t reg);
void CMP(i8080_t *cpu, uint8_t reg);
void INR(i8080_t *cpu, uint8_t *reg);
void INX(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2);
void DCR(i8080_t *cpu, uint8_t *reg);
void DCX(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2);
void DAD(i8080_t *cpu, uint8_t reg1, uint8_t reg2);void POP(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2);
void POP_PSW(i8080_t *cpu);
void PUSH(i8080_t *cpu, uint8_t reg1, uint8_t reg2);
void PUSH_PSW(i8080_t *cpu);

void LXI(i8080_t *cpu, uint8_t *reg1, uint8_t *reg2, uint8_t high, uint8_t low);

void JMP(i8080_t *cpu, uint16_t address);
void CALL(i8080_t *cpu, uint16_t address);
uint8_t Ccc(i8080_t *cpu, bool condition, uint16_t address);
uint8_t Rcc(i8080_t *cpu, bool condition);
void Jcc(i8080_t *cpu, bool condition, uint16_t address);
void RET(i8080_t *cpu);

#endif //INTEL8080_I8080_CPU_H
