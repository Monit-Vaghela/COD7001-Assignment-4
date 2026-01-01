#include <vm/memory.h>

void memory_init(memory_t *m) {
    for (int i = 0; i < MEM_SIZE; i++)
        m->data[i] = 0;
}
