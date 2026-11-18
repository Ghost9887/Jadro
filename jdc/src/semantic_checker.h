#ifndef SEMANTIC_CHECKER_H
#define SEMANTIC_CHECKER_H

#include "stack.h"

typedef struct StmntArr StmntArr;

typedef struct {
    Stack stack;
}SemanticChecker;

bool initialize_semantic_checker(SemanticChecker* checker);
void semantic_check(SemanticChecker* checker, StmntArr* stmnts);

#endif
