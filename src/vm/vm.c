#include <vm/vm.h>
#include <stdio.h>

void vm_init(vm_t *vm, instruction_t *code, int code_size) {
    vm->code = code;
    vm->code_size = code_size;
    vm->pc = 0;

    stack_init(&vm->stack);
    stack_init(&vm->call_stack);
    memory_init(&vm->memory);
}

void vm_run(vm_t *vm) {
    while (vm->pc < vm->code_size) {
        instruction_t instr = vm->code[vm->pc];

        switch (instr.opcode) {
            case OP_PUSH:
                stack_push(&vm->stack, instr.operand);
                break;

            case OP_ADD: {
                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                stack_push(&vm->stack, a + b);
                break;
            }

            case OP_HALT:
                return;

            default:
                printf("Invalid opcode: %d\n", instr.opcode);
                return;
        }

        vm->pc++;
    }
}
