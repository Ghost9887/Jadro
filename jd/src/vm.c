#include "vm.h"
#include "value.h"
#include "opcode.h"
#include "runtime_error.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define JUMP_SIZE (1 + 1 + sizeof(int))

void initialize_vm(VM* vm, const char* bytes) {
    vm->ip = 0;
    vm->bytes = bytes;
    initialize_stack(&vm->stack);
    vm->current_env = 0;
    new_env(&vm->env_arr[vm->current_env], NULL);
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
static void create_new_env(VM* vm);
static void destroy_current_env(VM* vm);
static void declare_var(VM* vm);
static void assign_var(VM* vm);
static void var(VM* vm);
static void if_statement(VM* vm);
static void jump_statement(VM* vm);

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
            case OP_GREATER:
            case OP_GREATER_EQUALS:
            case OP_LESSER:
            case OP_LESSER_EQUALS:
            case OP_EQUALS_EQUALS:
            case OP_NOT_EQUALS:
                binary_op(vm, code); 
                break; 
            case OP_PRINT: print_value(pop(&vm->stack)); break;
            case OP_DECL: declare_var(vm); break;
            case OP_ASSIGN: assign_var(vm); break;
            case OP_VAR: var(vm); break;
            case OP_NEW_ENV: create_new_env(vm); break;
            case OP_DELETE_ENV: destroy_current_env(vm); break;
            case OP_IF: if_statement(vm); break;
            case OP_JUMP: jump_statement(vm); break;
            case OP_END: running = false; break;
            default: printf("Uknown code\n"); exit(1); break;
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
    Value* new_val = (Value*)malloc(sizeof(Value));
    *new_val = (Value) { .tag = INTEGER_VALUE, .as = { .i = value } };
    push(&vm->stack, new_val);
}

static void push_double(VM* vm) {
    double value;
    memcpy(&value, vm->bytes + vm->ip, sizeof(double));
    vm->ip += sizeof(double);
    Value* new_val = (Value*)malloc(sizeof(Value));
    *new_val = (Value) { .tag = DOUBLE_VALUE, .as = { .d = value } };
    push(&vm->stack, new_val);
}

static void push_string(VM* vm) {
    vm->ip++;
    push_int(vm);
    Value* len = pop(&vm->stack);
    Value* new_val = (Value*)malloc(sizeof(Value));
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
    Value* new_val = (Value*)malloc(sizeof(Value));
    *new_val = (Value) { .tag = CHAR_VALUE, .as = { .c = c } }; 
    push(&vm->stack, new_val);
}

static void push_bool(VM* vm) {
    bool b;
    memcpy(&b, vm->bytes + vm->ip, sizeof(bool));
    vm->ip += sizeof(bool);
    Value*  new_val = (Value*)malloc(sizeof(Value));
    *new_val = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = b } };
    push(&vm->stack, new_val);
}

static void push_nil(VM* vm) {
    Value* new_val = (Value*)malloc(sizeof(Value));
    *new_val = (Value) { .tag = NIL_VALUE, .as = { .n = NULL } };
    push(&vm->stack, new_val);
}

static void binary_op(VM* vm, OpCode code) {
    Value* b = pop(&vm->stack);
    Value* a = pop(&vm->stack);
    Value* res = (Value*)malloc(sizeof(Value));

    switch (code) {
        case OP_ADD: {
            if (is_int(a) && is_int(b)) {
                int num = (a->as.i + b->as.i);
                *res = (Value) { .tag = INTEGER_VALUE, .as = { .i = num } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                double num = (a->as.d + b->as.d);
                *res = (Value) { .tag = DOUBLE_VALUE, .as = { .d = num } };
                push(&vm->stack, res);
            }else if (is_string(a) && is_string(b)) {
                char* new_str = (char*)malloc(sizeof(char) * (a->as.s.len + b->as.s.len));
                memcpy(new_str, a->as.s.ptr, a->as.s.len);
                memcpy(new_str + a->as.s.len, b->as.s.ptr, b->as.s.len);
                *res = (Value) { .tag = STRING_VALUE, .as = { 
                    .s = (String) { .len = (a->as.s.len + b->as.s.len), .ptr = new_str } } 
                };
                push(&vm->stack, res);
            }
        };break;
        case OP_SUBTRACT: {
            if (is_int(a) && is_int(b)) {
                int num = (a->as.i - b->as.i);
                *res = (Value) { .tag = INTEGER_VALUE, .as = { .i = num } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                double num = (a->as.d - b->as.d);
                *res = (Value) { .tag = DOUBLE_VALUE, .as = { .d = num } };
                push(&vm->stack, res);
            }
        };break;
        case OP_MULTIPLY: {
            if (is_int(a) && is_int(b)) {
                int num = (a->as.i * b->as.i);
                *res = (Value) { .tag = INTEGER_VALUE, .as = { .i = num } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                double num = (a->as.d * b->as.d);
                *res = (Value) { .tag = DOUBLE_VALUE, .as = { .d = num } };
                push(&vm->stack, res);
            }else if (is_string(a) && is_int(b)) {
                char* new_str = (char*)malloc(sizeof(char) * (a->as.s.len * b->as.i));
                for (int i = 0; i < b->as.i; i++) {
                    memcpy(new_str + (a->as.s.len * i), a->as.s.ptr, a->as.s.len);
                }
                *res = (Value) { .tag = STRING_VALUE, .as = { .s = 
                    (String) { .len = a->as.s.len * b->as.i, .ptr = new_str } 
                } };
                push(&vm->stack, res);
            }
        };break;
        case OP_DIVIDE: {
            if (is_int(a) && is_int(b)) {
                if (b->as.i == 0) runtime_error("Delenie nulov");
                else {
                    int num = (a->as.i / b->as.i);
                    *res = (Value) { .tag = INTEGER_VALUE, .as = { .i = num } };
                    push(&vm->stack, res);
                }
            }else if (is_double(a) && is_double(b)) {
                if (b->as.i == 0) runtime_error("Delenie nulov");
                else {
                    double num = (a->as.d / b->as.d); 
                    *res = (Value) { .tag = DOUBLE_VALUE, .as = { .d = num } };
                    push(&vm->stack, res);
                }
            }
        };break;
        case OP_GREATER: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i > b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (a->as.d > b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }
        };break;
        case OP_GREATER_EQUALS: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i >= b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (a->as.d >= b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }
        };break;
        case OP_LESSER: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i < b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (a->as.d < b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }
        };break;
        case OP_LESSER_EQUALS: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i <= b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (a->as.d <= b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }
        };break;
        case OP_EQUALS_EQUALS: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i == b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (b->as.d == b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_char(a) && is_char(b)) {
                bool ans = (a->as.c == b->as.c);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_string(a) && is_string(b)) {
                bool ans = (memcmp(a->as.s.ptr, b->as.s.ptr, a->as.s.len) == 0);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_bool(a) && is_bool(b)) {
                bool ans = (a->as.b == b->as.b);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_nil(a) && is_nil(b)) {
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = true } };
                push(&vm->stack, res);
            }
        };break;
        case OP_NOT_EQUALS: {
            if (is_int(a) && is_int(b)) {
                bool ans = (a->as.i != b->as.i);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_double(a) && is_double(b)) {
                bool ans = (b->as.d != b->as.d);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_char(a) && is_char(b)) {
                bool ans = (a->as.c != b->as.c);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_string(a) && is_string(b)) {
                bool ans = (memcmp(a->as.s.ptr, b->as.s.ptr, a->as.s.len) != 0);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_bool(a) && is_bool(b)) {
                bool ans = (a->as.b != b->as.b);
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = ans } };
                push(&vm->stack, res);
            }else if (is_nil(a) && is_nil(b)) {
                *res = (Value) { .tag = BOOLEAN_VALUE, .as = { .b = false } };
                push(&vm->stack, res);
            }
        };break;
        default: break; //unreachable
    }
}

static void declare_var(VM* vm) {
    Value* name = pop(&vm->stack);
    Value* val = pop(&vm->stack);
    add_variable(&vm->env_arr[vm->current_env], &name->as.s, val);
}

static void assign_var(VM* vm) {
    Value* name = pop(&vm->stack);
    Value* val = pop(&vm->stack);
    Variable* var = get_variable(&vm->env_arr[vm->current_env], &name->as.s);
    var->value = val;
}

static void var(VM* vm) {
    Value* name = pop(&vm->stack);
    push(&vm->stack, get_variable(&vm->env_arr[vm->current_env], &name->as.s)->value);
}

static void create_new_env(VM* vm) {
    unsigned int new = ++vm->current_env;
    unsigned int parent = vm->current_env - 1;
    new_env(&vm->env_arr[new], &vm->env_arr[parent]);
}

static void destroy_current_env(VM* vm) {
    destroy_env(&vm->env_arr[vm->current_env--]);
}

static void if_statement(VM* vm) {
    Value* condition = pop(&vm->stack);
    if (condition->as.b) {
        vm->ip += JUMP_SIZE;
    }
}

static void jump_statement(VM* vm) {
    vm->ip++;
    push_int(vm);
    Value* jump_size = pop(&vm->stack);
    vm->ip += jump_size->as.i;
}
