#include "value.h"

char* value_to_string(Value* value) {
    switch (value->tag) {
        case TAG_NIL: return "NIL";
        case TAG_STRING: return sprintf(value->as.s.ptr, "%.*s", value->as.s.len, value->as.s.ptr);
        case TAG_INT: return;
        case TAG_DOUBLE: return;
        case TAG_BOOLEAN: return value->as.b ? "Pravda" : "Nepravda";
    }
}
