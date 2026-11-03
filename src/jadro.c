#include "jadro.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG
//#define TESTS

void run(const char* input) {
#ifndef TESTS
    TokenArr tokens;
    if (!initialize_token_arr(&tokens)) return;

    Lexer lexer;
    initialize_lexer(&lexer, &tokens, input);
    tokenize(&lexer);
#ifdef DEBUG
    printf("\n---TOKENS---\n");
    for (int i = 0; i < (int)tokens.count; i++) {
        print_token(&tokens.tokens[i]);
    }
#endif

    StmntArr stmnts;
    if (!initialize_stmnt_arr(&stmnts)) return;

    Parser parser;
    initialize_parser(&parser, &stmnts, lexer.token_arr);
    parse(&parser);

#ifdef DEBUG
    printf("\n---AST---\n");
    for (int i = 0; i < (int)stmnts.count; i++) {
        print_stmnt(&stmnts.stmnts[i]);
    }
#endif

    //free_token_arr(&tokens);
    //free_stmnt_arr(&stmnts);
#endif

#ifdef TESTS
    lexer_test();
#endif
}
