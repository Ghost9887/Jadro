#ifndef EXPR_H
#define EXPR_H

#include "value.h"
#include "token.h"
#include <stddef.h>

typedef struct Expr Expr;
typedef struct ExprArr ExprArr;

typedef struct {
    Expr* left;
    Token* op;
    Expr* right;
}BinaryExpr;

typedef struct {
    Token* op;
    Expr* right;
}UnaryExpr;

typedef struct {
    Value value;
}ValueExpr;

typedef struct {
    String name;
    Token* token;
}VariableExpr;

typedef struct {
    String name;
    ExprArr* params;
}FunkCallExpr;

typedef enum {
    BINARY_EXPR,
    UNARY_EXPR,
    VALUE_EXPR,
    VARIABLE_EXPR,
    FUNK_CALL_EXPR
}ExprTag;

typedef struct Expr{
    ExprTag tag;
    union {
        BinaryExpr bin;
        UnaryExpr una;
        ValueExpr val;
        VariableExpr var;
        FunkCallExpr fun;
    }as;
}Expr;

typedef struct ExprArr{
    size_t capacity;
    size_t count;
    Expr** exprs;
}ExprArr;

void print_expr(Expr* expr);
void initialize_expr_arr(ExprArr* arr);
void reallocate_expr_arr(ExprArr* arr);

#endif
