#include <vm/stack.h>
#include <stdlib.h>
#include <stdio.h>

int stck_error_flag = 0;

void stack_init(stack *s) {
    s->top = -1;
}

int stack_push(stack *s, int value) {
    if (stack_isfull(s) != 0) return 1;

    s->data[++s->top] = value;
    return 0;
}

int stack_pop(stack *s) {
    if(stack_isempty(s) != 0) {
        stck_error_flag = 1;
        return 0;
    }
    return s->data[s->top--];
}

int stack_isempty(stack *s){
    if(s->top == -1) {
        printf("[STACK ERROR]: Stack is empty\n");
        return 1;
    }
    return 0;
}

int stack_isfull(stack *s){
    if(s->top == STACK_MAX - 1) {
        printf("[STACK ERROR]: Stack is FULL\n");
        return 1;
    }
    return 0;
}

void stack_print(stack *s) {
    if (stack_isempty(s) != 0) return;

    printf("[");
    for(int i = 0; i <= s->top; i++){
        printf("%d, ", s->data[i]);
    }
    printf("]\n");
}
