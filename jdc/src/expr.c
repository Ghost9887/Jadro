#include "expr.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_expr_arr(ExprArr* arr) {
    arr->capacity = 2;
    arr->count = 0;
    arr->exprs = (Expr**)malloc(sizeof(Expr*) * arr->capacity);
    if (arr->exprs == NULL) {
        printf("Nepodarilo sa allocovat ExprArr\n");
        exit(1);
    }
}

void reallocate_expr_arr(ExprArr* arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->exprs = (Expr**)realloc(arr->exprs, sizeof(Expr*) * arr->capacity);

        if (arr->exprs == NULL) {
            printf("Nepodarilo sa reallocovat ExprArr\n");
            exit(1);
        }
    }
}

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
        case FUNK_CALL_EXPR:
            printf("FunkCallExpr[ %.*s ", expr->as.fun.name.len, expr->as.fun.name.ptr);
            for (unsigned int i = 0; i < expr->as.fun.params->count; i++) {
                print_expr(expr->as.fun.params->exprs[i]);
                printf(", ");
            }
            printf("]");
            break;
    }
}
