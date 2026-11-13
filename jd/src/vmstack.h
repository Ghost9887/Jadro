#ifndef VMSTACK_H
#define VMSTACK_H

#include "value.h"

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Value** values;
    Value* top;
}VMStack;

bool initialize_stack(VMStack* stack);
void add(VMStack* stack, Value* value);
Value* pop(VMStack* stack);
Value* peek(VMStack* stack);
bool is_empty(VMStack* stack);

#endif
