#include <vm/vm.h>
#include <stdio.h>

static int arithmatic_underflow(vm *vm){
    if(vm->stack.top < 1){
        printf("[VM ERROR / %d] Not enough Operands to perform ADD\n", vm->pc);
        vm->running = 0;
        return 1;
    }
    return 0;
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
                    printf("[VM ERROR / %d] Cannot Duplicate Top of Stack\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                int top_value = vm->stack.data[vm->stack.top];
                stack_push(&vm->stack, top_value);
                (vm->pc)++;
                break;
            }

            case ADD:{
                if(arithmatic_underflow(vm) != 0) break;

                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                stack_push(&vm->stack, a + b);
                (vm->pc)++;
                break;
            }

            case SUB:{
                if(arithmatic_underflow(vm) != 0) break;

                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                stack_push(&vm->stack, a - b);
                (vm->pc)++;
                break;
            }

            case MUL:{
                if(arithmatic_underflow(vm) != 0) break;

                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                stack_push(&vm->stack, a * b);
                (vm->pc)++;
                break;
            }

            case DIV:{
                if(arithmatic_underflow(vm) != 0) break;

                int b = stack_pop(&vm->stack);
                if(b == 0){
                    printf("[VM Error / %d] Denominator cannot be 0\n", vm->pc);
                    vm->running = 0;
                    break;
                }

                int a = stack_pop(&vm->stack);
                stack_push(&vm->stack, a / b);
                (vm->pc)++;
                break;
            }

            case CMP:{
                if(arithmatic_underflow(vm) != 0) break;
            
                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);

                if(a < b){
                    stack_push(&vm->stack, 1);
                }else{
                    stack_push(&vm->stack, 0);
                }
                (vm->pc)++;
                break;
            }

            case HALT:{
                vm->running = 0;
                break;
            }

            default:
                printf("[VM Error / %d]Invalid opcode %02x\n", vm->pc, opcode);
                vm->running = 0;
                break;
            }
        stack_print(&vm->stack);
    }
    
}
