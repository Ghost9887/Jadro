#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    unsigned int line, col, start, current;   
    const char* input;
    TokenArr* token_arr;
}Lexer;

void initialize_lexer(Lexer* lexer, TokenArr* arr, const char* input);
void tokenize(Lexer* lexer);
void lexer_test();

#endif
