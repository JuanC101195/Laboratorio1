# 🖥️ Laboratorio 1 - Comandos UNIX en C

**Estudiante:** Juan Camilo  
**Repositorio:** [github.com/JuanC101195/Laboratorio1](https://github.com/JuanC101195/Laboratorio1)  
**Curso:** Sistemas Operativos  
**Fecha:** Marzo 2026

---

## 📋 Descripción General

Este laboratorio consiste en la implementación de 4 programas en C que imitan comandos básicos de UNIX. Los programas desarrollados son:

1. **wcat** - Concatena y muestra el contenido de archivos
2. **wgrep** - Busca patrones de texto en archivos
3. **wzip** - Comprime archivos usando Run-Length Encoding (RLE)
4. **wunzip** - Descomprime archivos generados por wzip

Todos los programas siguen las convenciones de UNIX para códigos de salida:
- `0` = Ejecución exitosa
- `1` = Error (archivo no encontrado, argumentos incorrectos, etc.)

---

## 📁 Estructura del Proyecto

```
Laboratorio1/
├── bin/                    # Ejecutables compilados
│   ├── wcat
│   ├── wgrep
│   ├── wzip
│   └── wunzip
├── data/                   # Archivos de prueba
│   ├── archivo1.txt
│   ├── archivo2.txt
│   ├── prueba_grep.txt
│   ├── repite.txt
│   ├── repite.z           # Archivo comprimido
│   ├── combinado.z        # Archivo comprimido
│   ├── recuperado.txt
│   ├── vacio.txt
│   ├── una.txt
│   └── variado.txt
├── src/                    # Código fuente
│   ├── wcat.c
│   ├── wgrep.c
│   ├── wzip.c
│   └── wunzip.c
└── README.md
```

---

## 🔧 Compilación

Para compilar todos los programas:

```bash
# Compilar wcat
gcc -o bin/wcat src/wcat.c -Wall -Werror

# Compilar wgrep
gcc -o bin/wgrep src/wgrep.c -Wall -Werror

# Compilar wzip
gcc -o bin/wzip src/wzip.c -Wall -Werror

# Compilar wunzip
gcc -o bin/wunzip src/wunzip.c -Wall -Werror
```

---

## 📖 Programas Implementados

### 1️⃣ wcat - Concatenador de Archivos

**Descripción:** Imprime el contenido de uno o más archivos a la salida estándar.

**Uso:**
```bash
./bin/wcat archivo1.txt archivo2.txt
```

**Código Principal:**
```c
int main(int argc, char *argv[]) {
    // Si no hay archivos, salir normalmente (código 0)
    if (argc <= 1) {
        return 0;
    }
    
    char buffer[1024];
    
    // Recorrer cada archivo que el usuario pasó como argumento
    for (int i = 1; i < argc; i++) {
        FILE *archivo = fopen(argv[i], "r");
        
        if (archivo == NULL) {
            printf("wcat: cannot open file\n");
            return 1;  // Código de error
        }
        
        // Leer el archivo línea por línea y mostrarlo
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            printf("%s", buffer);
        }
        
        fclose(archivo);
    }
    
    return 0;
}
```

**Características:**
- ✅ Manejo de múltiples archivos
- ✅ Buffer de 1024 bytes para lectura eficiente
- ✅ Validación de apertura de archivos
- ✅ Retorna 0 si no se pasan argumentos (comportamiento estándar de cat)

---

### 2️⃣ wgrep - Buscador de Patrones

**Descripción:** Busca un término en archivos o entrada estándar y muestra las líneas que lo contienen.

**Uso:**
```bash
# Buscar en archivos
./bin/wgrep mundo data/prueba_grep.txt

# Buscar desde stdin
echo "hola mundo" | ./bin/wgrep mundo
```

**Código Principal:**
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    
    char *termino = argv[1];
    FILE *archivo;
    char *linea = NULL;
    size_t len = 0;
    ssize_t lectura;
    
    // CASO 1: Solo término, sin archivos -> leer de stdin
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
        
        if (archivo == NULL) {
            printf("wgrep: cannot open file\n");
            free(linea);
            return 1;
        }
        
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
```

**Características:**
- ✅ Uso de `getline()` para manejar líneas de cualquier longitud
- ✅ Soporte para entrada desde stdin
- ✅ Búsqueda con `strstr()` (case-sensitive)
- ✅ Manejo correcto de memoria (free)
- ✅ Procesamiento de múltiples archivos

---

### 3️⃣ wzip - Compresor RLE

**Descripción:** Comprime archivos usando Run-Length Encoding en formato binario.

**Formato de salida:** `[int count (4 bytes)][char (1 byte)]`

**Uso:**
```bash
./bin/wzip data/repite.txt > data/repite.z
```

**Código Principal:**
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }
    
    FILE *archivo;
    int count = 0;
    char prev = '\0';
    char curr;
    
    for (int i = 1; i < argc; i++) {
        archivo = fopen(argv[i], "r");
        
        if (archivo == NULL) {
            printf("wzip: cannot open file\n");
            return 1;
        }
        
        while (fread(&curr, sizeof(char), 1, archivo) == 1) {
            if (count == 0) {
                prev = curr;
                count = 1;
            } else if (curr == prev) {
                count++;
            } else {
                // Escribir el grupo anterior
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
                prev = curr;
                count = 1;
            }
        }
        
        fclose(archivo);
    }
    
    // Escribir el último grupo
    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
    }
    
    return 0;
}
```

**Características:**
- ✅ Compresión en formato binario
- ✅ Usa `int` de 4 bytes para el contador
- ✅ Procesa múltiples archivos de forma continua
- ✅ Escritura directa a stdout (permite redirección)

**Ejemplo de compresión:**
```
Input:  "aaaaabbbbbccccc"
Output: [5][a][5][b][5][c]  (en binario)
```

---

### 4️⃣ wunzip - Descompresor RLE

**Descripción:** Descomprime archivos `.z` generados por wzip.

**Uso:**
```bash
./bin/wunzip data/repite.z
```

**Código Principal:**
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }
    
    FILE *archivo;
    int count;
    char ch;
    
    for (int i = 1; i < argc; i++) {
        archivo = fopen(argv[i], "rb");  // Modo binario
        
        if (archivo == NULL) {
            printf("wunzip: cannot open file\n");
            return 1;
        }
        
        // Leer pares (count, char) del archivo binario
        while (fread(&count, sizeof(int), 1, archivo) == 1) {
            if (fread(&ch, sizeof(char), 1, archivo) != 1) {
                fclose(archivo);
                return 1;  // Archivo corrupto
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
```

**Características:**
- ✅ Apertura en modo binario (`"rb"`)
- ✅ Validación de integridad del archivo
- ✅ Soporte para múltiples archivos
- ✅ Expansión correcta de secuencias

---

## 🧪 Pruebas Realizadas

### Tabla de Pruebas - wcat

| Prueba | Comando | Resultado Esperado | Estado |
|--------|---------|-------------------|--------|
| Sin argumentos | `./bin/wcat` | Salida vacía (código 0) | ✅ |
| Un archivo | `./bin/wcat data/archivo1.txt` | Contenido del archivo | ✅ |
| Múltiples archivos | `./bin/wcat data/archivo1.txt data/archivo2.txt` | Contenido concatenado | ✅ |
| Archivo inexistente | `./bin/wcat noexiste.txt` | "wcat: cannot open file" (código 1) | ✅ |

### Tabla de Pruebas - wgrep

| Prueba | Comando | Resultado Esperado | Estado |
|--------|---------|-------------------|--------|
| Sin argumentos | `./bin/wgrep` | "wgrep: searchterm [file ...]" | ✅ |
| Desde stdin | `echo "hola mundo" \| ./bin/wgrep mundo` | "hola mundo" | ✅ |
| Buscar en archivo | `./bin/wgrep mundo data/prueba_grep.txt` | Líneas con "mundo" | ✅ |
| Término no encontrado | `./bin/wgrep xyz data/archivo1.txt` | Sin salida | ✅ |
| Archivo inexistente | `./bin/wgrep test noexiste.txt` | Error (código 1) | ✅ |

### Tabla de Pruebas - wzip / wunzip

| Prueba | Comandos | Resultado Esperado | Estado |
|--------|----------|-------------------|--------|
| Comprimir texto repetitivo | `./bin/wzip data/repite.txt > data/repite.z` | Archivo binario comprimido | ✅ |
| Descomprimir | `./bin/wunzip data/repite.z` | "aaaaabbbbbccccc" | ✅ |
| Ciclo completo | `./bin/wzip data/archivo1.txt > temp.z && ./bin/wunzip temp.z` | Contenido original restaurado | ✅ |
| Múltiples archivos | `./bin/wzip data/una.txt data/repite.txt > data/combinado.z` | Archivo comprimido combinado | ✅ |
| wzip sin argumentos | `./bin/wzip` | "wzip: file1 [file2 ...]" | ✅ |
| wunzip sin argumentos | `./bin/wunzip` | "wunzip: file1 [file2 ...]" | ✅ |

### Validación de Integridad

```bash
# Prueba de integridad: comprimir y descomprimir debe dar el mismo resultado
./bin/wzip data/variado.txt > temp.z
./bin/wunzip temp.z > data/recuperado.txt
diff data/variado.txt data/recuperado.txt  # Sin diferencias ✅
```

---

## 🐛 Problemas Encontrados y Soluciones

### Problema 1: Líneas largas en wgrep
**Descripción:** Inicialmente usé `fgets()` con un buffer fijo, lo que limitaba el tamaño de las líneas.

**Solución:** Implementé `getline()` que asigna memoria dinámicamente según el tamaño de la línea:
```c
char *linea = NULL;
size_t len = 0;
getline(&linea, &len, stdin);  // Maneja cualquier longitud
free(linea);  // Importante: liberar memoria
```

### Problema 2: Formato binario en wzip
**Descripción:** Primero intenté escribir en texto, pero no era eficiente ni cumplía la especificación.

**Solución:** Usé `fwrite()` para escribir directamente en binario:
```c
fwrite(&count, sizeof(int), 1, stdout);  // 4 bytes para el contador
fwrite(&prev, sizeof(char), 1, stdout);  // 1 byte para el carácter
```

### Problema 3: Lectura binaria en wunzip
**Descripción:** Al intentar leer con `fgets()` se corrompía el archivo.

**Solución:** Abrir en modo binario (`"rb"`) y usar `fread()`:
```c
FILE *archivo = fopen(argv[i], "rb");  // Modo binario
fread(&count, sizeof(int), 1, archivo);
```

### Problema 4: Último grupo en wzip
**Descripción:** El último grupo de caracteres repetidos no se escribía al archivo.

**Solución:** Agregué escritura después del loop:
```c
// Escribir el último grupo si existe
if (count > 0) {
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&prev, sizeof(char), 1, stdout);
}
```

### Problema 5: Fugas de memoria en wgrep
**Descripción:** `getline()` asigna memoria que debe liberarse.

**Solución:** Llamar `free(linea)` antes de cada retorno:
```c
free(linea);
return 0;
```

---

## 💡 Decisiones de Diseño

### 1. Manejo de errores consistente
Todos los programas siguen el mismo patrón:
- Validar argumentos al inicio
- Verificar apertura de archivos antes de operar
- Retornar código 1 en caso de error

### 2. Eficiencia en wcat
Usé `fgets()` con buffer de 1024 bytes en lugar de `fgetc()` para mejor rendimiento en archivos grandes.

### 3. Flexibilidad en wgrep
Implementé dos modos de operación (stdin y archivos) para máxima compatibilidad con el estándar UNIX.

### 4. Formato binario estricto
wzip/wunzip usan `int` de 4 bytes + `char` de 1 byte para compatibilidad multiplataforma.

---

## 🎓 Conceptos Aprendidos

### Manejo de Archivos en C
- `fopen()`, `fclose()` para gestión de archivos
- Diferencia entre modo texto (`"r"`) y binario (`"rb"`)
- `fgets()` vs `getline()` para lectura de líneas
- `fread()` / `fwrite()` para datos binarios

### Gestión de Memoria
- Asignación dinámica con `getline()`
- Importancia de `free()` para evitar memory leaks
- `sizeof()` para tamaños de tipos de datos

### Argumentos de Línea de Comandos
- `argc` (argument count) y `argv` (argument vector)
- Validación de argumentos antes de procesar

### Códigos de Salida UNIX
- `0` = éxito
- `1` = error genérico

### Compresión de Datos
- Run-Length Encoding (RLE)
- Formato binario vs texto
- Trade-offs de compresión

---

## 🤖 Manifiesto de Transparencia - Uso de IA

Durante el desarrollo de este laboratorio, utilicé herramientas de IA (GitHub Copilot, ChatGPT) para:

### ✅ Usos Apropiados
1. **Comprensión de funciones:** Consulté documentación sobre `getline()`, `fread()`, `fwrite()`
2. **Debugging:** Analizar errores de compilación y segmentation faults
3. **Optimización:** Sugerencias para mejorar eficiencia del código
4. **Formato binario:** Entender cómo escribir/leer datos binarios correctamente
5. **Este README:** Estructura y formato de documentación profesional

### 🧠 Trabajo Original
1. **Lógica de los algoritmos:** Diseñé la lógica de RLE y búsqueda
2. **Manejo de casos especiales:** Identifiqué y resolví edge cases
3. **Pruebas:** Creé todos los archivos de prueba y validaciones
4. **Debugging:** Identifiqué y corregí bugs de forma independiente
5. **Integración:** Adapté sugerencias de IA a los requisitos específicos del laboratorio

### 📚 Aprendizaje
El uso de IA fue complementario a mi aprendizaje, no un sustituto. Cada función implementada fue comprendida a fondo antes de integrarla al proyecto.

---

## 🎥 Video de Sustentación

> **Enlace:** [Próximamente - Video de 10 minutos]

### Temas a Cubrir en el Video:
1. Demostración de cada programa (2 min)
2. Explicación del algoritmo RLE en wzip/wunzip (2 min)
3. Manejo de memoria dinámica en wgrep (2 min)
4. Casos especiales y manejo de errores (2 min)
5. Pruebas de integridad y validaciones (2 min)

---

## 📦 Cómo Ejecutar

### Prerrequisitos
- GCC (GNU Compiler Collection)
- Sistema operativo Linux/UNIX o WSL

### Pasos
```bash
# 1. Clonar el repositorio
git clone https://github.com/JuanC101195/Laboratorio1.git
cd Laboratorio1

# 2. Compilar (si es necesario)
gcc -o bin/wcat src/wcat.c -Wall -Werror
gcc -o bin/wgrep src/wgrep.c -Wall -Werror
gcc -o bin/wzip src/wzip.c -Wall -Werror
gcc -o bin/wunzip src/wunzip.c -Wall -Werror

# 3. Probar los programas
./bin/wcat data/archivo1.txt
./bin/wgrep mundo data/prueba_grep.txt
./bin/wzip data/repite.txt > output.z
./bin/wunzip output.z
```

---

## 📚 Referencias

- [The C Programming Language (K&R)](https://www.amazon.com/Programming-Language-2nd-Brian-Kernighan/dp/0131103628)
- [Linux man pages](https://man7.org/linux/man-pages/)
- [GNU C Library Documentation](https://www.gnu.org/software/libc/manual/)
- Material del curso de Sistemas Operativos

---

## ✅ Checklist de Requisitos

- [x] wcat implementado y funcional
- [x] wgrep con soporte para stdin y archivos
- [x] wzip con compresión RLE binaria
- [x] wunzip con descompresión correcta
- [x] Manejo de errores en todos los programas
- [x] Códigos de salida correctos (0 y 1)
- [x] Uso de `getline()` para líneas largas
- [x] Formato binario: int (4 bytes) + char (1 byte)
- [x] Pruebas exhaustivas realizadas
- [x] Código compilado sin warnings (`-Wall -Werror`)
- [x] README.md completo
- [x] Código subido a GitHub
- [ ] Video de sustentación grabado

---

## 👤 Autor

**Juan Camilo**  
GitHub: [@JuanC101195](https://github.com/JuanC101195)  
Repositorio: [Laboratorio1](https://github.com/JuanC101195/Laboratorio1)

---

## 📄 Licencia

Este proyecto es parte de un trabajo académico para el curso de Sistemas Operativos.

---

**Última actualización:** Marzo 2026
