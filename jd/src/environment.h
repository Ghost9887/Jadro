#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "variable.h"

typedef struct Env{
    struct Env* parent;   
    VariableArr var_arr;
}Env;

void new_env(Env* env, Env* parent);
void destroy_env(Env* env);
void add_variable(Env* env, String* name, Value* value);
Variable* get_variable(Env* env, String* name);

#endif
