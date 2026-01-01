#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 1024

typedef struct {
    int data[MEM_SIZE];
} memory_t;

void memory_init(memory_t *m);

#endif
