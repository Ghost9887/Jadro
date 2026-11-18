#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "variable.h"

typedef struct Env{
    struct Env* parent;   
    VariableArr var_arr;
}Env;

void new_env(Env* env, Env* parent);
void destroy_env(Env* env);
bool add_variable(Env* env, String* name, ValueTag value);
Variable* get_variable(Env* env, String* name);

#endif
