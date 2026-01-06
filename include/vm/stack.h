#ifndef STACK_H
#define STACK_H

#define STACK_MAX 256

typedef struct {
    int data[STACK_MAX];
    int top;
} stack;
extern int stck_error_flag;

void stack_init(stack *s);
int stack_push(stack *s, int value);
int stack_pop(stack *s);
int stack_isempty(stack *s);
int stack_isfull(stack *s);
void stack_print(stack *s);

#endif
