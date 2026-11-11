#include "jadro.h"
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG
//#define TESTS

const char* CONTENT_PTR = NULL;
bool HAD_ERROR = false;

void run(const char* input, const char* output_file_name) {
#ifndef TESTS
    CONTENT_PTR = input;
    TokenArr tokens;
    if (!initialize_token_arr(&tokens)) return;

    Lexer lexer;
    initialize_lexer(&lexer, &tokens, CONTENT_PTR);
    tokenize(&lexer);
    if (HAD_ERROR) goto cleanup;

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
    if (HAD_ERROR) goto cleanup;
#ifdef DEBUG
    printf("\n---AST---\n");
    for (int i = 0; i < (int)stmnts.count; i++) {
        print_stmnt(&stmnts.stmnts[i]);
        printf("\n");
    }
#endif

    Compiler compiler;
    initialize_compiler(&compiler, &stmnts, output_file_name);
    compile(&compiler);
cleanup:
    //free_token_arr(&tokens);
    //free_stmnt_arr(&stmnts);
#endif

#ifdef TESTS
    lexer_test();
#endif
}

//TODO: REFACTOR!!!!
void print_line(Token* token) {
    const char* temp; 
    const char* start; 
    const char* end;

    temp = token->lexeme.ptr;
    while (true) {
        if (*temp == '\n') {
            start = ++temp;
            break;
        }else if (temp == CONTENT_PTR) {
            start = temp;
            break;
        }
        temp--;
    }

    temp = token->lexeme.ptr;
    while (*temp != '\n' && *temp != '\0') temp++;
    end = temp;

    printf("%.*s\n", (int)(end - start), start);
    char pointy[1024];

    int max = (int)(token->col + token->lexeme.len - 1);
    if (max > (int)(sizeof(pointy) - 1)) max = (int)(sizeof(pointy) - 1);

    int i;
    for (i = 0; i < max; i++) {
        pointy[i] = '-';
    }

    pointy[i] = '^';
    pointy[++i] = '\0';
    printf("%s\n", pointy);
}

void jadro_error(Token* token, const char* message) {
    HAD_ERROR = true;
    printf("\n[CHYBA]: %s\n", message); 
    printf("riadok: %u, stlpec: %u\n", 
        token->line, 
        token->col + token->lexeme.len - 1
    );
    print_line(token);
}
