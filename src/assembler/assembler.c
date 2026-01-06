#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assembler/assembler.h>
#include <vm/instruction.h>   // IMPORTANT: opcode source

/* ================= LABEL TABLE ================= */

#define MAX_LABELS 128

typedef struct {
    char name[32];
    int addr;
} label_t;

static label_t labels[MAX_LABELS];
static int label_count = 0;

/* ================= CONSTANTS ================= */

#define INITIAL_CAPACITY 256

/* ================= FILE OUTPUT ================= */

static int write_bin(const char *asm_file,
                     unsigned char *code,
                     int size)
{
    char out_name[256];
    const char *dot = strrchr(asm_file, '.');

    if (dot && strcmp(dot, ".asm") == 0) {
        size_t len = dot - asm_file;
        snprintf(out_name, sizeof(out_name), "%.*s.bin",
                 (int)len, asm_file);
    } else {
        snprintf(out_name, sizeof(out_name), "%s.bin", asm_file);
    }

    FILE *out = fopen(out_name, "wb");
    if (!out) {
        perror("Assembler: output file");
        return -1;
    }

    fwrite(code, 1, size, out);
    fclose(out);
    return 0;
}

/* ================= BYTE EMISSION ================= */

static void emit_u8(unsigned char **buf, int *size, int *cap, uint8_t v)
{
    if (*size + 1 > *cap) {
        *cap *= 2;
        *buf = realloc(*buf, *cap);
        if (!*buf) {
            printf("[Assembler Error] Memory allocation failed\n");
            exit(1);
        }
    }
    (*buf)[(*size)++] = v;
}

/* break int32 → 4 × uint8 */
static void emit_i32(unsigned char **buf, int *size, int *cap, int32_t v)
{
    for (int byte = 3; byte >= 0; byte--) {
        uint8_t part = (v >> (byte * 8)) & 0xFF;
        emit_u8(buf, size, cap, part);
    }
}

/* ================= LABEL LOOKUP ================= */

static int find_label(const char *name)
{
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].addr;
    }
    return -1;
}

/* ================= ASSEMBLER ================= */

int assemble(const char *input_file, unsigned char **out_code)
{
    FILE *in = fopen(input_file, "r");
    if (!in) {
        perror("Assembler: input file");
        return -1;
    }

    char line[256];
    char instr[32], operand[32];

    /* ---------- PASS 1: LABEL ADDRESSES ---------- */

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
            pc += 5;   // opcode + int32
        else
            pc += 1;   // opcode only
    }

    rewind(in);

    /* ---------- PASS 2: BYTECODE ---------- */

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
            continue;

        /* instruction with operand */
        if (sscanf(line, "%31s %31s", instr, operand) == 2) {

            int value;
            if (operand[0] >= '0' && operand[0] <= '9') {
                value = atoi(operand);
            } else {
                value = find_label(operand);
                if (value < 0) {
                    printf("[Assembler Error] Unknown label: %s\n", operand);
                    free(code);
                    fclose(in);
                    return -1;
                }
            }

            if (!strcmp(instr, "PUSH")) {
                emit_u8(&code, &size, &capacity, PUSH);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "JMP")) {
                emit_u8(&code, &size, &capacity, JMP);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "JZ")) {
                emit_u8(&code, &size, &capacity, JZ);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "JNZ")) {
                emit_u8(&code, &size, &capacity, JNZ);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "LOAD")) {
                emit_u8(&code, &size, &capacity, LOAD);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "STORE")) {
                emit_u8(&code, &size, &capacity, STORE);
                emit_i32(&code, &size, &capacity, value);
            } else if (!strcmp(instr, "CALL")) {
                emit_u8(&code, &size, &capacity, CALL);
                emit_i32(&code, &size, &capacity, value);
            } else {
                printf("[Assembler Error] Unknown instruction: %s\n", instr);
                free(code);
                fclose(in);
                return -1;
            }
        }
        /* instruction without operand */
        else if (sscanf(line, "%31s", instr) == 1) {
            if (!strcmp(instr, "POP")) emit_u8(&code, &size, &capacity, POP);
            else if (!strcmp(instr, "DUP")) emit_u8(&code, &size, &capacity, DUP);
            else if (!strcmp(instr, "ADD")) emit_u8(&code, &size, &capacity, ADD);
            else if (!strcmp(instr, "SUB")) emit_u8(&code, &size, &capacity, SUB);
            else if (!strcmp(instr, "MUL")) emit_u8(&code, &size, &capacity, MUL);
            else if (!strcmp(instr, "DIV")) emit_u8(&code, &size, &capacity, DIV);
            else if (!strcmp(instr, "CMP")) emit_u8(&code, &size, &capacity, CMP);
            else if (!strcmp(instr, "RET")) emit_u8(&code, &size, &capacity, RET);
            else if (!strcmp(instr, "HALT")) emit_u8(&code, &size, &capacity, HALT);
            else {
                printf("[Assembler Error] Unknown instruction: %s\n", instr);
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
        write_bin(input_file, code, size);
        free(code);
    }

    return size;
}
