#include "vm.h"
#include "value.h"
#include "opcode.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void initialize_vm(VM* vm, const char* bytes) {
    vm->ip = 0;
    vm->bytes = bytes;
    if (!initialize_stack(&vm->stack)) exit(1);
}

bool is_at_end(VM* vm);
OpCode get_code(VM* vm);
void push_int(VM* vm);

void run(VM* vm) {
    while (!is_at_end(vm)) {
        OpCode code = get_code(vm);
        switch (code) {
            case OP_INTEGER: push_int(vm); break;
            case OP_PRINT: print_value(pop(&vm->stack)); break;
            default: printf("Uknonw code\n"); break;
        }
    }
}

//TODO: change to find the end somehow
bool is_at_end(VM* vm) {
    return vm->ip == 6; 
}

OpCode get_code(VM* vm) {
    return (OpCode)vm->bytes[vm->ip++];
}

void push_int(VM* vm) {
    int value;
    memcpy(&value, vm->bytes + vm->ip, sizeof(int));
    vm->ip += sizeof(int);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = INTEGER_VALUE, .as = { .i = value } };
    push(&vm->stack, new_val);
}
