#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef struct {
    unsigned int len;
    const char* ptr;
}String;

typedef enum {
    NIL_VALUE,
    STRING_VALUE,
    CHAR_VALUE,
    INTEGER_VALUE,
    DOUBLE_VALUE,
    BOOLEAN_VALUE,
}ValueTag;

typedef struct {
    ValueTag tag;
    union {
        void* n;
        String s;
        char c;
        int i;
        double d;
        bool b;
    }as;
}Value;

void print_value(const Value* value);

#endif
