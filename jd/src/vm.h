#ifndef VM_H
#define VM_H

typedef enum {
    OP_NEGATE,
    OP_INTEGER,
    OP_DOUBLE,
    OP_STRING,
    OP_CHAR,
    OP_BOOLEAN,
    OP_NIL,
    OP_VAR,
    OP_DECL,
    OP_ASSIGN,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_GREATER,
    OP_GREATER_EQUALS,
    OP_LESSER,
    OP_LESSER_EQUALS,
    OP_EQUALS_EQUALS,
    OP_NOT_EQUALS,
    OP_PRINT,
    OP_NEW_ENV,
    OP_DELETE_ENV
}OpCode;

typedef struct {
    const char* content;
    unsigned int ip;
}VM;

void initialize_vm(VM* vm, const char* content);
void run(VM* vm);

#endif
