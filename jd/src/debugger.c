#include "debugger.h"
#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_int_instruction(VM* vm, int offset) {
    int value;
    memcpy(&value, vm->bytes + offset, sizeof(int));
    printf("OP_INTEGER | %d\n", value);
    offset += sizeof(int);
    return offset;
}

int print_double_instruction(VM* vm, int offset) {
    double value;
    memcpy(&value, vm->bytes + offset, sizeof(double));
    printf("OP_DOUBLE | %f\n", value);
    offset += sizeof(int);
    return offset;
}

int print_string_instruction(VM* vm, int offset) {
    int len;
    memcpy(&len, vm->bytes + (++offset), sizeof(int));
    offset++;
    printf("OP_STRING | %*.s\n", len, vm->bytes + offset);
    offset += sizeof(int) + 1 + len;
    return offset;
}

int print_char_instruction(VM* vm, int offset) {
    char c;
    memcpy(&c, vm->bytes + offset, sizeof(char));
    printf("OP_CHAR | %c\n", c);
    return ++offset;
}

int print_bool_instruction(VM* vm, int offset) {
    bool b;
    memcpy(&b, vm->bytes + offset, sizeof(bool));
    printf("OP_BOOL | %b\n", b);
    return ++offset;
}

int print_simple_instruction(const char* name, int offset) {
    printf("%s\n", name);
    return ++offset;
}

int print_end_instruction() {
    printf("OP_END\n");
    return -1;
}

int print_instruction(VM* vm, int offset) {
    OpCode code = (OpCode)vm->bytes[offset];

    switch (code) {
        case OP_NEGATE: return print_simple_instruction("OP_NEGATE", offset);
        case OP_INTEGER: return print_int_instruction(vm, ++offset);
        case OP_DOUBLE: return print_double_instruction(vm, ++offset);
        case OP_STRING: return print_string_instruction(vm, ++offset);
        case OP_CHAR: return print_char_instruction(vm, ++offset);
        case OP_BOOLEAN: return print_bool_instruction(vm, ++offset);
        case OP_NIL: return print_simple_instruction("OP_NIL", offset);
        case OP_VAR: return print_simple_instruction("OP_VAR", offset);
        case OP_DECL: return print_simple_instruction("OP_DECL", offset);
        case OP_ASSIGN: return print_simple_instruction("OP_ASSIGN", offset);
        case OP_ADD: return print_simple_instruction("OP_ADD", offset);
        case OP_SUBTRACT: return print_simple_instruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY: return print_simple_instruction("OP_MULTIPLY", offset);
        case OP_DIVIDE: return print_simple_instruction("OP_DIVIDE", offset);
        case OP_MODULO: return print_simple_instruction("OP_MODULO", offset);
        case OP_GREATER: return print_simple_instruction("OP_GREATER", offset);
        case OP_GREATER_EQUALS: return print_simple_instruction("OP_GREATER_EQUALS", offset);
        case OP_LESSER: return print_simple_instruction("OP_LESSER", offset);
        case OP_LESSER_EQUALS: return print_simple_instruction("OP_LESSER_EQUALS", offset);
        case OP_EQUALS_EQUALS: return print_simple_instruction("OP_EQUALS_EQUALS", offset);
        case OP_NOT_EQUALS: return print_simple_instruction("OP_NOT_EQUALS", offset);
        case OP_PRINT: return print_simple_instruction("OP_PRINT", offset);
        case OP_NEW_ENV: return print_simple_instruction("OP_NEW_ENV", offset);
        case OP_DELETE_ENV: return print_simple_instruction("OP_DELETE_ENV", offset);
        case OP_END: return print_end_instruction();
        default: printf("UKNOWN CODE\n"); return ++offset;
    }
}

void debug(VM* vm) {
    int offset = 0;
    printf("----DEBUG_START----\n");
    while (offset >= 0) {
        offset = print_instruction(vm, offset);
    }
    printf("----DEBUG_END----\n");
}
