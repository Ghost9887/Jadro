#include "stack.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define INITIAL_CAPACITY 2

void initialize_stack(Stack* stack) {
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->types = (ValueTag*)malloc(sizeof(ValueTag) * stack->capacity);
    if (stack->types == NULL) {
        printf("Nepodarilo sa allocovat stack\n");
        exit(1);
    }
}

void reallocate_stack(Stack* stack) {
    if (stack->count + 1 > stack->capacity) {
        stack->capacity *= 2;
        stack->types = 
            (ValueTag*)realloc(stack->types, sizeof(ValueTag) * stack->capacity);

        if (stack->types == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            exit(1);
        }
    }else if (stack->capacity > INITIAL_CAPACITY && (stack->count - 1 < stack->capacity / 2)) {
        stack->capacity /= 2;
        if (stack->capacity % 2 != 0) stack->capacity++;

        stack->types =
            (ValueTag*)realloc(stack->types, sizeof(ValueTag) * stack->capacity);

        if (stack->types == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            exit(1);
        }
    }
}

void push(Stack* stack, ValueTag type) {
    reallocate_stack(stack);
    stack->types[stack->count++] = type;
}

ValueTag pop(Stack* stack) {
    if (!is_empty(stack)) {
        return stack->types[--stack->count];
    }
    return NIL_VALUE;
}

bool is_empty(Stack* stack) {
    return stack->count == 0;
}
