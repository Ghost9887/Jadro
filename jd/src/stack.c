#include "stack.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define INITIAL_CAPACITY 2

void initialize_stack(Stack* stack) {
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->values = (Value**)malloc(sizeof(Value*) * stack->capacity);
    if (stack->values == NULL) {
        printf("Nepodarilo sa allocovat stack\n");
        exit(1);
    }

    stack->top = NULL;
}

void reallocate_stack(Stack* stack) {
    if (stack->count + 1 > stack->capacity) {
        stack->capacity *= 2;
        stack->values = 
            (Value**)realloc(stack->values, sizeof(Value*) * stack->capacity);

        if (stack->values == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            exit(1);
        }
    }else if (stack->capacity > INITIAL_CAPACITY && (stack->count - 1 < stack->capacity / 2)) {
        stack->capacity /= 2;
        if (stack->capacity % 2 != 0) stack->capacity++;

        stack->values =
            (Value**)realloc(stack->values, sizeof(Value*) * stack->capacity);

        if (stack->values == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            exit(1);
        }
    }
}

void push(Stack* stack, Value* value) {
    reallocate_stack(stack);
    stack->values[stack->count++] = value;
}

Value* pop(Stack* stack) {
    if (!is_empty(stack)) {
        return stack->values[--stack->count];
    }
    return NULL;
}

bool is_empty(Stack* stack) {
    return stack->count == 0;
}
