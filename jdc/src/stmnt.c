#include "stmnt.h"
#include <stdlib.h>
#include <stdio.h>

void print_stmnt(Stmnt* stmnt) {
    switch (stmnt->tag) {
        case EXPR_STMNT:
            printf("ExprStmnt[");
            print_expr(stmnt->as.exp.expr);
            printf("]");
            break;
        case PRINT_STMNT:
            printf("PrintStmnt[");
            print_expr(stmnt->as.pri.expr);
            printf("]");
            break;
        case BLOCK_STMNT:
            printf("BlockStmnt[");
            for (int i = 0; i < (int)stmnt->as.blo.stmnts->count; i++) {
                print_stmnt(stmnt->as.blo.stmnts->stmnts[i]);
            }
            printf("]");
            break;
        case DECL_STMNT:
            printf("DeclStmnt[ %.*s = ", stmnt->as.dec.name.len, stmnt->as.dec.name.ptr);
            print_expr(stmnt->as.dec.expr);
            printf("]");
            break;
        case ASSIGN_STMNT:
            printf("AssignStmnt[ %.*s = ", stmnt->as.dec.name.len, stmnt->as.dec.name.ptr);
            print_expr(stmnt->as.dec.expr);
            printf("]");
            break;
        case IF_STMNT:
            printf("IfStmnt[ (");
            print_expr(stmnt->as.ifs.condition);
            printf(") { ");
            print_stmnt(stmnt->as.ifs.body);
            if (stmnt->as.ifs.else_body != NULL) {
                printf("} ");
                print_stmnt(stmnt->as.ifs.else_body);
            }
            printf("} ]");
            break;
        case WHILE_STMNT:
            printf("WhileStmnt[ (");
            print_expr(stmnt->as.whi.condition);
            printf(" ) { ");
            print_stmnt(stmnt->as.whi.body);
            printf("} ]");
            break;
    }
}

void initialize_stmnt_arr(StmntArr* arr) {
    arr->count = 0;
    arr->capacity = 2;
    arr->stmnts = (Stmnt**)malloc(sizeof(Stmnt*) * arr->capacity);

    if (arr->stmnts == NULL) {
        printf("Nepodarilo sa vytvorit pole Statement\n");
        exit(1);
    }
}

void reallocate_stmnt_arr(StmntArr* arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->stmnts = (Stmnt**)realloc(arr->stmnts, sizeof(Stmnt*) * arr->capacity);

        if (arr->stmnts == NULL) {
            printf("Nepodarilo sa zvacsit statement pole\n");
            exit(1);
        }
    }
}
