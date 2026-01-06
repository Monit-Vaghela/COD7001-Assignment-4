#ifndef VM_H
#define VM_H

#include <vm/stack.h>
#include <vm/memory.h>
#include <vm/instruction.h>
#include <time.h>

typedef struct {
    unsigned char *code;
    int code_size;
    int pc;
    int running;

    stack stack;
    stack call_stack;
    int memory[MEM_SIZE];
} vm;

extern clock_t stack_time;
extern clock_t memory_time;
void vm_init(vm *vm, unsigned char *code, int code_size);
void vm_run(vm *vm, int debug, int benchmark);

#endif
