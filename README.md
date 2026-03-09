# 🖥️ Laboratorio 1 - Comandos UNIX en C

**Estudiante:** Juan Esteban Cardozo Rivera  
**Repositorio:** [github.com/JuanC101195/Laboratorio1](https://github.com/JuanC101195/Laboratorio1)  
**Curso:** Sistemas Operativos  
**Fecha:** Marzo 2026

## 🔧 Tecnologías

![C](https://img.shields.io/badge/Language-C-blue?logo=c)
![GCC](https://img.shields.io/badge/Compiler-GCC-green)
![Linux](https://img.shields.io/badge/OS-Linux-yellow?logo=linux)
![UNIX](https://img.shields.io/badge/Paradigm-UNIX-orange)
![Git](https://img.shields.io/badge/Version%20Control-Git-red?logo=git)

---

## 📋 Descripción General

Este laboratorio consiste en la implementación de 4 programas en C que imitan comandos básicos de UNIX:

1. **wcat** - Concatena y muestra el contenido de archivos
2. **wgrep** - Busca patrones de texto en archivos o stdin
3. **wzip** - Comprime archivos usando Run-Length Encoding (RLE)
4. **wunzip** - Descomprime archivos .z generados por wzip

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
│   ├── combinado.z
│   └── ...
├── src/                    # Código fuente
│   ├── wcat.c
│   ├── wgrep.c
│   ├── wzip.c
│   └── wunzip.c
└── README.md
```

---

## 🔧 Compilación

```bash
gcc -o bin/wcat src/wcat.c -Wall -Werror
gcc -o bin/wgrep src/wgrep.c -Wall -Werror
gcc -o bin/wzip src/wzip.c -Wall -Werror
gcc -o bin/wunzip src/wunzip.c -Wall -Werror
```

---

## 📖 Programas Implementados

### 1️⃣ wcat - Concatenador de Archivos

**Descripción:** Imprime el contenido de uno o más archivos a stdout.

**Características clave:**
- Maneja múltiples archivos secuencialmente
- Buffer de 1024 bytes para lectura eficiente con `fgets()`
- Retorna 0 si no se pasan argumentos (comportamiento estándar de `cat`)
- Validación de apertura de archivos

**Fragmento clave del código:**
```c
// Validar apertura y leer archivo línea por línea
FILE *archivo = fopen(argv[i], "r");
if (archivo == NULL) {
    printf("wcat: cannot open file\n");
    return 1;
}

while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
    printf("%s", buffer);
}
```

---

### 2️⃣ wgrep - Buscador de Patrones

**Descripción:** Busca un término en archivos o entrada estándar y muestra las líneas que lo contienen.

**Características clave:**
- Usa `getline()` para manejar líneas de **cualquier longitud** (no limitado por buffer fijo)
- Soporta lectura desde stdin cuando no se especifican archivos
- Búsqueda case-sensitive con `strstr()`
- Libera memoria correctamente con `free()`

**Fragmento clave del código:**
```c
// getline asigna memoria dinámicamente según el tamaño de la línea
char *linea = NULL;
size_t len = 0;

while ((lectura = getline(&linea, &len, archivo)) != -1) {
    if (strstr(linea, termino) != NULL) {
        printf("%s", linea);
    }
}

free(linea);  // Importante: liberar memoria asignada
```

---

### 3️⃣ wzip - Compresor RLE

**Descripción:** Comprime archivos usando Run-Length Encoding en **formato binario**.

**Formato de salida:** `[int count (4 bytes)][char (1 byte)]` repetido

**Características clave:**
- Escribe en formato binario con `fwrite()`
- Contador de tipo `int` (4 bytes) para soportar secuencias largas
- Procesa múltiples archivos de forma continua
- Escribe a stdout (permite redirección)

**Fragmento clave del código:**
```c
// Escribir contador (4 bytes) + carácter (1 byte) en binario
fwrite(&count, sizeof(int), 1, stdout);  // int = 4 bytes
fwrite(&prev, sizeof(char), 1, stdout);  // char = 1 byte
```

**Ejemplo:**
```
Input:  "aaaaabbbbbccccc"
Output: [5][a][5][b][5][c][1][\n]  (en binario)
```

---

### 4️⃣ wunzip - Descompresor RLE

**Descripción:** Descomprime archivos `.z` generados por wzip.

**Características clave:**
- Apertura en modo binario (`"rb"`)
- Lee pares (int, char) con `fread()`
- Validación de integridad del archivo
- Soporte para múltiples archivos

**Fragmento clave del código:**
```c
FILE *archivo = fopen(argv[i], "rb");  // Modo binario

while (fread(&count, sizeof(int), 1, archivo) == 1) {
    if (fread(&ch, sizeof(char), 1, archivo) != 1) {
        return 1;  // Archivo corrupto
    }
    
    for (int j = 0; j < count; j++) {
        printf("%c", ch);
    }
}
```

---

## 🧪 Pruebas Realizadas y Resultados

### Pruebas - wcat

#### ✅ Prueba 1: Mostrar un archivo
```bash
$ ./bin/wcat data/archivo1.txt
 Hola mundo
 Esta es la linea 2
Línea 3
```

#### ✅ Prueba 2: Sin argumentos (comportamiento estándar)
```bash
$ ./bin/wcat
$ echo $?
0
```

#### ✅ Prueba 3: Archivo no existe
```bash
$ ./bin/wcat noexiste.txt
wcat: cannot open file
$ echo $?
1
```

#### ✅ Prueba 4: Múltiples archivos
```bash
$ ./bin/wcat data/archivo1.txt data/archivo2.txt
[Contenido de archivo1]
[Contenido de archivo2]
```

---

### Pruebas - wgrep

#### ✅ Prueba 1: Buscar en archivo
```bash
$ ./bin/wgrep mundo data/prueba_grep.txt
Hola mundo
esta línea tiene mundo
mundo aquí también
```

#### ✅ Prueba 2: Desde stdin
```bash
$ echo "hola mundo cruel" | ./bin/wgrep mundo
hola mundo cruel
```

#### ✅ Prueba 3: Sin argumentos
```bash
$ ./bin/wgrep
wgrep: searchterm [file ...]
$ echo $?
1
```

#### ✅ Prueba 4: Término no encontrado
```bash
$ ./bin/wgrep xyz data/archivo1.txt
$ echo $?
0
```
_(Salida vacía, sin error)_

#### ✅ Prueba 5: Archivo no existe
```bash
$ ./bin/wgrep test noexiste.txt
wgrep: cannot open file
$ echo $?
1
```

---

### Pruebas - wzip / wunzip

#### ✅ Prueba 1: Compresión básica
```bash
$ cat data/repite.txt
aaaaabbbbbccccc

$ ./bin/wzip data/repite.txt | hexdump -C
00000000  05 00 00 00 61 05 00 00  00 62 05 00 00 00 63 01  |....a....b....c.|
00000010  00 00 00 0a                                       |....|
00000014
```
**Explicación del hexdump:**
- `05 00 00 00` = 5 en little-endian (int de 4 bytes)
- `61` = 'a' en ASCII
- `05 00 00 00` = 5
- `62` = 'b'
- `05 00 00 00` = 5
- `63` = 'c'
- `01 00 00 00` = 1
- `0a` = '\n' (salto de línea)

#### ✅ Prueba 2: Descompresión
```bash
$ ./bin/wunzip data/repite.z
aaaaabbbbbccccc
```

#### ✅ Prueba 3: Ciclo completo (compresión → descompresión)
```bash
$ ./bin/wzip data/variado.txt > temp.z
$ ./bin/wunzip temp.z > recuperado.txt
$ diff data/variado.txt recuperado.txt
$ echo $?
0
```
_(Sin diferencias = integridad preservada)_

#### ✅ Prueba 4: Múltiples archivos
```bash
$ ./bin/wzip data/archivo1.txt data/archivo2.txt > combinado.z
$ ./bin/wunzip combinado.z
[Contenido de archivo1 + archivo2 descomprimido]
```

#### ✅ Prueba 5: Sin argumentos
```bash
$ ./bin/wzip
wzip: file1 [file2 ...]
$ echo $?
1

$ ./bin/wunzip
wunzip: file1 [file2 ...]
$ echo $?
1
```

---

### Validación de Integridad

```bash
# Comprobar que compresión/descompresión no pierda información
$ for file in data/*.txt; do
>   ./bin/wzip "$file" > temp.z
>   ./bin/wunzip temp.z > temp.txt
>   diff "$file" temp.txt && echo "✓ $file OK"
> done
✓ data/archivo1.txt OK
✓ data/archivo2.txt OK
✓ data/prueba_grep.txt OK
✓ data/repite.txt OK
✓ data/variado.txt OK
```

---

## 🐛 Problemas Encontrados y Soluciones

### Problema 1: Líneas largas en wgrep
**Descripción:** Inicialmente usé `fgets()` con buffer fijo de 1024 bytes, lo que limitaba el tamaño de las líneas.

**Solución:** Cambié a `getline()` que asigna memoria dinámicamente:
```c
char *linea = NULL;
size_t len = 0;
getline(&linea, &len, stdin);  // Maneja cualquier longitud
```

**Aprendizaje:** `getline()` es más flexible pero requiere `free()` para evitar memory leaks.

---

### Problema 2: Formato binario en wzip
**Descripción:** Primer intento escribiendo en texto plano no cumplía especificación.

**Solución:** Usar `fwrite()` para escritura binaria directa:
```c
fwrite(&count, sizeof(int), 1, stdout);  // 4 bytes
fwrite(&prev, sizeof(char), 1, stdout);  // 1 byte
```

**Aprendizaje:** Diferencia entre modo texto y binario es crítica para compresión.

---

### Problema 3: Último grupo no se comprimía
**Descripción:** El último grupo de caracteres repetidos quedaba sin escribir.

**Solución:** Agregar escritura después del loop principal:
```c
if (count > 0) {
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&prev, sizeof(char), 1, stdout);
}
```

---

### Problema 4: Lectura incorrecta en wunzip
**Descripción:** Intentar leer archivo binario en modo texto corrompía datos.

**Solución:** Abrir archivo en modo binario:
```c
FILE *archivo = fopen(argv[i], "rb");  // 'b' = binary
```

---

### Problema 5: Memory leak en wgrep
**Descripción:** `getline()` asigna memoria que debe liberarse manualmente.

**Solución:** Llamar `free(linea)` antes de cada retorno del programa.

---

## 💡 Decisiones de Diseño

### 1. Uso de `getline()` en wgrep
**Razón:** No hay límite práctico en el tamaño de líneas en archivos reales. `fgets()` con buffer fijo fallaría con líneas muy largas.

### 2. Formato binario estricto
**Razón:** 
- `int` de 4 bytes permite contar hasta 2^31 repeticiones
- Más eficiente que formato texto
- Compatible con especificación del laboratorio

### 3. Buffer de 1024 bytes en wcat
**Razón:** Balance entre uso de memoria y eficiencia. Leer carácter por carácter con `fgetc()` sería muy lento.

### 4. Validación de errores al inicio
**Razón:** Principio "fail fast" - detectar errores antes de procesar datos.

---

## 🎓 Conceptos Aprendidos

- **Manejo de archivos:** `fopen()`, `fclose()`, diferencia entre modo texto y binario
- **I/O en C:** `fgets()`, `getline()`, `fread()`, `fwrite()`
- **Gestión de memoria:** Asignación dinámica, `free()`, evitar memory leaks
- **Argumentos CLI:** `argc`, `argv`
- **Códigos de salida UNIX:** Convenciones estándar (0 = éxito, 1 = error)
- **Compresión RLE:** Algoritmo simple pero efectivo para datos repetitivos
- **Formato binario:** Serialización de datos, endianness

---

## 🤖 Manifiesto de Transparencia - Uso de IA

### ✅ Usos Apropiados
1. **Documentación:** Consultas sobre funciones de C (`getline`, `fread`, `fwrite`)
2. **Debugging:** Analizar errores de compilación y segmentation faults
3. **Formato binario:** Entender cómo escribir/leer datos binarios correctamente
4. **Este README:** Estructura y formato de documentación profesional

### 🧠 Trabajo Original
1. **Lógica de algoritmos:** Diseñé la implementación de RLE y búsqueda
2. **Resolución de problemas:** Identifiqué y corregí bugs (último grupo, memory leaks, etc.)
3. **Pruebas:** Creé todos los archivos de prueba y casos de validación
4. **Integración:** Adapté sugerencias de IA a requisitos específicos del laboratorio

### 📚 Aprendizaje
El uso de IA fue **complementario**, no sustituto del aprendizaje. Cada función implementada fue comprendida a fondo antes de integrarla.

---

## 🎥 Video de Sustentación

> **Enlace:** [Próximamente - Video de 10 minutos]

### Puntos a Cubrir:
1. **Demostración práctica** de cada programa (wcat, wgrep, wzip, wunzip)
2. **Explicación del algoritmo RLE** con ejemplo visual
3. **Uso de getline()** y manejo de memoria dinámica
4. **Formato binario** - mostrar hexdump y explicar estructura
5. **Pruebas de integridad** - ciclo completo compresión/descompresión
6. **Manejo de errores** y casos especiales

---

## 📦 Cómo Ejecutar

```bash
# 1. Clonar el repositorio
git clone https://github.com/JuanC101195/Laboratorio1.git
cd Laboratorio1

# 2. Compilar (si es necesario)
gcc -o bin/wcat src/wcat.c -Wall -Werror
gcc -o bin/wgrep src/wgrep.c -Wall -Werror
gcc -o bin/wzip src/wzip.c -Wall -Werror
gcc -o bin/wunzip src/wunzip.c -Wall -Werror

# 3. Probar
./bin/wcat data/archivo1.txt
./bin/wgrep mundo data/prueba_grep.txt
./bin/wzip data/repite.txt > output.z
./bin/wunzip output.z
```

---

## ✅ Checklist de Requisitos

- [x] wcat implementado y funcional
- [x] wgrep con soporte para stdin y archivos
- [x] wzip con compresión RLE binaria (int 4 bytes + char 1 byte)
- [x] wunzip con descompresión correcta
- [x] Manejo de errores en todos los programas
- [x] Códigos de salida correctos (0 = éxito, 1 = error)
- [x] Uso de `getline()` para líneas de cualquier longitud
- [x] Pruebas exhaustivas con resultados documentados
- [x] Validación de integridad (compresión/descompresión sin pérdida)
- [x] Código compilado sin warnings (`-Wall -Werror`)
- [x] README.md completo con ejemplos reales
- [x] Código subido a GitHub
- [ ] Video de sustentación grabado

---

## 📚 Referencias

- [The C Programming Language (K&R)](https://www.amazon.com/Programming-Language-2nd-Brian-Kernighan/dp/0131103628)
- [Linux man pages - getline](https://man7.org/linux/man-pages/man3/getline.3.html)
- [GNU C Library Documentation](https://www.gnu.org/software/libc/manual/)
- Material del curso de Sistemas Operativos

---

## 👤 Autor

**Juan Esteban Cardozo Rivera**  
GitHub: [@JuanC101195](https://github.com/JuanC101195)  
Correo: juan.cardozor@udea.edu.co

---

**Última actualización:** Marzo 2026
