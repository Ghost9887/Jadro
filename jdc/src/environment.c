#include "environment.h"
#include <stdlib.h>
#include <string.h>

void new_env(Env* env, Env* parent) {
    *env = (Env) { .parent = parent };
    initialize_var_arr(&env->var_arr);
}

void destroy_env(Env* env) {
    free_var_arr(&env->var_arr);
}

bool add_variable(Env* env, String* name, ValueTag value) {
    for (unsigned int i = 0; i < env->var_arr.count; i++) {
        String* cur = &env->var_arr.variables[i].name;
        if (memcmp(cur->ptr, name->ptr, cur->len) == 0) return false;
    }

    reallocate_var_arr(&env->var_arr);
    env->var_arr.variables[env->var_arr.count++] = 
        (Variable) { .name = (String) { .ptr = strdup(name->ptr), .len = name->len }, .value = value };
    return true;
}

Variable* get_variable(Env* env, String* name) {
    for (unsigned int i = 0; i < env->var_arr.count; i++) {
        String* cur = &env->var_arr.variables[i].name;
        if (memcmp(cur->ptr, name->ptr, cur->len) == 0) 
            return &env->var_arr.variables[i];
    }
    if (env->parent != NULL) {
        return get_variable(env->parent, name);
    }
    return NULL;
}
