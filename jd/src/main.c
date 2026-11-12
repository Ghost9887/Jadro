#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

void print_help();
char* read_file(const char* file_path);

int main(int argc, char** argv) {
    if (argc != 2) 
        print_help();
    VM vm;
    char* content = read_file(argv[1]);
    if (content == NULL) return 1;
    
    initialize_vm(&vm, content);
    run(&vm);

    free(content);
    return 0;
}

char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Subor '%s' sa nepodaril otvorit\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* input = (char*)malloc(size + 1);
    if (input == NULL) {
        printf("Nepodarilo sa alokovať pamäť\n");
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(input, 1, size, file);
    if (bytes_read != size) {
        printf("Subor '%s' sa nepodarilo precitat\n", file_path);
        fclose(file);
        return NULL;
    }
    input[size] = '\0';

    fclose(file);
    return input;
}

void print_help() {
    printf("Pouzitite: jd <kompilovany_subor>\n");
}
