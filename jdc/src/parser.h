#ifndef PARSER_H
#define PARSER_H

#include "stmnt.h"
#include "token.h"

typedef struct {
    TokenArr* token_arr;
    StmntArr* stmnt_arr;
    unsigned int ip;
    
}Parser;

void initialize_parser(Parser* parser, StmntArr* stmnt_arr, TokenArr* token_arr);
void parse(Parser* parser);

#endif
