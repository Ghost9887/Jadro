#ifndef TOKEN_H
#define TOKEN_H

#include "value.h"

typedef enum {
    TOKEN_RPAREN, TOKEN_LPAREN, TOKEN_RBRACE, TOKEN_LBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,
    TOKEN_EQUALS, TOKEN_GREATER, TOKEN_LESSER, TOKEN_BANG,
    TOKEN_EQUALS_EQUALS, TOKEN_GREATER_EQUALS, TOKEN_LESSER_EQUALS, TOKEN_BANG_EQUALS,
    TOKEN_SEMICOLON, TOKEN_DOT, TOKEN_COMMA, 

    TOKEN_STRING, TOKEN_INTEGER, TOKEN_DOUBLE, TOKEN_BOOLEAN, TOKEN_IDENTIFIER,

    TOKEN_PREM, TOKEN_AK, TOKEN_INAK, TOKEN_POKIAL, TOKEN_PRE,

    TOKEN_UKNOWN
}TokenType;

typedef struct {
    TokenType type;
    unsigned int line, col, len;
    const char* lexeme;
    Value value;
}Token;

typedef struct {
    unsigned int capacity;
    unsigned int count;
    Token* tokens;
}TokenArr;

bool initialize_token_arr(TokenArr* arr);
bool reallocate_token_arr(TokenArr* arr);
void free_token_arr(TokenArr* arr);
void print_token(const Token* token);

#endif
