#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Si no hay archivos, salir normalmente (código 0)
    if (argc <= 1) {
        return 0;
    }
    
    char buffer[1024];
    
    // Recorrer cada archivo que el usuario pasó como argumento
    for (int i = 1; i < argc; i++) {
        // Intentar abrir el archivo en modo lectura
        FILE *archivo = fopen(argv[i], "r");
        
        // Si no se pudo abrir (archivo no existe, permisos, etc.)
        if (archivo == NULL) {
            printf("wcat: cannot open file\n");
            return 1;  // Código de error
        }
        
        // Leer el archivo línea por línea y mostrarlo
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            printf("%s", buffer);
        }
        
        // Cerrar el archivo
        fclose(archivo);
    }
    
    return 0;  // Todo bien
}

