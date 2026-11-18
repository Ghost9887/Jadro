#ifndef STACK_H
#define STACK_H

#include "value.h"

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Value** values;
    Value* top;
}Stack;

void initialize_stack(Stack* stack);
void push(Stack* stack, Value* value);
Value* pop(Stack* stack);
bool is_empty(Stack* stack);

#endif
