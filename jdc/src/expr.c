#include "expr.h"
#include <stdio.h>
#include <stdlib.h>

void print_expr(Expr* expr) {
    switch (expr->tag) {
        case BINARY_EXPR: 
            BinaryExpr* bin = &expr->as.bin;
            printf("BinaryExpr[");
            print_expr(bin->left);
            printf(" '%.*s' ", bin->op->lexeme.len, bin->op->lexeme.ptr);
            print_expr(bin->right);
            printf("]");
            break;
        case UNARY_EXPR:
            UnaryExpr* una = &expr->as.una;
            printf("UnaryExpr[");
            printf(" '%.*s' ", una->op->lexeme.len, una->op->lexeme.ptr);
            print_expr(una->right);
            printf("]");
            break;
        case VALUE_EXPR:
            ValueExpr* val = &expr->as.val;
            printf("ValueExpr[");
            print_value(&val->value);
            printf("]");
            break;
        case VARIABLE_EXPR:
            printf("VariableExpr[%.*s]", expr->as.var.name.len, expr->as.var.name.ptr);
            break;
    }
}
