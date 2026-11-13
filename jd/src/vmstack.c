#include "vmstack.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool initialize_stack(VMStack* stack) {
    stack->count = 0;
    stack->capacity = 2;
    stack->values = (Value**)malloc(sizeof(Value*) * stack->capacity);
    if (stack->values == NULL) {
        printf("Nepodarilo sa allocovat stack\n");
        return false;
    }

    stack->top = NULL;
    return true;
}

bool reallocate_stack(VMStack* stack) {
    if (stack->count + 1 > stack->capacity) {
        stack->capacity *= 2;
        stack->values = 
            (Value**)realloc(stack->values, sizeof(Value*) * stack->capacity);

        if (stack->values == NULL) {
            printf("Nepodarilo sa reallocovat stack\n");
            return false;
        }
    }
    return true;
}

void add(VMStack* stack, Value* value) {
    reallocate_stack(stack);
    stack->values[stack->count++] = value;
}

Value* pop(VMStack* stack) {
    if (!is_empty(stack)) {
        return stack->values[--stack->count];
    }
    return NULL;
}

Value* peek(VMStack* stack) {
    return (stack->count > 0) ? stack->values[stack->count] : NULL;
}

bool is_empty(VMStack* stack) {
    return stack->count == 0;
}
