#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 1024

extern int mem_error_flag;
void memory_init(int *memory);
int memory_load(int *memory, int addr);
int memory_store(int *memory, int addr, int data);

#endif
