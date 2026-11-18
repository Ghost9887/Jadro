#include "semantic_checker.h"
#include "jadro.h"
#include "stack.h"
#include "stmnt.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static void evaluate(Stack* stack, Stmnt* stmnt);
static void execute(Stack* stack, Expr* expr);
static void expr_statement(Stack* stack, ExprStmnt* stmnt);
static void print_statement(Stack* stack, PrintStmnt* stmnt);
static void block_statement(Stack* stack, BlockStmnt* stmnt);
static void decl_statement(Stack* stack, DeclStmnt* stmnt);
static void assign_statement(Stack* stack, AssignStmnt* stmnt);
static void binary_expression(Stack* stack, BinaryExpr* expr);
static void unary_expression(Stack* stack, UnaryExpr* expr);
static void value_expression(Stack* stack, ValueExpr* expr);
static void var_expression(Stack* stack, VariableExpr* expr);
static bool is_same_type(ValueTag a, ValueTag b);

void semantic_check(StmntArr* stmnts) {
    Stack stack;         
    if(!initialize_stack(&stack)) exit(1);

    for (unsigned int i = 0; i < stmnts->count; i++) {
        evaluate(&stack, &stmnts->stmnts[i]);
    }
}

static void evaluate(Stack* stack, Stmnt* stmnt) {
    switch (stmnt->tag) {
        case EXPR_STMNT: expr_statement(stack, &stmnt->as.exp); break;
        case PRINT_STMNT: print_statement(stack, &stmnt->as.pri); break; 
        case BLOCK_STMNT: block_statement(stack, &stmnt->as.blo); break;
        case DECL_STMNT: decl_statement(stack, &stmnt->as.dec); break;
        case ASSIGN_STMNT: assign_statement(stack, &stmnt->as.ass); break;
    }
}

static void execute(Stack* stack, Expr* expr) {
    switch (expr->tag) {
        case BINARY_EXPR: binary_expression(stack, &expr->as.bin); break;
        case UNARY_EXPR: unary_expression(stack, &expr->as.una); break;
        case VALUE_EXPR: value_expression(stack, &expr->as.val); break;
        case VARIABLE_EXPR: var_expression(stack, &expr->as.var); break;
    }
}

static void expr_statement(Stack* stack, ExprStmnt* stmnt) {
    execute(stack, stmnt->expr);
}

static void print_statement(Stack* stack, PrintStmnt* stmnt) {
    execute(stack, stmnt->expr);
}

static void block_statement(Stack* stack, BlockStmnt* stmnt) {
    for (unsigned int i = 0; i < stmnt->stmnts->count; i++) {
        evaluate(stack, &stmnt->stmnts->stmnts[i]);
    }
}

static void decl_statement(Stack* stack, DeclStmnt* stmnt) {}

static void assign_statement(Stack* stack, AssignStmnt* stmnt) {}

static void binary_expression(Stack* stack, BinaryExpr* expr) {
    execute(stack, expr->left);
    execute(stack, expr->right);

    ValueTag b = pop(stack);
    ValueTag a = pop(stack);

    switch (expr->op->type) {
        case TOKEN_PLUS: { 
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, DOUBLE_VALUE);
            else if (a == STRING_VALUE && b == STRING_VALUE) push(stack, STRING_VALUE);
            else error(expr->op, "Operacia '+' sa neda pouzit na tomto vzorcy");
        };break;
        case TOKEN_MINUS: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, DOUBLE_VALUE);
            else error(expr->op, "Operacia '-' sa neda pouzit na tomto vzorcy");
        };break;
        case TOKEN_STAR: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, DOUBLE_VALUE);
            else if (a == STRING_VALUE && b == INTEGER_VALUE) push(stack, STRING_VALUE);
            else error(expr->op, "Operacia '*' sa neda pouzit na tomto vzorcy");
        };break;
        case TOKEN_SLASH: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, DOUBLE_VALUE);
            else error(expr->op, "Operacia '/' sa neda pouzit na tomto vzorcy");
        };break;
        case TOKEN_PERCENT: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else error(expr->op, "Operacia '\%' sa neda pouzit na tomto vzorcy"); 
        };break;
        case TOKEN_EQUALS_EQUALS: {
            if (is_same_type(a, b)) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        case TOKEN_BANG_EQUALS: {
            if (is_same_type(a, b)) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        case TOKEN_GREATER: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, BOOLEAN_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        case TOKEN_GREATER_EQUALS: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, BOOLEAN_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        case TOKEN_LESSER: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, BOOLEAN_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        case TOKEN_LESSER_EQUALS: {
            if (a == INTEGER_VALUE && b == INTEGER_VALUE) push(stack, BOOLEAN_VALUE);
            else if (a == DOUBLE_VALUE && b == DOUBLE_VALUE) push(stack, BOOLEAN_VALUE);
            else error(expr->op, "Tieto typy sa nedaju porovnat");
        };break;
        default: break; //unreachable
    }
    
}

static void unary_expression(Stack* stack, UnaryExpr* expr) {
    execute(stack, expr->right);
    ValueTag a = pop(stack);

    switch (expr->op->type) {
        case TOKEN_MINUS: {
            if (a == INTEGER_VALUE) push(stack, INTEGER_VALUE);
            else if (a == DOUBLE_VALUE) push(stack, DOUBLE_VALUE);
            else if (a == BOOLEAN_VALUE) error(expr->op, "Tento typ sa neguje pomocou '!'");
            else error(expr->op, "Tento typ sa neda negovat");
        };break;
        case TOKEN_BANG: {
            if (a == BOOLEAN_VALUE) push(stack, BOOLEAN_VALUE);
            else if (a == INTEGER_VALUE || a == DOUBLE_VALUE) error(expr->op, "Tento typ sa neguje pomocou '-'");
            else error(expr->op, "Tento typ sa neda negovat");
        };break;
        default: break; //unreachable
    }
}

static void value_expression(Stack* stack, ValueExpr* expr) {
    push(stack, expr->value.tag);
}

static void var_expression(Stack* stack, VariableExpr* expr) {
}

static bool is_same_type(ValueTag a, ValueTag b) {
    return a == b;
}
