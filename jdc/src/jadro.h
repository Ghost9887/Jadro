#ifndef JADRO_H
#define JADRO_H

#include "token.h"

void run(const char* input, const char* output_file_name);
void syntax_error(Token* token, const char* message);
void compile_error();

#endif
