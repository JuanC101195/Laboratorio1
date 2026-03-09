# Makefile para Laboratorio 1 - Comandos UNIX en C
# Autor: Juan Esteban Cardozo Rivera

CC = gcc
CFLAGS = -Wall -Werror
BINDIR = bin
SRCDIR = src

# Ejecutables
TARGETS = $(BINDIR)/wcat $(BINDIR)/wgrep $(BINDIR)/wzip $(BINDIR)/wunzip

# Target por defecto: compilar todos los programas
all: $(TARGETS)

# Compilar wcat
$(BINDIR)/wcat: $(SRCDIR)/wcat.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ wcat compilado"

# Compilar wgrep
$(BINDIR)/wgrep: $(SRCDIR)/wgrep.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ wgrep compilado"

# Compilar wzip
$(BINDIR)/wzip: $(SRCDIR)/wzip.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ wzip compilado"

# Compilar wunzip
$(BINDIR)/wunzip: $(SRCDIR)/wunzip.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ wunzip compilado"

# Limpiar ejecutables y archivos temporales
clean:
	rm -f $(TARGETS)
	rm -f temp.z temp.txt
	@echo "✓ Archivos temporales eliminados"

# Recompilar todo desde cero
rebuild: clean all

# Ejecutar pruebas básicas
test: all
	@echo "=== Probando wcat ==="
	./$(BINDIR)/wcat data/archivo1.txt
	@echo "\n=== Probando wgrep ==="
	./$(BINDIR)/wgrep mundo data/prueba_grep.txt
	@echo "\n=== Probando wzip/wunzip ==="
	./$(BINDIR)/wzip data/repite.txt | ./$(BINDIR)/wunzip /dev/stdin
	@echo "\n✓ Todas las pruebas completadas"

# Ayuda
help:
	@echo "Targets disponibles:"
	@echo "  make          - Compilar todos los programas"
	@echo "  make all      - Compilar todos los programas"
	@echo "  make clean    - Eliminar ejecutables y temporales"
	@echo "  make rebuild  - Limpiar y recompilar todo"
	@echo "  make test     - Ejecutar pruebas básicas"
	@echo "  make help     - Mostrar esta ayuda"

.PHONY: all clean rebuild test help
