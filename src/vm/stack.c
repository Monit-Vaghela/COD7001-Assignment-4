#include <vm/stack.h>
#include <stdlib.h>

void stack_init(stack_t *s) {
    s->top = -1;
}

void stack_push(stack_t *s, int value) {
    s->data[++s->top] = value;
}

int stack_pop(stack_t *s) {
    return s->data[s->top--];
}

int stack_peek(stack_t *s) {
    return s->data[s->top];
}
