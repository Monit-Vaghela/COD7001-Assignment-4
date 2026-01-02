#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vm/instruction.h>

int assemble(const char *input_file, unsigned char **out_code);

#endif
