#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vm/instruction.h>

int assemble(const char *input_file, instruction_t **out_code);

#endif
