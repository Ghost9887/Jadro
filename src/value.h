#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef enum {
    TAG_NIL,
    TAG_STRING,
    TAG_INTEGER,
    TAG_DOUBLE,
    TAG_BOOLEAN,
}Tag;

typedef struct {
    Tag tag;
    union {
        void* n;
        char* s;
        int i;
        double d;
        bool b;
    }as;
}Value;

#endif
