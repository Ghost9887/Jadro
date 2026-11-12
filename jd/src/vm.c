#include "vm.h"

void initialize_vm(VM* vm, const char* content) {
    vm->content = content;
    vm->ip = 0;
}

void run(VM* vm) {}
