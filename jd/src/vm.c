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

static OpCode get_code(VM* vm);
static bool is_int(Value* value);
static bool is_double(Value* value);
static bool is_string(Value* value);
static bool is_char(Value* value);
static bool is_bool(Value* value);
static bool is_nil(Value* value);
static void push_int(VM* vm);
static void push_double(VM* vm);
static void push_string(VM* vm);
static void push_char(VM* vm);
static void push_bool(VM* vm);
static void push_nil(VM* vm);
static void binary_op(VM* vm, OpCode code);

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
            case OP_ADD:
            case OP_SUBTRACT: 
            case OP_MULTIPLY:
            case OP_DIVIDE:
            case OP_MODULO:
                binary_op(vm, code); 
                break; 
            case OP_PRINT: print_value(pop(&vm->stack)); break;
            case OP_END: running = false; break;
            default: printf("Uknown code\n"); break;
        }
    }
}

static OpCode get_code(VM* vm) {
    return (OpCode)vm->bytes[vm->ip++];
}

static bool is_int(Value* value) {
    return value->tag == INTEGER_VALUE;
}
static bool is_double(Value* value) {
    return value->tag == DOUBLE_VALUE;
}

static bool is_string(Value* value) {
    return value->tag == STRING_VALUE;
}

static bool is_char(Value* value) {
    return value->tag == CHAR_VALUE;
}

static bool is_bool(Value* value) {
    return value->tag == BOOLEAN_VALUE;
}

static bool is_nil(Value* value) {
    return value->tag == NIL_VALUE;
}

static void push_int(VM* vm) {
    int value;
    memcpy(&value, vm->bytes + vm->ip, sizeof(int));
    vm->ip += sizeof(int);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = INTEGER_VALUE, .as = { .i = value } };
    push(&vm->stack, new_val);
}

static void push_double(VM* vm) {
    double value;
    memcpy(&value, vm->bytes + vm->ip, sizeof(double));
    vm->ip += sizeof(double);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = DOUBLE_VALUE, .as = { .d = value } };
    push(&vm->stack, new_val);
}

static void push_string(VM* vm) {
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

static void push_char(VM* vm) {
    char c;
    memcpy(&c, vm->bytes + vm->ip, sizeof(char));
    vm->ip += sizeof(char);
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = CHAR_VALUE, .as = { .c = c } }; 
    push(&vm->stack, new_val);
}

static void push_bool(VM* vm) {
    bool b;
    memcpy(&b, vm->bytes + vm->ip, sizeof(bool));
    vm->ip += sizeof(bool);
    Value*  new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = b } };
    push(&vm->stack, new_val);
}

static void push_nil(VM* vm) {
    vm->ip += 1;
    Value* new_val = malloc(sizeof(Value));
    *new_val = (Value) { .tag = NIL_VALUE, .as = { .n = NULL } };
    push(&vm->stack, new_val);
}

static void binary_op(VM* vm, OpCode code) {
    Value* b = pop(&vm->stack);
    Value* a = pop(&vm->stack);
    Value* res = malloc(sizeof(Value));

    switch (code) {
        case OP_ADD: {
            if (is_int(b) && is_int(a)) {
                int num = (a->as.i + b->as.i);
                *res = (Value) { .tag = INTEGER_VALUE, .as = { .i = num } };
                push(&vm->stack, res);
            }else if (is_double(b) && is_double(b)) {
                double num = (a->as.d + b->as.d);
                *res = (Value) { .tag = DOUBLE_VALUE, .as = { .d = num } };
                push(&vm->stack, res);
            }            
            //TODO: ERROR
        };break;
        case OP_SUBTRACT:
        case OP_MULTIPLY:
        case OP_DIVIDE:
        case OP_GREATER:
        case OP_GREATER_EQUALS:
        case OP_LESSER:
        case OP_LESSER_EQUALS:
        case OP_EQUALS_EQUALS:
        case OP_NOT_EQUALS:
        default: break; //unreachable
    }
}
