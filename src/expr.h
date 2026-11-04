#ifndef EXPR_H
#define EXPR_H

#include "value.h"
#include "token.h"

typedef struct Expr Expr;

typedef struct {
    struct Expr* left;
    Token* op;
    struct Expr* right;
}BinaryExpr;

typedef struct {
    Token* op;
    struct Expr* right;
}UnaryExpr;

typedef struct {
    Value value;
}ValueExpr;

typedef enum {
    BINARY_EXPR,
    UNARY_EXPR,
    VALUE_EXPR,
}ExprTag;

typedef struct Expr{
    ExprTag tag;
    union {
        BinaryExpr bin;
        UnaryExpr una;
        ValueExpr val;
    }as;
}Expr;

void print_expr(Expr* expr);

#endif
