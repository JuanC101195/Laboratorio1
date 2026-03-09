#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verificar que hay al menos un archivo
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }
    
    FILE *archivo;
    int count;
    char ch;
    
    // Procesar cada archivo
    for (int i = 1; i < argc; i++) {
        archivo = fopen(argv[i], "rb");  // Modo binario
        
        if (archivo == NULL) {
            printf("wunzip: cannot open file\n");
            return 1;
        }
        
        // Leer pares (count, char) del archivo binario
        while (fread(&count, sizeof(int), 1, archivo) == 1) {
            if (fread(&ch, sizeof(char), 1, archivo) != 1) {
                // Archivo corrupto
                fclose(archivo);
                return 1;
            }
            
            // Imprimir el carácter 'count' veces
            for (int j = 0; j < count; j++) {
                printf("%c", ch);
            }
        }
        
        fclose(archivo);
    }
    
    return 0;
}
