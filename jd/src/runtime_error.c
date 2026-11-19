#include "runtime_error.h"
#include <stdio.h>
#include <stdlib.h>

void runtime_error(const char* message) {
    printf("[CHYBA]: %s\n", message);
    exit(1);
}
