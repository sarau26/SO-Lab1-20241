#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readLine(FILE *file) {
    int buffer_size = 100; 
    int length = 0;        
    char *buffer = malloc(buffer_size * sizeof(char));

    if (buffer == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    char c;
    while ((c = fgetc(file)) != EOF && c != '\n') {
        buffer[length++] = c;

        if (length >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size * sizeof(char));

            if (buffer == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
    }

    buffer[length] = '\0';

    if (length == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

void reverseLines(char *filename_in, char *filename_out) {
    FILE *file_in, *file_out;
    char **lines = NULL;
    int line_count = 0;
    int capacity = 10;

    if (strcmp(filename_in, filename_out) == 0) {
        fprintf(stderr, "El archivo de entrada y salida deben diferir\n");
        exit(1);
    }

    file_in = fopen(filename_in, "r");
    if (file_in == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", filename_in);
        exit(1);
    }

    lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    char *line;
    while ((line = readLine(file_in)) != NULL) {
        if (line_count >= capacity) {
            capacity *= 2; 
            lines = realloc(lines, capacity * sizeof(char *));
            if (lines == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        lines[line_count++] = line;
    }
    fclose(file_in);

    file_out = fopen(filename_out, "w");
    if (file_out == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", filename_out);
        exit(1);
    }

    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(file_out, "%s\n", lines[i]);
        free(lines[i]);
    }

    fclose(file_out);
    free(lines);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    reverseLines(input_filename, output_filename);

    printf("El archivo se ha invertido y guardado en %s\n", output_filename);
    
    return 0;
}
