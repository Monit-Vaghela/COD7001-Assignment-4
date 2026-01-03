#include <stdio.h>
#include <vm/memory.h>

int mem_error_flag = 0;

void memory_init(int *memory) {
    for (int i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
}

int memory_store(int *memory, int addr, int data){
    if(addr >= MEM_SIZE || addr < 0){
        printf("Out of bounds memory address\n");
        return 1;
    }
    memory[addr] = data;
    return 0;
}

int memory_load(int *memory, int addr){
    if(addr >= MEM_SIZE || addr < 0){
        printf("Out of bounds memory address\n");
        mem_error_flag = 1;
        return 0;
    }
    return memory[addr];
}
