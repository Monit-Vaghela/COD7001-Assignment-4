#ifndef STACK_H
#define STACK_H

#define STACK_MAX 1024

typedef struct {
    int data[STACK_MAX];
    int top;
} stack_t;

void stack_init(stack_t *s);
void stack_push(stack_t *s, int value);
int  stack_pop(stack_t *s);
int  stack_peek(stack_t *s);

#endif
