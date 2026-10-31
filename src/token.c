#include "token.h"
#include <stdio.h>

void print_token(const Token* token) {
    printf("Token[ line: %u | col: %u | lexeme: '%s' ]\n", 
        token->line,
        token->col,
        token->lexeme
    ); 
}
