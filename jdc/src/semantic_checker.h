#ifndef SEMANTIC_CHECKER_H
#define SEMANTIC_CHECKER_H

#include "stack.h"
#include "environment.h"

#define MAX_ENVS 256

typedef struct StmntArr StmntArr;

typedef struct {
    Stack stack;
    Env env_arr[MAX_ENVS];
    unsigned int current_env;
}SemanticChecker;

void initialize_semantic_checker(SemanticChecker* checker);
void semantic_check(SemanticChecker* checker, StmntArr* stmnts);

#endif
