#ifndef VM_H
#define VM_H

#include "stack.h"
#include "environment.h"

#define MAX_ENVS 256

typedef struct {
    const char* bytes;
    unsigned int bytes_len;
    unsigned int ip;
    Stack stack;
    Env env_arr[MAX_ENVS];
    unsigned int current_env;
}VM;

void initialize_vm(VM* vm, const char* content);
void run(VM* vm);

#endif
