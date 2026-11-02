#include "parser.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static Stmnt statement(Parser* parser);
static Expr* expression(Parser* parser);
static Expr* equality(Parser* parser);
static Expr* comparison(Parser* parser);
static Expr* term(Parser* parser);
static Expr* factor(Parser* parser);
static Expr* unary(Parser* parser);
static Expr* primary(Parser* parser);
static void advance(Parser* parser);
static Token* previous(Parser* parser);
static bool match(Parser* parser, int count, ...);
static bool consume(Parser* parser, TokenType type);
static TokenType peek_type(Parser* parser);
    

void initialize_parser(Parser* parser, StmntArr* stmnt_arr, TokenArr* token_arr) {
    parser->token_arr = token_arr;
    parser->stmnt_arr = stmnt_arr;
    parser->ip = 0;
}

void parse(Parser* parser) {
    while (parser->ip < parser->token_arr->count) {
        if (!reallocate_stmnt_arr(parser->stmnt_arr)) return;
        parser->stmnt_arr->stmnts[parser->stmnt_arr->count++] = statement(parser);
    }
}

static Stmnt statement(Parser* parser) {
    return (Stmnt){ .tag = STMNT_TAG_EXPR, .as = { .e = &(ExprStmnt){ .expr = expression(parser) } } };
}

static Expr* expression(Parser* parser) {
    return equality(parser);
}

static Expr* equality(Parser* parser) {
    return comparison(parser);
}

static Expr* comparison(Parser* parser) {
    return term(parser);
}

static Expr* term(Parser* parser) {
    return factor(parser);
}

static Expr* factor(Parser* parser) {
    return unary(parser);
}

static Expr* unary(Parser* parser) {
    return primary(parser);
}

static Expr* primary(Parser* parser) {
    printf("RETURNING NULL FROM PRIMARY PARSING\n");
    exit(1);
}

static void advance(Parser* parser) {
    parser->ip++;
}

static Token* previous(Parser* parser) {
    return &parser->token_arr->tokens[parser->ip - 1];
}

static bool match(Parser* parser, int count, ...) {
    va_list ap;

    for (int i = 0; i < count; i++) {
        if (va_arg(ap, TokenType) == peek_type(parser)) {
            advance(parser);
            return true;
        }
    }
    return false;
}

static bool consume(Parser* parser, TokenType type) {
    if (peek_type(parser) == type) {
        advance(parser);
        return true;
    }
    return false;
}

static TokenType peek_type(Parser* parser) {
    return parser->token_arr->tokens[parser->ip].type;
}
