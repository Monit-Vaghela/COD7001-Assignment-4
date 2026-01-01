#include <assembler/assembler.h>
#include <stdio.h>
#include <stdlib.h>

int assemble(const char *input_file, instruction_t **out_code) {
    (void)input_file;

    *out_code = malloc(sizeof(instruction_t));
    (*out_code)[0].opcode = OP_HALT;
    (*out_code)[0].operand = 0;

    return 1;
}
