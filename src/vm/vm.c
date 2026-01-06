#include <vm/vm.h>
#include <vm/metrics.h>
#include <stdio.h>
#include <time.h>

static int vm_error_flag = 0;
static unsigned char mem_initialized[MEM_SIZE];



static int arithmatic_underflow(vm *vm, char *operator){
    if(vm->stack.top < 1){
        printf("[VM ERROR / %d]: Not enough Operands on stack to perform %s\n", vm->pc, operator);
        vm->running = 0;
        return 1;
    }
    return 0;
}

static int check_loop(vm *vm, char *loop_type){
    if (vm->pc + 1 >= vm->code_size) {
        printf("[VM Error / %d]: %s requires another byte\n", vm->pc, loop_type);
        vm->running = 0;
        vm_error_flag = 1;
        return 1;
    }

    int addr = vm->code[vm->pc + 1];

    if (addr >= vm->code_size) {
        printf("[VM Error / %d]: Out of Bound Address\n", vm->pc);
        vm->running = 0;
        vm_error_flag = 1;
        return 1;
    }
    return addr;
}

static int check_next_byte(vm *vm, char *opcode){
    if(vm->pc + 1 >= vm->code_size){
        printf("[VM Error / %d]: %s Requires 2 bytes\n", vm->pc, opcode);
        vm->running = 0;
        return 1;
    }
    return 0;
}

static void print_vm_state(vm *vm, unsigned char opcode){
    printf("\n====================================================\n");
    printf("                 VIRTUAL MACHINE STATE               \n");
    printf("====================================================\n");

    printf("PC      : %-4d\n", vm->pc);
    printf("OPCODE  : 0x%02X\n", opcode);
    printf("RUNNING : %s\n", vm->running ? "YES" : "NO");

    printf("\n---------------------- STACK ------------------------\n");
    stack_print(&vm->stack);

    printf("\n------------------- CALL STACK ----------------------\n");
    stack_print(&vm->call_stack);

    printf("\n---------------------- MEMORY -----------------------\n");
    int found = 0;
    for(int i = 0; i < MEM_SIZE; i++){
        if(mem_initialized[i]){
            printf("[ADDR %3d] = %d\n", i, vm->memory[i]);
            found = 1;
        }
    }
    if(!found) printf("NO Initialized Memory\n");

    printf("====================================================\n");
}


void vm_init(vm *vm, unsigned char *code, int code_size){
    printf("Initializing virtual machine\n");
    vm->code = code;
    vm->code_size = code_size;
    vm->pc = 0;
    vm->running = 1;

    stack_init(&vm->stack);
    stack_init(&vm->call_stack);
    memory_init(vm->memory);

    for(int i = 0; i < MEM_SIZE; i++)
        mem_initialized[i] = 0;
}

void vm_run(vm *vm, int debug, int benchmark) {
    while(vm->running && vm->pc < vm->code_size){
        unsigned char opcode = vm->code[vm->pc];

        if(benchmark) instruction_count++;
        if(debug) print_vm_state(vm, opcode);
        //printf("Current Instruction : %02x\n", current_instr);
        switch (opcode){

            case PUSH:{
                if(check_next_byte(vm, "PUSH") != 0 ) break;
    
                (vm->pc)++;
                int operand = vm->code[vm->pc];
                if(stack_push(&vm->stack, operand) != 0){
                    printf("[VM Error / %d]: Stack is Full Cannot Push\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                (vm->pc)++;
                break;
            }
            
            case POP:{
                stack_pop(&vm->stack);
                if(stck_error_flag){
                    printf("[VM Error / %d]: Stack Is Empty Cannot Pop\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                (vm->pc)++;
                break;
            }

            case DUP:{
                if(stack_isempty(&vm->stack) != 0){
                    printf("[VM ERROR / %d]: Cannot Duplicate Top of Stack\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                int top_value = vm->stack.data[vm->stack.top];
                stack_push(&vm->stack, top_value);
                (vm->pc)++;
                break;
            }

            case ADD:{
                if(arithmatic_underflow(vm, "ADD") != 0) break;
                
                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                clock_t t1 = clock();
                int c = a + b;
                clock_t t2 = clock();
                alu_time += (t2 - t1);
                stack_push(&vm->stack, c);
                (vm->pc)++;
                break;
            }

            case SUB:{
                if(arithmatic_underflow(vm, "SUB") != 0) break;

                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                clock_t t1 = clock();
                int c = a - b;
                clock_t t2 = clock();
                alu_time += (t2 - t1);
                stack_push(&vm->stack, c);
                (vm->pc)++;
                break;
            }

            case MUL:{
                if(arithmatic_underflow(vm, "MUL") != 0) break;

                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);

                clock_t t1 = clock();
                int c = a * b;
                clock_t t2 = clock();
                alu_time += (t2 - t1);
                stack_push(&vm->stack, c);
                (vm->pc)++;
                break;
            }

            case DIV:{
                if(arithmatic_underflow(vm, "DIV") != 0) break;

                int b = stack_pop(&vm->stack);
                if(b == 0){
                    printf("[VM Error / %d] Denominator cannot be 0\n", vm->pc);
                    vm->running = 0;
                    break;
                }

                int a = stack_pop(&vm->stack);
                clock_t t1 = clock();
                int c = a / b;
                clock_t t2 = clock();
                alu_time += (t2 - t1);
                stack_push(&vm->stack, c);
                (vm->pc)++;
                break;
            }

            case CMP:{
                if(arithmatic_underflow(vm, "CMP") != 0) break;
            
                int b = stack_pop(&vm->stack);
                int a = stack_pop(&vm->stack);
                clock_t t1 = clock();
                if(a < b){
                    clock_t t2 = clock();
                    alu_time += (t2 - t1);
                    stack_push(&vm->stack, 1);
                }else{
                    clock_t t2 = clock();
                    alu_time += (t2 - t1);
                    stack_push(&vm->stack, 0);
                }
                (vm->pc)++;
                break;
            }

            case JMP:{
                clock_t t1 = clock();
                int addr = check_loop(vm, "JMP");
                if(vm_error_flag != 0) break;
                if (debug) printf("JUMPING TO %d\n", addr);
                vm->pc = addr;
                clock_t t2 = clock();
                control_time += (t2 - t1);
                break;     
            }

            case JZ:{
                clock_t t1 = clock();
                int top = stack_pop(&vm->stack);
                if (stck_error_flag) {
                    printf("[VM Error / %d]: Stack is empty\n", vm->pc);
                    vm->running = 0;
                    break;
                }

                int addr = check_loop(vm, "JZ");
                if(vm_error_flag != 0) break;

                if (top == 0) {
                    if (debug) printf("JUMPING TO %d\n", addr);
                    vm->pc = addr;
                } else {
                    vm->pc += 2;
                }
                clock_t t2 = clock();
                control_time += (t2 - t1);
                break;
            }

            case JNZ:{
                clock_t t1 = clock();
                int top = stack_pop(&vm->stack);
                if (stck_error_flag) {
                    printf("[VM Error / %d]: Stack is empty\n", vm->pc);
                    vm->running = 0;
                    break;
                }

                int addr = check_loop(vm, "JNZ");
                if(vm_error_flag != 0) break;

                if (top != 0) {
                    if (debug) printf("JUMPING TO %d\n", addr);
                    vm->pc = addr;
                } else {
                    vm->pc += 2;
                }
                clock_t t2 = clock();
                control_time += (t2 - t1);
                break;
            }

            case STORE:{
                if(check_next_byte(vm, "STORE") != 0) break;
            
                int addr = vm->code[vm->pc + 1];
                int top = stack_pop(&vm->stack);
                if (stck_error_flag) {
                    printf("[VM Error / %d]: Stack is empty\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                if(memory_store(vm->memory, addr, top) != 0){
                    vm->running = 0;
                    break;
                }
                mem_initialized[addr] = 1;

                if (debug) printf("DATA STORED AT %d is %d\n", addr, top);
                (vm->pc) += 2;
                
                break;
            }

            case LOAD:{
                if(check_next_byte(vm, "LOAD") != 0) break;
                int addr = vm->code[vm->pc + 1];

                int data = memory_load(vm->memory, addr);
                if(mem_error_flag != 0){
                    vm->running = 0;
                    break;
                }
                stack_push(&vm->stack, data);

                if (debug) printf("DATA %d Loaded from %d\n", data, addr);
                (vm->pc) += 2;
                break;
            }

            case CALL:{
                clock_t t1 = clock();
                if(check_next_byte(vm, "CALL") != 0) break;

                int addr = vm->code[vm->pc + 1];
                if(addr >= vm->code_size){
                    printf("[VM Error / %d]: Call Address %d is out of bounds\n", vm->pc, addr);
                    vm->running = 0;
                    break;
                }

                if (stack_push(&vm->call_stack, vm->pc + 2) != 0) {
                    printf("[VM Error / %d]: Call Stack is Full\n", vm->pc);
                    vm->running = 0;
                    break;
                }

                vm->pc = addr;
                clock_t t2 = clock();
                control_time += (t2 - t1);
                break;
            }

            case RET:{
                clock_t t1 = clock();
                int ret_addr = stack_pop(&vm->call_stack);
                if(stck_error_flag != 0){
                    printf("[VM Error / %d]: No Return Value present in call stack\n", vm->pc);
                    vm->running = 0;
                    break;
                }
                if(ret_addr >= vm->code_size){
                    printf("[VM Error / %d]: Return Address %d is out of Bounds\n", vm->pc, ret_addr);
                    vm->running = 0;
                    break;
                }
                vm->pc = ret_addr;
                clock_t t2 = clock();
                control_time += (t2 - t1);
                break;
            }

            case HALT:{
                vm->running = 0;
                break;
            }

            default:
                printf("[VM Error / %d] : Invalid opcode %02x\n", vm->pc, opcode);
                vm->running = 0;
                break;
            }
    }
    printf("\n\nFINAL VM STATE\n");
    print_vm_state(vm, 0xFF);
}
