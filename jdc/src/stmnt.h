#ifndef STMNT_H
#define STMNT_H

#include "expr.h"

typedef struct Stmnt Stmnt;
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
    Token* token;
}DeclStmnt;

typedef struct {
    String name;
    Expr* expr;
    Token* token;
}AssignStmnt;

typedef struct {
    Expr* condition;
    Stmnt* body;
    Stmnt* else_body;
    Token* token;
}IfStmnt;

typedef struct {
    Expr* condition;
    Stmnt* body;
    Token* token;
}WhileStmnt;

typedef struct {
    String name;
    StmntArr* params;
    Stmnt* body;
}FunkDeclStmnt;

typedef struct {
    Expr* expr;
}ReturnStmnt;

typedef enum {
    EXPR_STMNT,
    PRINT_STMNT,
    BLOCK_STMNT,
    DECL_STMNT,
    ASSIGN_STMNT,
    IF_STMNT,
    WHILE_STMNT,
    FUNK_DECL_STMNT,
    RETURN_STMNT
}StmntTag;

typedef struct Stmnt {
    StmntTag tag;
    union {
        ExprStmnt exp;
        PrintStmnt pri;
        BlockStmnt blo;
        DeclStmnt dec;
        AssignStmnt ass;
        IfStmnt ifs;
        WhileStmnt whi;
        FunkDeclStmnt fun;
        ReturnStmnt ret;
    }as;
}Stmnt;

typedef struct StmntArr{
    unsigned int count;
    unsigned int capacity;
    Stmnt** stmnts;
}StmntArr;

void print_stmnt(Stmnt* stmnt);
void initialize_stmnt_arr(StmntArr* arr);
void reallocate_stmnt_arr(StmntArr* arr);

#endif
