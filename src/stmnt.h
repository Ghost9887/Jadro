#ifndef STMNT_H
#define STMNT_H

#include "expr.h"

struct StmntArr;

typedef struct {
    Expr* expr;
}ExprStmnt;

typedef enum {
    EXPR_STMNT
}StmntTag;

typedef struct {
    StmntTag tag;
    union {
        ExprStmnt exp;
    }as;
}Stmnt;

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Stmnt* stmnts;
}StmntArr;

void print_stmnt(Stmnt* stmnt);
bool initialize_stmnt_arr(StmntArr* arr);
bool reallocate_stmnt_arr(StmntArr* arr);
void free_stmnt_arr(StmntArr* arr);

#endif
