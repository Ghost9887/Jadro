#include "parser.h"
#include "jadro.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static Stmnt statement(Parser* parser);
static Stmnt expr_statement(Parser* parser);
static Stmnt print_statement(Parser* parser);
static Stmnt block_statement(Parser* parser);
static Stmnt decl_statement(Parser* parser);
static Stmnt assign_statement(Parser* parser);
static Expr* expression(Parser* parser);
static Expr* equality(Parser* parser);
static Expr* comparison(Parser* parser);
static Expr* term(Parser* parser);
static Expr* factor(Parser* parser);
static Expr* unary(Parser* parser);
static Expr* primary(Parser* parser);
static void advance(Parser* parser);
static Token* previous(Parser* parser);
static Token* peek(Parser* parser);
static TokenType peek_type(Parser* parser);
static bool match(Parser* parser, int count, ...);
static bool consume(Parser* parser, TokenType type, const char* message);
static void parse_error(Parser* parser, const char* message);
static void synchronize(Parser* parser);

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
    if (match(parser, 1, TOKEN_VYTLAC)) return print_statement(parser);
    if (match(parser, 1, TOKEN_LBRACE)) return block_statement(parser);
    if (match(parser, 1, TOKEN_PREM)) return decl_statement(parser);
    if (match(parser, 1, TOKEN_IDENTIFIER)) return assign_statement(parser);
    return expr_statement(parser);
}

static Stmnt expr_statement(Parser* parser) {
    Stmnt stmnt = (Stmnt) {
        .tag = EXPR_STMNT, 
        .as = { .exp = (ExprStmnt) { .expr = expression(parser) } } 
    };
    consume(parser, TOKEN_SEMICOLON, "Ocakavany ';'");
    return stmnt;
}

static Stmnt print_statement(Parser* parser) {
    consume(parser, TOKEN_LPAREN, "Ocakavany '('");
    Expr* expr = expression(parser);
    consume(parser, TOKEN_RPAREN, "Ocakavany ')'");
    consume(parser, TOKEN_SEMICOLON, "Ocakavany ';'");
    return (Stmnt) {
        .tag = PRINT_STMNT,
        .as = { .exp = (ExprStmnt) { .expr = expr } }
    };
}

static Stmnt block_statement(Parser* parser) {
    StmntArr* stmnts = (StmntArr*)malloc(sizeof(StmntArr));
    initialize_stmnt_arr(stmnts);

    while (!match(parser, 1, TOKEN_RBRACE)) {
        reallocate_stmnt_arr(stmnts);
        stmnts->stmnts[stmnts->count++] = statement(parser);
    }

    return (Stmnt) {
        .tag = BLOCK_STMNT,
        .as = { .blo = (BlockStmnt) { .stmnts = stmnts } } 
    };
}

static Stmnt decl_statement(Parser* parser) {
    consume(parser, TOKEN_IDENTIFIER, "Ocakavany identifikator");
    String name = previous(parser)->lexeme;
    
    bool has_value = false;
    Expr* expr;
    if (match(parser, 1, TOKEN_EQUALS)) {
        has_value = true;
        expr = expression(parser);
    }
    consume(parser, TOKEN_SEMICOLON, "Ocakavany ';'");
    if (!has_value) {
        expr = malloc(sizeof(Expr));
        *expr = (Expr) {
            .tag = VALUE_EXPR,
            .as = { .val = (ValueExpr) { .value = (Value) { .tag = NIL_VALUE, .as = { .n = NULL } } } }
        };
    }
    
    return (Stmnt) {
        .tag = DECL_STMNT,
        .as = { .dec = (DeclStmnt) { .name = name, .expr = expr } }
    };
}

static Stmnt assign_statement(Parser* parser) {
    String name = previous(parser)->lexeme;
    consume(parser, TOKEN_EQUALS, "Ocakavany '='");
    Expr* expr = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Ocakavany ';'");

    return (Stmnt) {
        .tag = ASSIGN_STMNT,
        .as = { .ass = (AssignStmnt) { .name = name, .expr = expr } }
    };
}

static Expr* expression(Parser* parser) {
    return equality(parser);
}

static Expr* equality(Parser* parser) {
    Expr* expr = comparison(parser);
    
    while (match(parser, 2, TOKEN_EQUALS_EQUALS, TOKEN_BANG_EQUALS)) {
        Token* prev = previous(parser);
        Expr* right = comparison(parser);

        Expr* binary = malloc(sizeof(Expr));
        *binary = (Expr) {
            .tag = BINARY_EXPR,
            .as = { .bin = (BinaryExpr) { .left = expr, .op = prev, .right = right } }
        };

        expr = binary;
    }

    return expr;
}

static Expr* comparison(Parser* parser) {
    Expr* expr = term(parser);
    
    while (match(parser, 4, TOKEN_GREATER, TOKEN_LESSER, TOKEN_GREATER_EQUALS, TOKEN_LESSER_EQUALS)) {
        Token* prev = previous(parser);
        Expr* right = comparison(parser);

        Expr* binary = malloc(sizeof(Expr));
        *binary = (Expr) {
            .tag = BINARY_EXPR,
            .as = { .bin = (BinaryExpr) { .left = expr, .op = prev, .right = right } }
        };

        expr = binary;
    }

    return expr;
}

static Expr* term(Parser* parser) {
    Expr* expr = factor(parser);

    while (match(parser, 2, TOKEN_PLUS, TOKEN_MINUS)) {
        Token* prev = previous(parser);
        Expr* right = factor(parser);

        Expr* binary = malloc(sizeof(Expr));
        *binary = (Expr) {
            .tag = BINARY_EXPR,
            .as = { .bin = (BinaryExpr) { .left = expr, .op = prev, .right = right } }
        };

        expr = binary;
    }

    return expr;
}

static Expr* factor(Parser* parser) {
    Expr* expr = unary(parser);
    
    while (match(parser, 3, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT)) {
        Token* prev = previous(parser);
        Expr* right = comparison(parser);

        Expr* binary = malloc(sizeof(Expr));
        *binary = (Expr) {
            .tag = BINARY_EXPR,
            .as = { .bin = (BinaryExpr) { .left = expr, .op = prev, .right = right } }
        };

        expr = binary;
    }
    return expr;
}

static Expr* unary(Parser* parser) {
    if (match(parser, 2, TOKEN_MINUS, TOKEN_BANG)) {
        Token* prev = previous(parser);
        Expr* right = primary(parser);

        Expr* unary = malloc(sizeof(Expr));
        *unary = (Expr) {
            .tag = UNARY_EXPR,
            .as = { .una = (UnaryExpr) { .op = prev, right = right } }
        };

        return unary;
    }

    return primary(parser);
}

static Expr* primary(Parser* parser) {
    Expr* expr = malloc(sizeof(Expr));
    
    if (match(parser, 6, TOKEN_STRING, TOKEN_CHAR, TOKEN_INTEGER, TOKEN_DOUBLE, TOKEN_BOOLEAN, TOKEN_NIL)) {
        Token* prev = previous(parser);
        *expr = (Expr) {
            .tag = VALUE_EXPR,
            .as = { .val = (ValueExpr) { .value = prev->value } }
        };
        return expr;
    }

    parse_error(parser, "Ocakavana podmienka");
    return NULL;
}

static void advance(Parser* parser) {
    parser->ip++;
}

static Token* previous(Parser* parser) {
    return &parser->token_arr->tokens[parser->ip - 1];
}

static Token* peek(Parser* parser) {
    return &parser->token_arr->tokens[parser->ip];
}

static TokenType peek_type(Parser* parser) {
    return peek(parser)->type;
}

static bool match(Parser* parser, int count, ...) {
    va_list ap;

    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        if (va_arg(ap, TokenType) == peek_type(parser)) {
            advance(parser);
            va_end(ap);
            return true;
        }
    }
    va_end(ap);

    return false;
}

static bool consume(Parser* parser, TokenType type, const char* message) {
    if (peek_type(parser) == type) {
        advance(parser);
        return true;
    }else {
        parse_error(parser, message);
    }
    return false;
}

static void parse_error(Parser* parser, const char* message) {
    jadro_error(previous(parser), message);
    synchronize(parser);
}

static void synchronize(Parser* parser) {
    while (parser->ip < parser->token_arr->count) {
        if (peek(parser)->type == TOKEN_SEMICOLON) {
            advance(parser);
            return;
        }
        switch (peek(parser)->type) {
            case TOKEN_VYTLAC:
            case TOKEN_LBRACE:
            case TOKEN_IDENTIFIER:
            case TOKEN_PREM:
            case TOKEN_RBRACE:
                return;
            default: advance(parser);
        }
    }
}
