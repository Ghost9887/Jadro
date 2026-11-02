#ifndef EXPR_H
#define EXPR_H

#include "value.h"

struct Expr;

typedef struct {
    struct Expr* left;
    char op;
    struct Expr* right;
}BinaryExpr;

typedef struct {
    Value value;
}ValueExpr;

typedef enum {
    EXPR_TAG_BINARY,
    EXPR_TAG_VALUE
}ExprTag;

typedef struct {
    ExprTag tag;
    union {
        BinaryExpr* b;
        ValueExpr* v;
    }as;
}Expr;

#endif
