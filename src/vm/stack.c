#include <vm/stack.h>
#include <vm/metrics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int stck_error_flag = 0;

void stack_init(stack *s) {
    s->top = -1;
}

int stack_push(stack *s, int value) {
    clock_t t1 = clock();
    if (stack_isfull(s) != 0) return 1;

    s->data[++s->top] = value;
    clock_t t2 = clock();
    stack_time += (t2 - t1);
    return 0;
}

int stack_pop(stack *s) {
    clock_t t1 = clock();
    if(stack_isempty(s) != 0) {
        stck_error_flag = 1;
        return 0;
    }
    clock_t t2 = clock();
    stack_time += (t2 - t1);
    return s->data[s->top--];
}

int stack_isempty(stack *s){
    if(s->top == -1) {
        return 1;
    }
    return 0;
}

int stack_isfull(stack *s){
    if(s->top == STACK_MAX - 1) {
        return 1;
    }
    return 0;
}

void stack_print(stack *s) {
    printf("[");
    for(int i = 0; i <= s->top; i++){
        printf("%d, ", s->data[i]);
    }
    printf("]\n");
}
