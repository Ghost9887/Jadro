#ifndef STMNT_H
#define STMNT_H

#include "expr.h"

struct StmntArr;

typedef struct {
    Expr* expr;
}ExprStmnt;

typedef struct {
    struct StmntArr* stmnt_arr;
}BlockStmnt;

typedef enum {
    STMNT_TAG_EXPR,
    STMTN_TAG_BLOCK
}StmntTag;

typedef struct {
    StmntTag tag;
    union {
        ExprStmnt* e;
        BlockStmnt* b;
    }as;
}Stmnt;

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Stmnt* stmnts;
}StmntArr;

bool initialize_stmnt_arr(StmntArr* arr);
bool reallocate_stmnt_arr(StmntArr* arr);
void free_stmnt_arr(StmntArr* arr);

#endif
