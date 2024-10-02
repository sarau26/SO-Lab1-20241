#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Función para verificar si los dos archivos son iguales
int compareFiles(const char *file1, const char *file2) {
    struct stat stat1, stat2;
    if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0) {
        return -1;
    }
    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino) ? 1 : 0;
}

void reverseLines(FILE *file_in, FILE *file_out) {
    char **lines = NULL;
    int line_count = 0;
    int capacity = 10;

    // Asigna memoria para el array de líneas
    lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Lee cada línea del archivo de entrada y la almacena en el array
    char *line = NULL;
    size_t length = 0;
    while (getline(&line, &length, file_in) > 0) {
        // Elimina el salto de línea al final si existe
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0'; // Reemplaza el '\n' con '\0'
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

    // Escribe las líneas en orden inverso en el archivo de salida
    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(file_out, "%s\n", lines[i]);
        free(lines[i]); // Libera cada línea almacenada
    }

    free(lines); // Libera el array
}

int main(int argc, char *argv[]) {
    FILE *file_in = NULL;
    FILE *file_out = NULL;

    if (argc == 1) {
        // Sin argumentos: leer de stdin
        file_in = stdin;
        file_out = stdout;
    } else if (argc == 2) {
        // Un argumento: leer de un archivo y mostrar en stdout
        file_in = fopen(argv[1], "r");
        if (file_in == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            return 1;
        }
        file_out = stdout;
    } else if (argc == 3) {
        // Dos argumentos: leer de un archivo y escribir en otro
        if (compareFiles(argv[1], argv[2]) == 1) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            return 1;
        }
        file_in = fopen(argv[1], "r");
        if (file_in == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            return 1;
        }
        file_out = fopen(argv[2], "w");
        if (file_out == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            fclose(file_in);
            return 1;
        }
    } else {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    // Llama a la función para invertir líneas
    reverseLines(file_in, file_out);

    // Cierra los archivos si son distintos de stdin y stdout
    if (file_in != stdin) fclose(file_in);
    if (file_out != stdout) fclose(file_out);
    
    return 0;
}
