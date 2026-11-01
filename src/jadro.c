#include "jadro.h"
#include "lexer.h"
#include <stdlib.h>

void run(const char* input) {
    TokenArr tokens;
    if (!initialize_token_arr(&tokens)) return;

    Lexer lexer;
    initialize_lexer(&lexer, &tokens, input);
    tokenize(&lexer);

    for (int i = 0; i < (int)tokens.count; i++) {
        print_token(&tokens.tokens[i]);
    }

    free_token_arr(&tokens);
}
