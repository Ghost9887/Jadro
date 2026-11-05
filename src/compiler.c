#include "compiler.h"
#include <stddef.h>
#include <stdint.h>

void initialize_compiler(Compiler* compiler, StmntArr* stmnt_arr, const char* output_file_name) {
    compiler->stmnt_arr = stmnt_arr;
    compiler->output_file = fopen((output_file_name == NULL) ? "output.skc" : output_file_name, "w");
}

static void write_code(Compiler* compiler, OpCode code);
static void evaluate(Compiler* compiler, Stmnt* stmnt);
static void expr_statement(Compiler* compiler, ExprStmnt* stmnt);
static void print_statement(Compiler* compiler, PrintStmnt* stmnt);
static void block_statement(Compiler* compiler, BlockStmnt* stmnt);
static void decl_statement(Compiler* compiler, DeclStmnt* stmnt);
static void assign_statement(Compiler* compiler, AssignStmnt* stmnt);
static void binary_expression(Compiler* compiler, BinaryExpr* expr);
static void unary_expression(Compiler* compiler, UnaryExpr* expr);
static void value_expression(Compiler* compiler, ValueExpr* expr);
static void var_expression(Compiler* compiler, VariableExpr* expr);

void compile(Compiler* compiler) {
    for (unsigned int i = 0; i < compiler->stmnt_arr->count; i++) {
        evaluate(compiler, &compiler->stmnt_arr->stmnts[i]);
    }
    fclose(compiler->output_file);
}

static void write_code(Compiler* compiler, OpCode code) {
    uint8_t byte = (uint8_t)code;
    fwrite(&code, sizeof(byte), 1, compiler->output_file);
}

static void write_value(Compiler* compiler, Value* value) {
    switch (value->tag) {
        case NIL_VALUE: write_code(compiler, OP_NIL); break;
        case STRING_VALUE: {
                String str = value->as.s;
                write_code(compiler, OP_STRING);
                write_code(compiler, OP_INTEGER);
                fwrite(&str.len, sizeof(str.len), 1, compiler->output_file);
                fwrite(str.ptr, 1, str.len, compiler->output_file);
            };break;
        case CHAR_VALUE: {
                char c = value->as.c;
                write_code(compiler, OP_CHAR);
                fwrite(&c, sizeof(c), 1, compiler->output_file);
        }; break;
        case INTEGER_VALUE: {
            int i = value->as.i;
            write_code(compiler, OP_INTEGER);
            fwrite(&i, sizeof(i), 1, compiler->output_file);
        }; break;
        case DOUBLE_VALUE: {
            double d = value->as.d;
            write_code(compiler, OP_DOUBLE);
            fwrite(&d, sizeof(d), 1, compiler->output_file);
        };break;
        case BOOLEAN_VALUE: {
            bool b = value->as.b;
            write_code(compiler, OP_BOOLEAN);
            fwrite(&b, sizeof(b), 1, compiler->output_file);
        };break;
    }
}

static void evaluate(Compiler* compiler, Stmnt* stmnt) {
    switch (stmnt->tag) {
        case EXPR_STMNT: expr_statement(compiler, &stmnt->as.exp); break;
        case PRINT_STMNT: print_statement(compiler, &stmnt->as.pri); break; 
        case BLOCK_STMNT: block_statement(compiler, &stmnt->as.blo); break;
        case DECL_STMNT: decl_statement(compiler, &stmnt->as.dec); break;
        case ASSIGN_STMNT: assign_statement(compiler, &stmnt->as.ass); break;
    }
}

static void execute(Compiler* compiler, Expr* expr) {
    switch (expr->tag) {
        case BINARY_EXPR: binary_expression(compiler, &expr->as.bin); break;
        case UNARY_EXPR: unary_expression(compiler, &expr->as.una); break;
        case VALUE_EXPR: value_expression(compiler, &expr->as.val); break;
        case VARIABLE_EXPR: var_expression(compiler, &expr->as.var); break;
    }
}

static void expr_statement(Compiler* compiler, ExprStmnt* stmnt) {
    execute(compiler, stmnt->expr);
}

static void print_statement(Compiler* compiler, PrintStmnt* stmnt) {
    execute(compiler, stmnt->expr);
    write_code(compiler, OP_PRINT);
}

static void block_statement(Compiler* compiler, BlockStmnt* stmnt) {
    write_code(compiler, OP_NEW_ENV);
    for (unsigned int i = 0; i < stmnt->stmnts->count; i++) {
        evaluate(compiler, &stmnt->stmnts->stmnts[i]);
    }
    write_code(compiler, OP_DELETE_ENV);
}

static void decl_statement(Compiler* compiler, DeclStmnt* stmnt) {
    execute(compiler, stmnt->expr);
    write_code(compiler, OP_DECL);
}

static void assign_statement(Compiler* compiler, AssignStmnt* stmnt) {
    execute(compiler, stmnt->expr);
    write_code(compiler, OP_ASSIGN);
}

static void binary_expression(Compiler* compiler, BinaryExpr* expr) {
    execute(compiler, expr->left);
    execute(compiler, expr->right);

    switch (expr->op->type) {
        case TOKEN_PLUS: write_code(compiler, OP_ADD); break;
        case TOKEN_MINUS: write_code(compiler, OP_SUBTRACT); break;
        case TOKEN_STAR: write_code(compiler, OP_MULTIPLY); break;
        case TOKEN_SLASH: write_code(compiler, OP_DIVIDE); break;
        case TOKEN_PERCENT: write_code(compiler, OP_MODULO); break;
        case TOKEN_GREATER: write_code(compiler, OP_GREATER); break;
        case TOKEN_GREATER_EQUALS: write_code(compiler, OP_GREATER_EQUALS); break;
        case TOKEN_LESSER: write_code(compiler, OP_LESSER); break;
        case TOKEN_LESSER_EQUALS: write_code(compiler, OP_LESSER_EQUALS); break;
        case TOKEN_EQUALS_EQUALS: write_code(compiler, OP_EQUALS_EQUALS); break;
        case TOKEN_BANG_EQUALS: write_code(compiler, OP_NOT_EQUALS); break;
        default: break; //unreachable
    }
}

static void unary_expression(Compiler* compiler, UnaryExpr* expr) {
    execute(compiler, expr->right);
    write_code(compiler, OP_NEGATE);
}

static void value_expression(Compiler* compiler, ValueExpr* expr) {
    write_value(compiler, &expr->value);
}

static void var_expression(Compiler* compiler, VariableExpr* expr) {
    Value temp = (Value) { .tag = STRING_VALUE, .as = { .s = expr->name } };
    write_value(compiler, &temp);
    write_code(compiler, OP_VAR);
}
