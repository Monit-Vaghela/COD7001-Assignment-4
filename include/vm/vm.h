#ifndef VM_H
#define VM_H

#include <vm/stack.h>
#include <vm/memory.h>
#include <vm/instruction.h>

typedef struct {
    unsigned char *code;
    int code_size;
    int pc;
    int running;

    stack stack;
    stack call_stack;
    int memory[MEM_SIZE];
} vm;

void vm_init(vm *vm, unsigned char *code, int code_size);
void vm_run(vm *vm, int debug);

#endif
