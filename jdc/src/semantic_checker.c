#include "semantic_checker.h"
#include "jadro.h"
#include "stmnt.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static void evaluate(SemanticChecker* checker, Stmnt* stmnt);
static void execute(SemanticChecker* checker, Expr* expr);
static void expr_statement(SemanticChecker* checker, ExprStmnt* stmnt);
static void print_statement(SemanticChecker* checker, PrintStmnt* stmnt);
static void block_statement(SemanticChecker* checker, BlockStmnt* stmnt);
static void decl_statement(SemanticChecker* checker, DeclStmnt* stmnt);
static void assign_statement(SemanticChecker* checker, AssignStmnt* stmnt);
static void binary_expression(SemanticChecker* checker, BinaryExpr* expr);
static void unary_expression(SemanticChecker* checker, UnaryExpr* expr);
static void value_expression(SemanticChecker* checker, ValueExpr* expr);
static void var_expression(SemanticChecker* checker, VariableExpr* expr);
static bool is_same_type(ValueTag a, ValueTag b);

bool initialize_semantic_checker(SemanticChecker* checker) {
    return initialize_stack(&checker->stack);
}

void semantic_check(SemanticChecker* checker, StmntArr* stmnts) {
    for (unsigned int i = 0; i < stmnts->count; i++) {
        evaluate(checker, &stmnts->stmnts[i]);
    }
}

static void evaluate(SemanticChecker* checker, Stmnt* stmnt) {
    switch (stmnt->tag) {
        case EXPR_STMNT: expr_statement(checker, &stmnt->as.exp); break;
        case PRINT_STMNT: print_statement(checker, &stmnt->as.pri); break; 
        case BLOCK_STMNT: block_statement(checker, &stmnt->as.blo); break;
        case DECL_STMNT: decl_statement(checker, &stmnt->as.dec); break;
        case ASSIGN_STMNT: assign_statement(checker, &stmnt->as.ass); break;
    }
}

static void execute(SemanticChecker* checker, Expr* expr) {
    switch (expr->tag) {
        case BINARY_EXPR: binary_expression(checker, &expr->as.bin); break;
        case UNARY_EXPR: unary_expression(checker, &expr->as.una); break;
        case VALUE_EXPR: value_expression(checker, &expr->as.val); break;
        case VARIABLE_EXPR: var_expression(checker, &expr->as.var); break;
    }
}

static void expr_statement(SemanticChecker* checker, ExprStmnt* stmnt) {
    execute(checker, stmnt->expr);
}

static void print_statement(SemanticChecker* checker, PrintStmnt* stmnt) {
    execute(checker, stmnt->expr);
}

static void block_statement(SemanticChecker* checker, BlockStmnt* stmnt) {
    for (unsigned int i = 0; i < stmnt->stmnts->count; i++) {
        evaluate(checker, &stmnt->stmnts->stmnts[i]);
    }
}

static void decl_statement(SemanticChecker* checker, DeclStmnt* stmnt) {}

static void assign_statement(SemanticChecker* checker, AssignStmnt* stmnt) {}

static void binary_expression(SemanticChecker* checker, BinaryExpr* expr) {
    execute(checker, expr->left);
    execute(checker, expr->right);

    Stack* stack = &checker->stack;
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

static void unary_expression(SemanticChecker* checker, UnaryExpr* expr) {
    execute(checker, expr->right);

    Stack* stack = &checker->stack;
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

static void value_expression(SemanticChecker* checker, ValueExpr* expr) {
    push(&checker->stack, expr->value.tag);
}

static void var_expression(SemanticChecker* checker, VariableExpr* expr) {
}

static bool is_same_type(ValueTag a, ValueTag b) {
    return a == b;
}
