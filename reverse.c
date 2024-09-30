#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverseLines(char *filename_in, char *filename_out) {
    FILE *file_in, *file_out;
    char **lines = NULL;
    int line_count = 0;
    int capacity = 10;

    // Verifica que el archivo de entrada y salida no sean el mismo
    if (strcmp(filename_in, filename_out) == 0) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    // Abre el archivo de entrada para lectura
    file_in = fopen(filename_in, "r");
    if (file_in == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename_in);
        exit(1);
    }

    // Asigna memoria para el array de líneas
    lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Lee cada línea del archivo de entrada y la almacena en el array
    char *line = NULL;
    size_t length = 0;
    while (getline(&line, &length, file_in) != -1) {
        // Elimina el salto de línea al final si existe
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0'; // Reemplaza el '\n' con '\0' caracter de terminación de cadena
        }

        if (line_count >= capacity) {
            capacity *= 2; 
            lines = realloc(lines, capacity * sizeof(char *));
            if (lines == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        lines[line_count++] = strdup(line); // Almacena una copia de la línea
    }
    free(line); // Libera el buffer de línea
    fclose(file_in);

    // Abre el archivo de salida para escritura
    file_out = fopen(filename_out, "w");
    if (file_out == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", filename_out);
        exit(1);
    }

    // Escribe las líneas en orden inverso en el archivo de salida
    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(file_out, "%s\n", lines[i]);
        free(lines[i]); // Libera cada línea almacenada
    }

    fclose(file_out);
    free(lines); // Libera el array
}

int main(int argc, char *argv[]) {

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    // Valida si el archivo de entrada existe
    FILE *file_check = fopen(input_filename, "r");
    if (file_check == NULL) {
        // Reemplaza el caracter especial en el nombre del archivo
        char *cleaned_filename = input_filename;
        while (*cleaned_filename) {
            if (*cleaned_filename == '\r') {
                *cleaned_filename = '\0';
                break;
            }
            cleaned_filename++;
        }
        fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
        return 1;
    }
    fclose(file_check);

    //Si se le entregan mas o menos de 3 argumentos, falla
    if (argc != 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    // Llama a la función para invertir líneas
    reverseLines(input_filename, output_filename);
    
    return 0;
}
