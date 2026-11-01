#include "jadro.h"
#include "lexer.h"
#include <stdlib.h>

#define DEBUG

void run(const char* input) {
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

    free_token_arr(&tokens);
}
