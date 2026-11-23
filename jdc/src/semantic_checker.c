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
static void if_statement(SemanticChecker* checker, IfStmnt* stmnt);
static void while_statement(SemanticChecker* checker, WhileStmnt* stmnt);
static void binary_expression(SemanticChecker* checker, BinaryExpr* expr);
static void unary_expression(SemanticChecker* checker, UnaryExpr* expr);
static void value_expression(SemanticChecker* checker, ValueExpr* expr);
static void var_expression(SemanticChecker* checker, VariableExpr* expr);
static bool is_same_type(ValueTag a, ValueTag b);
static void create_new_env(SemanticChecker* checker);
static void destroy_current_env(SemanticChecker* checker);

void initialize_semantic_checker(SemanticChecker* checker) {
    initialize_stack(&checker->stack);
    checker->current_env = 0;
    new_env(&checker->env_arr[checker->current_env], NULL);
}

void semantic_check(SemanticChecker* checker, StmntArr* stmnts) {
    for (unsigned int i = 0; i < stmnts->count; i++) {
        evaluate(checker, stmnts->stmnts[i]);
    }
}

static void evaluate(SemanticChecker* checker, Stmnt* stmnt) {
    switch (stmnt->tag) {
        case EXPR_STMNT: expr_statement(checker, &stmnt->as.exp); break;
        case PRINT_STMNT: print_statement(checker, &stmnt->as.pri); break; 
        case BLOCK_STMNT: block_statement(checker, &stmnt->as.blo); break;
        case DECL_STMNT: decl_statement(checker, &stmnt->as.dec); break;
        case ASSIGN_STMNT: assign_statement(checker, &stmnt->as.ass); break;
        case IF_STMNT: if_statement(checker, &stmnt->as.ifs); break;
        case WHILE_STMNT: while_statement(checker, &stmnt->as.whi); break;
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
    create_new_env(checker);
    for (unsigned int i = 0; i < stmnt->stmnts->count; i++) {
        evaluate(checker, stmnt->stmnts->stmnts[i]);
    }
    destroy_current_env(checker);
}

static void decl_statement(SemanticChecker* checker, DeclStmnt* stmnt) {
    execute(checker, stmnt->expr);
    ValueTag val = pop(&checker->stack);

    if (!add_variable(&checker->env_arr[checker->current_env], &stmnt->name, val)) 
        error(stmnt->token, "Premenna uz existuje");
}

static void assign_statement(SemanticChecker* checker, AssignStmnt* stmnt) {
    execute(checker, stmnt->expr);   
    ValueTag val = pop(&checker->stack);

    Variable* var = get_variable(&checker->env_arr[checker->current_env], &stmnt->name);
    if (var == NULL) {
        error(stmnt->token, "Premenna neexistuje");
    }
    else {
        var->value = val;
    }
}

static void if_statement(SemanticChecker* checker, IfStmnt* stmnt) {
    execute(checker, stmnt->condition);

    ValueTag condition = pop(&checker->stack);

    if (condition != BOOLEAN_VALUE) {
        error(stmnt->token, "Podmienka musi by vyhodnotena boolom");
    }

    evaluate(checker, stmnt->body);
    if (stmnt->else_body != NULL) {
        evaluate(checker, stmnt->else_body);
    }
}

static void while_statement(SemanticChecker* checker, WhileStmnt* stmnt) {
    execute(checker, stmnt->condition);

    ValueTag condition = pop(&checker->stack);

    if (condition != BOOLEAN_VALUE) {
        error(stmnt->token, "Podmienka musi by vyhodnotena boolom");
    }
    evaluate(checker, stmnt->body);
}

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
            else error(expr->op, "Operacia '%' sa neda pouzit na tomto vzorcy"); 
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
    Variable* var = get_variable(&checker->env_arr[checker->current_env], &expr->name);
    if (var == NULL) 
        error(expr->token, "Premenna neexistuje");
    else {
        push(&checker->stack, var->value);
    }
}

static bool is_same_type(ValueTag a, ValueTag b) {
    return a == b;
}

static void create_new_env(SemanticChecker* checker) {
    if (checker->current_env + 1 >= MAX_ENVS) {
        printf("Maximalny pocet blokov presiahnuty! (MAX: %u)\n", MAX_ENVS - 1);
        exit(1);
    }else {
        unsigned int new = ++checker->current_env;
        unsigned int parent = checker->current_env - 1;
        new_env(&checker->env_arr[new], &checker->env_arr[parent]);
    }
}

static void destroy_current_env(SemanticChecker* checker) {
    if (checker->current_env <= 0) {
        printf("Current env je mensi alebo rovni 0!\n");
        exit(1);
    }else {
        destroy_env(&checker->env_arr[checker->current_env--]);
    }
}   
