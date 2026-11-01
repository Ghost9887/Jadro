#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef struct {
    unsigned int len;
    const char* ptr;
}String;

typedef enum {
    TAG_NIL,
    TAG_STRING,
    TAG_CHAR,
    TAG_INTEGER,
    TAG_DOUBLE,
    TAG_BOOLEAN,
}Tag;

typedef struct {
    Tag tag;
    union {
        void* n;
        String s;
        char c;
        int i;
        double d;
        bool b;
    }as;
}Value;

#endif
