#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assembler/assembler.h>

/* Opcodes — MUST match instruction.h */
#define OP_PUSH   0x01
#define OP_POP    0x02
#define OP_DUP    0x03

#define OP_ADD    0x10
#define OP_SUB    0x11
#define OP_MUL    0x12
#define OP_DIV    0x13
#define OP_CMP    0x14

#define OP_JMP    0x20
#define OP_JZ     0x21
#define OP_JNZ    0x22

#define OP_STORE  0x30
#define OP_LOAD   0x31

#define OP_CALL   0x40
#define OP_RET    0x41

#define OP_HALT   0xFF

#define INITIAL_CAPACITY 256

static void emit_u8(unsigned char **buf, int *size, int *cap, uint8_t v) {
    if (*size + 1 > *cap) {
        *cap *= 2;
        *buf = realloc(*buf, *cap);
        if (!*buf) {
            fprintf(stderr, "Assembler: realloc failed\n");
            exit(1);
        }
    }
    (*buf)[(*size)++] = v;
}

int assemble(const char *input_file, unsigned char **out_code) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        perror("Assembler: input file");
        return -1;
    }

    int capacity = INITIAL_CAPACITY;
    int size = 0;
    unsigned char *code = malloc(capacity);
    if (!code) {
        fclose(in);
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n")] = 0;

        /* skip empty lines and comments */
        if (line[0] == '\0' || line[0] == ';' || line[0] == '#')
            continue;

        char instr[32];
        int value;

        /* instructions with operand (1-byte operand) */
        if (sscanf(line, "%31s %d", instr, &value) == 2) {
            if (!strcmp(instr, "PUSH")) {
                emit_u8(&code, &size, &capacity, OP_PUSH);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "JMP")) {
                emit_u8(&code, &size, &capacity, OP_JMP);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "JZ")) {
                emit_u8(&code, &size, &capacity, OP_JZ);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "JNZ")) {
                emit_u8(&code, &size, &capacity, OP_JNZ);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "LOAD")) {
                emit_u8(&code, &size, &capacity, OP_LOAD);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "STORE")) {
                emit_u8(&code, &size, &capacity, OP_STORE);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else if (!strcmp(instr, "CALL")) {
                emit_u8(&code, &size, &capacity, OP_CALL);
                emit_u8(&code, &size, &capacity, (uint8_t)value);
            } else {
                fprintf(stderr, "Assembler error: unknown instruction %s\n", instr);
                free(code);
                fclose(in);
                return -1;
            }
        }
        /* instructions without operand */
        else if (sscanf(line, "%31s", instr) == 1) {
            if (!strcmp(instr, "POP")) emit_u8(&code, &size, &capacity, OP_POP);
            else if (!strcmp(instr, "DUP")) emit_u8(&code, &size, &capacity, OP_DUP);
            else if (!strcmp(instr, "ADD")) emit_u8(&code, &size, &capacity, OP_ADD);
            else if (!strcmp(instr, "SUB")) emit_u8(&code, &size, &capacity, OP_SUB);
            else if (!strcmp(instr, "MUL")) emit_u8(&code, &size, &capacity, OP_MUL);
            else if (!strcmp(instr, "DIV")) emit_u8(&code, &size, &capacity, OP_DIV);
            else if (!strcmp(instr, "CMP")) emit_u8(&code, &size, &capacity, OP_CMP);
            else if (!strcmp(instr, "RET")) emit_u8(&code, &size, &capacity, OP_RET);
            else if (!strcmp(instr, "HALT")) emit_u8(&code, &size, &capacity, OP_HALT);
            else {
                fprintf(stderr, "Assembler error: unknown instruction %s\n", instr);
                free(code);
                fclose(in);
                return -1;
            }
        }
    }

    fclose(in);

    if (out_code) {
        *out_code = code;
    } else {
        free(code);
    }

    return size;
}
