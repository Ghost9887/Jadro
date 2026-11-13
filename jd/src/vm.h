#ifndef VM_H
#define VM_H

#include "vmstack.h"

typedef struct {
    const char* bytes;
    unsigned int bytes_len;
    unsigned int ip;
    VMStack stack;
}VM;

void initialize_vm(VM* vm, const char* content);
void run(VM* vm);

#endif
