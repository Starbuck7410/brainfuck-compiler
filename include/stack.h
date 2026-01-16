#include <stdlib.h>
#define MAX_STACK_SIZE 1024

typedef struct {
    size_t head;
    int * data;
} stack_T;

stack_T stack_init();

void stack_destroy(stack_T * stack);

void stack_push(stack_T * stack, int value);

int stack_pop(stack_T * stack);