#include "jadro.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

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
    for (int i = 0; i < (int)tokens.count; i++) {
        print_token(&tokens.tokens[i]);
    }
#endif

    StmntArr stmnts;
    if (!initialize_stmnt_arr(&stmnts)) return;

    Parser parser;
    initialize_parser(&parser, &stmnts, lexer.token_arr);
    parse(&parser);

    free_token_arr(&tokens);
    free_stmnt_arr(&stmnts);
#endif

#ifdef TESTS
    lexer_test();
#endif
}
