#ifndef VARIABLE_H
#define VARIABLE_H

#include "value.h"

typedef struct {
    String name;
    ValueTag value;
}Variable;

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Variable* variables;
}VariableArr;

void initialize_var_arr(VariableArr* arr);
void reallocate_var_arr(VariableArr* arr);
void free_var_arr(VariableArr* arr);

#endif
