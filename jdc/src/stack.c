#include "stack.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define INITIAL_CAPACITY 2

bool initialize_stack(Stack* stack) {
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->values = (Value**)malloc(sizeof(Value*) * stack->capacity);
    if (stack->values == NULL) {
        printf("Nepodarilo sa allocovat stack\n");
        return false;
    }

    stack->top = NULL;
    return true;
}

bool reallocate_stack(Stack* stack) {
    if (stack->count + 1 > stack->capacity) {
        stack->capacity *= 2;
        stack->values = 
            (Value**)realloc(stack->values, sizeof(Value*) * stack->capacity);

        if (stack->values == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            return false;
        }
    }else if (stack->capacity > INITIAL_CAPACITY && (stack->count - 1 < stack->capacity / 2)) {
        stack->capacity /= 2;
        if (stack->capacity % 2 != 0) stack->capacity++;

        stack->values =
            (Value**)realloc(stack->values, sizeof(Value*) * stack->capacity);

        if (stack->values == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            return false;
        }
    }
    return true;
}

void push(Stack* stack, Value* value) {
    if (!reallocate_stack(stack)) exit(1);
    stack->values[stack->count++] = value;
}

Value* pop(Stack* stack) {
    if (!is_empty(stack)) {
        return stack->values[--stack->count];
    }
    return NULL;
}

Value* peek(Stack* stack) {
    return (stack->count > 0) ? stack->values[stack->count - 1] : NULL;
}

bool is_empty(Stack* stack) {
    return stack->count == 0;
}
