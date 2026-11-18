#include "variable.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_var_arr(VariableArr* arr) {
    arr->count = 0;
    arr->capacity = 2;
    arr->variables = (Variable*)malloc(sizeof(Variable) * arr->capacity);

    if (arr->variables == NULL) {
        printf("Nepodarilo sa allocovat Variable Arr\n");
        exit(1);
    }
}

void reallocate_var_arr(VariableArr* arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->variables = (Variable*)realloc(arr->variables, sizeof(Variable) * arr->capacity);
        if (arr->variables == NULL) {
            printf("Nepodarilo sa reallocovat Variable Arr\n");
            exit(1);
        }
    }
}

void free_var_arr(VariableArr* arr) {
    for (unsigned int i = 0; i < arr->count; i++) {
        free(arr->variables[i].value);
        free((char*)arr->variables[i].name.ptr);
    }
    free(arr->variables);
}
