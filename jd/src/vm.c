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

OpCode get_code(VM* vm);
void push_int(VM* vm);
void push_double(VM* vm);
void push_string(VM* vm);
void push_char(VM* vm);
void push_bool(VM* vm);
void push_nil(VM* vm);

void run(VM* vm) {
    bool running = true;
    while (running) {
        OpCode code = get_code(vm);
        switch (code) {
            case OP_INTEGER: push_int(vm); break;
            case OP_DOUBLE: push_double(vm); break;
            case OP_STRING: push_string(vm); break;
            case OP_CHAR: push_char(vm); break;
            case OP_BOOLEAN: push_bool(vm); break;
            case OP_NIL: push_nil(vm); break;
            case OP_PRINT: print_value(pop(&vm->stack)); break;
            case OP_END: running = false; break;
            default: printf("Uknown code\n"); break;
        }
    }
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

void push_double(VM* vm) {
    double value;
    memcpy(&value, vm->bytes + vm->ip, sizeof(double));
    vm->ip += sizeof(double);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = DOUBLE_VALUE, .as = { .d = value } };
    push(&vm->stack, new_val);
}

void push_string(VM* vm) {
    vm->ip++;
    push_int(vm);
    Value* len = pop(&vm->stack);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = STRING_VALUE, .as = 
        { .s = (String) { .len = len->as.i, .ptr = &vm->bytes[vm->ip] } } 
    };
    vm->ip += len->as.i;
    free(len);
    push(&vm->stack, new_val);
}

void push_char(VM* vm) {
    char c;
    memcpy(&c, vm->bytes + vm->ip, sizeof(char));
    vm->ip += sizeof(char);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = CHAR_VALUE, .as = { .c = c } }; 
    push(&vm->stack, new_val);
}

void push_bool(VM* vm) {
    bool b;
    memcpy(&b, vm->bytes + vm->ip, sizeof(bool));
    vm->ip += sizeof(bool);
    Value*  new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = b } };
    push(&vm->stack, new_val);
}

void push_nil(VM* vm) {
    vm->ip += 1;
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = NIL_VALUE, .as = { .n = NULL } };
    push(&vm->stack, new_val);
}
