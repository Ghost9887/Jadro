#include "value.h"
#include <stdio.h>

void print_value(const Value* value) {
    switch (value->tag) {
        case NIL_VALUE: printf("nic"); break;
        case STRING_VALUE: printf("%.*s", value->as.s.len, value->as.s.ptr); break;
        case CHAR_VALUE: printf("%c", value->as.c); break;
        case BOOLEAN_VALUE: printf("%s", value->as.b ? "pravda" : "nepravda"); break;
        case INTEGER_VALUE: printf("%d", value->as.i); break;
        case DOUBLE_VALUE: printf("%f", value->as.d); break;
    }
}
