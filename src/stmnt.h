#ifndef STMNT_H
#define STMNT_H

#include "expr.h"

typedef struct StmntArr StmntArr;

typedef struct {
    Expr* expr;
}ExprStmnt;

typedef struct {
    Expr* expr;
}PrintStmnt;

typedef struct {
    StmntArr* stmnts;   
}BlockStmnt;

typedef struct {
    String name;
    Expr* expr;
}DeclStmnt;

typedef struct {
    String name;
    Expr* expr;
}AssignStmnt;

typedef enum {
    EXPR_STMNT,
    PRINT_STMNT,
    BLOCK_STMNT,
    DECL_STMNT,
    ASSIGN_STMNT,
}StmntTag;

typedef struct {
    StmntTag tag;
    union {
        ExprStmnt exp;
        PrintStmnt pri;
        BlockStmnt blo;
        DeclStmnt dec;
        AssignStmnt ass;
    }as;
}Stmnt;

typedef struct StmntArr{
    unsigned int count;
    unsigned int capacity;
    Stmnt* stmnts;
}StmntArr;

void print_stmnt(Stmnt* stmnt);
bool initialize_stmnt_arr(StmntArr* arr);
bool reallocate_stmnt_arr(StmntArr* arr);
void free_stmnt_arr(StmntArr* arr);

#endif
