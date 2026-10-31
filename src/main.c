#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void print_help();
void repl();
void read_file(const char* file_path);

int main(int argc, const char** argv) {
    if (argc == 1) {
        repl();
    }else if (argc == 2) {
        read_file(argv[1]);
    }else {
        print_help();
    }

    return 0;
}

void repl() {
    char input[1024] = {0};

    printf("Pre ukonecnie zadajte 'koniec' alebo 'q'\n");
    while (true) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Nedalo sa nacitat input\n");
            continue;
        }

        if (input[strcspn(input, "\n")] == '\0') {
            printf("Input je priliz velky\n");
            while (getchar() != '\n');
            continue;
        } 
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "koniec") == 0 || strcmp(input, "q") == 0) 
            break;

        printf("%s\n", input);
    }
}

void read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) 
        goto open_error;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* input = (char*)malloc(size + 1);

    if (fgets(input, size, file) == NULL) 
        goto read_error;

    printf("%s\n", input);
    free(input);
    return;

open_error:
    printf("Subor '%s' sa nepodaril otvorit\n", file_path);
    return;

read_error:
    printf("Subor '%s' sa nepodarilo nacitat\n", file_path);
    free(input);
    return;
}

void print_help() {
    printf("Pouzitie: jdc <subor>\n");
}
