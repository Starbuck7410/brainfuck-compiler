#include "../include/stack.h"

stack_T stack_init(){
    stack_T result = {.head = 0};
    result.data = malloc(sizeof(int) * MAX_STACK_SIZE);
    return result;
}

void stack_destroy(stack_T * stack){
    free(stack->data);
    stack->data = NULL;
    stack->head = 0;
}

void stack_push(stack_T * stack, int value){
    stack->data[stack->head] = value;
    stack->head++;
}

int stack_pop(stack_T * stack){
    stack->head--;
    return stack->data[stack->head];
}