#include "token.h"
#include <stdlib.h>
#include <stdio.h>

bool initialize_token_arr(TokenArr* arr) {
    arr->count = 0;
    arr->capacity = 2;
    arr->tokens = (Token*)malloc(sizeof(Token) * arr->capacity);

    if (arr->tokens == NULL) {
        printf("Nepodarilo sa vytvorit pole Token\n");
        return false;
    }

    return true;
}

bool reallocate_token_arr(TokenArr* arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->tokens = (Token*)realloc(arr->tokens, sizeof(Token) * arr->capacity);

        if (arr->tokens == NULL) {
            printf("Nepodarilo sa zvacsit pole\n");
            return false;
        }
    }
    return true;
}

void free_token_arr(TokenArr* arr) {
    free(arr->tokens);
    arr->tokens = NULL;
}

char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_LPAREN: return "LPAREN"; 
        case TOKEN_RPAREN: return "RPAREN"; 
        case TOKEN_LBRACE: return "LBRACE"; 
        case TOKEN_RBRACE: return "RBRACE"; 
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PERCENT: return "PERCENT";
        case TOKEN_EQUALS: return "EQUALS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESSER: return "LESSER";
        case TOKEN_BANG: return "BANG";
        case TOKEN_EQUALS_EQUALS: return "EQUALS_EQUALS";
        case TOKEN_GREATER_EQUALS: return "GREATER_EQUALS";
        case TOKEN_LESSER_EQUALS: return "LESSER_EQUALS";
        case TOKEN_BANG_EQUALS: return "BANG_EQUALS";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_DOUBLE: return "DOUBLE";
        case TOKEN_BOOLEAN: return "BOOLEAN";
        case TOKEN_NIL: return "NIL";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_PREM: return "PREM";
        case TOKEN_VYTLAC: return "VYTLAC";
        case TOKEN_AK: return "AK";
        case TOKEN_INAK: return "INAK";
        case TOKEN_POKIAL: return "POKIAL";
        case TOKEN_PRE: return "PRE";
        case TOKEN_UKNOWN: return "!!UKNOWN!!";
    }

    return NULL;
}

void print_token(const Token* token) {
    printf("Token[ type: %s | line: %u | col: %u | lexeme: \"%.*s\" | value: ",
        token_type_to_string(token->type),
        token->line,
        token->col,
        token->lexeme.len,
        token->lexeme.ptr
    );
    print_value(&token->value);
    printf(" ]\n");
}
