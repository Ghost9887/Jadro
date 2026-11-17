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
static bool is_int(Value* value);
static bool is_double(Value* value);
static bool is_string(Value* value);
static bool is_char(Value* value);
static bool is_bool(Value* value);
static bool is_nil(Value* value);
static bool is_same_type(Value* a, Value* b);

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

    Value* b = pop(stack);
    Value* a = pop(stack);

    if (b == NULL || a == NULL) {
        error(expr->op, "CHYBA KOMPILATORA!");
        return;
    }

    switch (expr->op->type) {
        case TOKEN_PLUS: { 
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_string(a) && !is_string(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_char(a) || is_bool(a) || is_nil(a)) {
                error(expr->op, "Operacia '+' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_MINUS: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_char(a) || is_bool(a) || is_nil(a) || is_string(a)) {
                error(expr->op, "Operacia '-' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_STAR: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_string(a) && !is_int(b)) {
                error(expr->op, "Typ 'retazec' sa da pouzit iba s typom int");
            }else if (is_char(a) || is_bool(a) || is_nil(a)) {
                error(expr->op, "Operacia '*' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_SLASH: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_char(a) || is_bool(a) || is_nil(a) || is_string(a)) {
                error(expr->op, "Operacia '/' sa neda pouzit na tomto type");
            }else {
                int divider = (is_int(b)) ? b->as.i : b->as.d;
                if (divider == 0) error(expr->op, "Delenie nulou");
            }
        };break;
        case TOKEN_PERCENT: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy niesu ten isty typ");
            }else if (is_char(a) || is_bool(a) || is_nil(a) || is_string(a) || is_double(a)) {
                error(expr->op, "Operacia '\%' sa neda pouzit na tomto type");
            }        
        };break;
        case TOKEN_EQUALS_EQUALS: {
            if (!is_same_type(a, b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }
        };break;
        case TOKEN_BANG_EQUALS: {
            if (!is_same_type(a, b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }
        };break;
        case TOKEN_GREATER: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_bool(a) && !is_bool(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Operacia '>' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_GREATER_EQUALS: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_bool(a) && !is_bool(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Operacia '>=' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_LESSER: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_bool(a) && !is_bool(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Operacia '<' sa neda pouzit na tomto type");
            }
        };break;
        case TOKEN_LESSER_EQUALS: {
            if (is_int(a) && !is_int(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_double(a) && !is_double(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_bool(a) && !is_bool(b)) {
                error(expr->op, "Operandy sa nedaju porovnat");
            }else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Operacia '<=' sa neda pouzit na tomto type");
            }
        };break;
        default: break; //unreachable
    }
    
}

static void unary_expression(Stack* stack, UnaryExpr* expr) {
    execute(stack, expr->right);
    Value* a = pop(stack);

    switch (expr->op->type) {
        case TOKEN_MINUS: {
            if (is_bool(a)) {
                error(expr->op, "Tento typ sa neguje pomocou '!'");
            }
            else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Tento typ sa neda negovat");
            }
        };break;
        case TOKEN_BANG: {
            if (is_int(a) || is_double(a)) {
                error(expr->op, "Tento typ sa neguje pomocou '-'");
            }
            else if (is_char(a) || is_string(a) || is_nil(a)) {
                error(expr->op, "Tento typ sa neda negovat");
            }
        };break;
        default: break; //unreachable
    }
}

static void value_expression(Stack* stack, ValueExpr* expr) {
    push(stack, &expr->value);
}

static void var_expression(Stack* stack, VariableExpr* expr) {
}

static bool is_int(Value* value) {
    return value->tag == INTEGER_VALUE;
}

static bool is_double(Value* value) {
    return value->tag == DOUBLE_VALUE;
}

static bool is_string(Value* value) {
    return value->tag == STRING_VALUE;
}

static bool is_char(Value* value) {
    return value->tag == CHAR_VALUE;
}

static bool is_bool(Value* value) {
    return value->tag == BOOLEAN_VALUE;
}

static bool is_nil(Value* value) {
    return value->tag == NIL_VALUE;
}

static bool is_same_type(Value* a, Value* b) {
    return a->tag == b->tag;
}
