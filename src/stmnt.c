#include "stmnt.h"
#include <stdlib.h>
#include <stdio.h>

bool initialize_stmnt_arr(StmntArr* arr) {
    arr->count = 0;
    arr->capacity = 2;
    arr->stmnts = (Stmnt*)malloc(sizeof(Stmnt) * arr->capacity);

    if (arr->stmnts == NULL) {
        printf("Nepodarilo sa vytvorit pole Statement\n");
        return false;
    }

    return true;
}

bool reallocate_stmnt_arr(StmntArr* arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->stmnts = (Stmnt*)realloc(arr->stmnts, sizeof(Stmnt) * arr->capacity);

        if (arr->stmnts == NULL) {
            printf("Nepodarilo sa zvacsit statement pole\n");
            return false;
        }
    }
    return true;
}

void free_stmnt_arr(StmntArr* arr) {
    free(arr->stmnts);
    arr->stmnts = NULL;
}
