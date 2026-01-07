#include <stdio.h>
#include <vm/memory.h>
#include <vm/metrics.h>

int mem_error_flag = 0;

void memory_init(int *memory) {
    for (int i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
}

int memory_store(int *memory, int addr, int data){
    clock_t t1 = clock();
    if(addr >= MEM_SIZE || addr < 0){
        return 1;
    }
    memory[addr] = data;
    clock_t t2 = clock();
    memory_time += (t2 - t1);
    
    return 0;
}

int memory_load(int *memory, int addr){
    clock_t t1 = clock();
    if(addr >= MEM_SIZE || addr < 0){
        mem_error_flag = 1;
        return 0;
    }
    clock_t t2 = clock();
    memory_time += (t2 - t1);
    return memory[addr];
}
