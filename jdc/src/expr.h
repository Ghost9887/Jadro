#ifndef EXPR_H
#define EXPR_H

#include "value.h"
#include "token.h"

typedef struct Expr Expr;

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

typedef enum {
    BINARY_EXPR,
    UNARY_EXPR,
    VALUE_EXPR,
    VARIABLE_EXPR
}ExprTag;

typedef struct Expr{
    ExprTag tag;
    union {
        BinaryExpr bin;
        UnaryExpr una;
        ValueExpr val;
        VariableExpr var;
    }as;
}Expr;

void print_expr(Expr* expr);

#endif
