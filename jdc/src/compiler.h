#ifndef COMPILER_H
#define COMPILER_H

#include "stmnt.h"
#include <stdio.h>

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
    OP_DELETE_ENV,
    OP_END
}OpCode;

typedef struct {
    StmntArr* stmnt_arr;
    FILE* output_file;
}Compiler;

void initialize_compiler(Compiler* compiler, StmntArr* stmnt_arr, const char* output_file_name);
void compile(Compiler* compiler);

#endif
