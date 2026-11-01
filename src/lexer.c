#include "lexer.h"
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void initialize_lexer(Lexer* lexer, TokenArr* arr, const char* input) {
    lexer->line = 1;
    lexer->col = 0;
    lexer->start = 0;
    lexer->current = 0;
    lexer->input = input;
    lexer->token_arr = arr;
}

static bool is_at_end(Lexer* lexer);
static char advance(Lexer* lexer);
static bool match(const char c, Lexer* lexer);
static void make_identifier(Lexer* lexer);
static void make_digit(Lexer* lexer);
static void make_string(Lexer* lexer);
static void make_token(TokenType type, Lexer* lexer);
static void make_token_value(TokenType type, Value value, Lexer* lexer);

void tokenize(Lexer* lexer) {

    while (!is_at_end(lexer)) {
        lexer->start = lexer->current;
        char c = advance(lexer);
        switch (c) {
            case '(': make_token(TOKEN_LPAREN, lexer); break;   
            case ')': make_token(TOKEN_RPAREN, lexer); break;
            case '{': make_token(TOKEN_LBRACE, lexer); break;
            case '}': make_token(TOKEN_RBRACE, lexer); break;
            case '[': make_token(TOKEN_LBRACKET, lexer); break;
            case ']': make_token(TOKEN_RBRACKET, lexer); break;
            case '.': make_token(TOKEN_DOT, lexer); break;
            case ',': make_token(TOKEN_COMMA, lexer); break;
            case '+': make_token(TOKEN_PLUS, lexer); break;
            case '-': make_token(TOKEN_MINUS, lexer); break;
            case '/': make_token(TOKEN_SLASH, lexer); break;
            case '*': make_token(TOKEN_STAR, lexer); break;
            case '%': make_token(TOKEN_PERCENT, lexer); break;
            case '=': make_token(match('=', lexer) ? TOKEN_EQUALS_EQUALS : TOKEN_EQUALS, lexer); break;
            case '>': make_token(match('=', lexer) ? TOKEN_GREATER_EQUALS : TOKEN_GREATER, lexer); break;
            case '<': make_token(match('=', lexer) ? TOKEN_LESSER_EQUALS : TOKEN_LESSER, lexer); break;
            case '!': make_token(match('=', lexer) ? TOKEN_BANG_EQUALS : TOKEN_BANG, lexer); break;
            case '\n': lexer->line++; lexer->col = 0; break;
            case ' ':
            case '\t':
            case '\r':
                break;          
            case '"': make_string(lexer); break;
            default: 
                if (isdigit(c)) {
                    make_digit(lexer);
                    break;
                }else if(isalpha(c) || c == '_') {
                    make_identifier(lexer);
                }else {
                    make_token(TOKEN_UKNOWN, lexer); 
                    break;
                }
        }
    }
}

static void make_token(TokenType type, Lexer* lexer) {
    Token new_token = (Token) {
        .type = type,
        .line = lexer->line,
        .col = lexer->col,
        .len = lexer->current - lexer->start,
        .lexeme = &lexer->input[lexer->start],
        .value = (Value) { .tag = TAG_NIL, .as = { .n = NULL } }
    };
    
    reallocate_token_arr(lexer->token_arr);
    lexer->token_arr->tokens[lexer->token_arr->count++] = new_token;
}

static void make_token_value(TokenType type, Value value, Lexer* lexer) {
    Token new_token = (Token) {
        .type = type,
        .line = lexer->line,
        .col = lexer->col,
        .len = lexer->current - lexer->start,
        .lexeme = &lexer->input[lexer->start],
        .value = value
    };
    
    reallocate_token_arr(lexer->token_arr);
    lexer->token_arr->tokens[lexer->token_arr->count++] = new_token;
}

static char peek(Lexer* lexer) {
    return lexer->input[lexer->current];
}

static TokenType check_keyword(const char* identifier) {
    if (memcmp("prem", identifier, 4) == 0) return TOKEN_PREM; 
    else if (memcmp("ak", identifier, 2) == 0) return TOKEN_AK;
    else if (memcmp("inak", identifier, 4) == 0) return TOKEN_INAK;

    return TOKEN_IDENTIFIER;
}

static void make_identifier(Lexer* lexer) {
    const char* identifier;

    while (!is_at_end(lexer) && (isalnum(peek(lexer)) || peek(lexer) == '_')) advance(lexer);
    
    identifier = &lexer->input[lexer->start];

    make_token(check_keyword(identifier), lexer);
}

static void make_digit(Lexer* lexer) {
    bool decimal = false;
    while (!is_at_end(lexer) && isdigit(peek(lexer))) advance(lexer);
    
    if (peek(lexer) == '.') {
        advance(lexer);
        if (isdigit(peek(lexer))) {
            decimal = true;
            while (!is_at_end(lexer) && isdigit(peek(lexer))) advance(lexer);   
        }
    }

    Value new_value;
    if (decimal) {
        new_value = (Value) { 
            .tag = TAG_DOUBLE, 
            .as = { .d = strtod(&lexer->input[lexer->start], NULL) }
        };
    }else {
        new_value = (Value) {
            .tag = TAG_INTEGER,
            .as = { .i = atoi(&lexer->input[lexer->start]) }
        };
    }

    make_token_value(decimal ? TOKEN_DOUBLE : TOKEN_INTEGER, new_value, lexer);
}

static void make_string(Lexer* lexer) {
    lexer->start = lexer->current;

    while (!is_at_end(lexer) && advance(lexer) != '"');

    lexer->current--;
    Value new_value = (Value) {
        .tag = TAG_STRING,
        .as = { .s = { .len = (lexer->current - lexer->start), .ptr = &lexer->input[lexer->start] } }
    };

    
    make_token_value(TOKEN_STRING, new_value, lexer);
    lexer->current++;
}

static char advance(Lexer* lexer) {
    lexer->col++;
    return lexer->input[lexer->current++];
}

static bool match(const char c, Lexer* lexer) {
    if (peek(lexer) == c) {
        lexer->current++;
        return true;
    }

    return false;
}

static bool is_at_end(Lexer* lexer) {
    return peek(lexer) == '\0';
}
