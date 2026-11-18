#ifndef STACK_H
#define STACK_H

#include "value.h"

typedef struct {
    unsigned int count;
    unsigned int capacity;
    ValueTag* types;
}Stack;

void initialize_stack(Stack* stack);
void push(Stack* stack, ValueTag type);
ValueTag pop(Stack* stack);
bool is_empty(Stack* stack);

#endif
