#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verificar que hay al menos un archivo
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }
    
    FILE *archivo;
    int count = 0;
    char prev = '\0';  // Carácter anterior
    char curr;         // Carácter actual
    
    // Procesar cada archivo
    for (int i = 1; i < argc; i++) {
        archivo = fopen(argv[i], "r");
        
        if (archivo == NULL) {
            printf("wzip: cannot open file\n");
            return 1;
        }
        
        // Leer carácter por carácter
        while (fread(&curr, sizeof(char), 1, archivo) == 1) {
            if (count == 0) {
                // Primer carácter
                prev = curr;
                count = 1;
            } else if (curr == prev) {
                // Mismo carácter, incrementar contador
                count++;
            } else {
                // Carácter diferente, escribir el anterior
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
                prev = curr;
                count = 1;
            }
        }
        
        fclose(archivo);
    }
    
    // Escribir el último grupo si existe
    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
    }
    
    return 0;
}
