#include "vm.h"
#include "value.h"
#include <stdbool.h>
#include <string.h>

void initialize_vm(VM* vm, const char* content) {
    vm->ip = 0;
    vm->content_len = strlen(content);
    vm->content = content;
    initialize_stack(&vm->stack);
}

bool is_at_end(VM* vm);
OpCode get_code(VM* vm);

void run(VM* vm) {
    while (!is_at_end(vm)) {
        OpCode code = get_code(vm);
        switch (code) {
            default: break;
        }
    }
}

bool is_at_end(VM* vm) {
    return vm->ip < vm->content_len;
}

OpCode get_code(VM* vm) {
    return (OpCode)vm->content[vm->ip++];
}
