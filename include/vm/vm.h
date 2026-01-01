#ifndef VM_H
#define VM_H

#include <vm/stack.h>
#include <vm/memory.h>
#include <vm/instruction.h>

typedef struct {
    instruction_t *code;
    int code_size;
    int pc;

    stack_t stack;
    stack_t call_stack;
    memory_t memory;
} vm_t;

void vm_init(vm_t *vm, instruction_t *code, int code_size);
void vm_run(vm_t *vm);

#endif
