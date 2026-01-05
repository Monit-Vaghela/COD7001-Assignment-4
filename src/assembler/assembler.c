#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assembler/assembler.h>

#define MAX_LABELS 128

typedef struct {
    char name[32];
    int addr;
} label_t;

static label_t labels[MAX_LABELS];
static int label_count = 0;

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

static int find_label(const char *name) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].addr;
    }
    return -1;
}

int assemble(const char *input_file, unsigned char **out_code) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        perror("Assembler: input file");
        return -1;
    }

    char line[256];
    char instr[32], operand[32];

    /* -------- PASS 1: collect labels -------- */
    int pc = 0;
    label_count = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
            continue;

        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            strcpy(labels[label_count].name, line);
            labels[label_count].addr = pc;
            label_count++;
            continue;
        }

        if (sscanf(line, "%31s %31s", instr, operand) == 2)
            pc += 2;   // opcode + operand
        else
            pc += 1;   // opcode only
    }

    rewind(in);

    /* -------- PASS 2: generate bytecode -------- */
    int capacity = INITIAL_CAPACITY;
    int size = 0;
    unsigned char *code = malloc(capacity);
    if (!code) {
        fclose(in);
        return -1;
    }

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
            continue;

        if (strchr(line, ':'))
            continue;   // skip label-only lines

        /* instruction with operand */
        if (sscanf(line, "%31s %31s", instr, operand) == 2) {

            int value;
            if (operand[0] >= '0' && operand[0] <= '9') {
                value = atoi(operand);
            } else {
                value = find_label(operand);
                if (value < 0) {
                    fprintf(stderr, "Assembler error: unknown label %s\n", operand);
                    free(code);
                    fclose(in);
                    return -1;
                }
            }

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
        /* instruction without operand */
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

    if (out_code)
        *out_code = code;
    else
        free(code);

    return size;
}
