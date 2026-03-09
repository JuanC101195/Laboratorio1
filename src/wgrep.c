#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Si no hay término de búsqueda, mostrar error
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    
    char *termino = argv[1];  // La palabra a buscar
    FILE *archivo;
    char *linea = NULL;
    size_t len = 0;
    ssize_t lectura;
    
    // CASO 1: Solo término, sin archivos -> leer de teclado (stdin)
    if (argc == 2) {
        while ((lectura = getline(&linea, &len, stdin)) != -1) {
            if (strstr(linea, termino) != NULL) {
                printf("%s", linea);
            }
        }
        free(linea);
        return 0;
    }
    
    // CASO 2: Con archivos
    for (int i = 2; i < argc; i++) {
        archivo = fopen(argv[i], "r");
        
        // Si no se puede abrir el archivo
        if (archivo == NULL) {
            printf("wgrep: cannot open file\n");
            free(linea);
            return 1;
        }
        
        // Leer línea por línea buscando el término
        while ((lectura = getline(&linea, &len, archivo)) != -1) {
            if (strstr(linea, termino) != NULL) {
                printf("%s", linea);
            }
        }
        
        fclose(archivo);
    }
    
    free(linea);
    return 0;
}
