#include <vm/vm.h>
#include <stdio.h>
static void print_code(vm *vm){
    int i = 0;
    while(i < vm->code_size){
        printf("%d -> %02x\n", i, vm->code[i]);
        i++;
    }
}
void vm_init(vm *vm, unsigned char *code, int code_size) {
    printf("Initializing virtual machine\n");
    vm->code = code;
    vm->code_size = code_size;
    vm->pc = 0;
    vm->running = 1;

    stack_init(&vm->stack);
    // stack_init(&vm->call_stack);
    // memory_init(&vm->memory);
    print_code(vm);
}

void vm_run(vm *vm) {
    while(vm->running){
        unsigned char opcode = vm->code[vm->pc];
        //printf("Current Instruction : %02x\n", current_instr);
        switch (opcode){

            case PUSH:{
                (vm->pc)++;
                int operand = vm->code[vm->pc];
                stack_push(&vm->stack, operand);
                (vm->pc)++;
                break;
            }
            
            case POP:{
                stack_pop(&vm->stack);
                if(error_flag){
                    vm->running = 0;
                    break;
                }
                (vm->pc)++;
                break;
            }

            case DUP:{
                if(stack_isempty(&vm->stack) != 0){
                    printf("[VM ERROR] Cannot Duplicate Top of Stack\n");
                    vm->running = 0;
                    break;
                }
                int top_value = vm->stack.data[vm->stack.top];
                stack_push(&vm->stack, top_value);
                (vm->pc)++;
                break;
            }

            case HALT:{
                vm->running = 0;
                break;
            }

            default:
                printf("Invalid opcode %02x at PC %d\n", opcode, vm->pc);
                vm->running = 0;
                break;
            }
        stack_print(&vm->stack);
    }
    
}
