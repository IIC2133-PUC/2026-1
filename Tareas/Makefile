###############################################################################
#                   Makefile Múltiple Recursivo (Unix-compatible)             #
#                                                                             #
# Versión original por Vicente Errázuriz                                      #
# Para el curso de Estructuras de Datos y Algoritmos, 2017 - 1, PUC           #
# Makefile diseñada para el trabajo de varios programas con código común      #
#                                                                             #
# OJO: Modificada por Benja Vicente (2023 - 2), para:                         #
#   -  permitir bien SRC y BIN                                                #
#                                                                             #
# OJO 2: Modificado por Luis González (ljgonzalez1) (2026 - 1), para hacerlo  #
#        más user friendly. Se:                                               #
#   - Implementa búsqueda recursiva de archivos a compilar                    #
#   - Catch por si alguna carpeta en COMMON no existe                         #
#   - Agregar % de progreso                                                   #
#   - Simplificar el las flags para optimizar y debuggear                     #
#   - Agregar colores ANSI :D                                                 #
#   - Agregar regla de 'recreate' := make clean & make all                    #
#                                                                             #
###############################################################################


###############################################################################
#                               GUÍA DE USO                                   #
###############################################################################
#                                                                             #
# VARIABLES CONFIGURABLES:                                                    #
#   COLORS, DEBUG_MODE, PROGRAMS, COMMON, SRC, BIN, OBJ, OPT, LIB, etc.       #
#                                                                             #
# COMANDOS DISPONIBLES:                                                       #
#   make, make clean, make recreate, make recreate binX, make binX...         #
#                                                                             #
# NOTAS: Tus archivos y carpetas no pueden tener espacios en el nombre        #
###############################################################################


###############################################################################
########========--------         Variables Base        --------========########
###############################################################################

# Descomentar una de las dos líneas:
DEBUG_MODE = true   # Mostrará warnings y no optimizará. Sirve para debug
# DEBUG_MODE = false  # No mostrará warnings y optimizará al máximo

# Directorios que serán compilados a un programa
PROGRAMS=puffle \
	cardjitsu

# Directorios con elementos de uso común
COMMON=

# La carpeta donde va todo el código
SRC=src

# La carpeta donde quedan los ejecutables
BIN=.

# El compilador a usar: Gnu C Compiler, Standard 2011 with GNU extensions
CC=gcc -std=gnu11

# La carpeta donde van todos los archivos de objeto
OBJ=obj


###############################################################################
# OPTIMIZACIÓN POR COMPILADOR, según el flag DEBUG_MODE                       #
###############################################################################

ifeq ($(strip $(DEBUG_MODE)),true)
    # Modo Debug (con warnings, etc.)
    # O0: Sin optimización.
    # g3: Con info para debug.
    # fno-omit-frame-pointer: Mejores backtraces / stack traces.
    # fno-inline: Sin uso de funciones inline. Usa calls en vez.
    # W*: Warnings varios
    OPT = -O0 -g3 \
    -Wall -Wextra \
    -Wunused -Wshadow -Wundef \
    -Wconversion -Wsign-conversion \
    -Wformat \
    -Wstrict-prototypes \
    -fno-omit-frame-pointer -fno-inline

else
    # Modo Optimizado
    OPT = -O3 -s -flto
    # Si estamos en modo optimizado, se suprimen los mensajes de entrada/salida de
    # directorios en las llamadas recursivas a make.
    MAKEFLAGS += --no-print-directory
endif

###############################################################################
# PARÁMETROS                                                                  #
###############################################################################

CFLAGS = $(OPT)

###############################################################################
# LIBRERÍAS                                                                   #
###############################################################################

# Matemáticas (C Math library)
MTH=-lm
# Portable Network Graphics (PNG)
PNG=#-lpng

LIB=$(MTH) $(PNG)

###############################################################################
# MÓDULOS Y PROGRAMAS                                                         #
###############################################################################

# Todos los directorios que contienen archivos de código
SRCDIR=$(COMMON) $(PROGRAMS)

###############################################################################
# DEPENDENCIAS Y DIRECTORIOS                                                  #
###############################################################################

# Todos los archivos .h de las carpetas comunes
DEPS := $(foreach i, $(COMMON), $(shell find $(SRC)/$(i) -name '*.h'))

# Todos los archivos .h
HDRFILES := $(shell find $(SRC) -name '*.h')

# Todos los archivos .c
SRCFILES := $(shell find $(SRC) -name '*.c')

# Archivos de objeto .o, un estado intermedio de compilación
OBJFILES := $(foreach i, $(SRCFILES), $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(i)))

# Los directorios para los archivos de objeto .o
OBJDIR := $(patsubst $(SRC)/%, $(OBJ)/%, $(shell find $(SRC) -type d))

# Archivos ejecutables
BIN_PROGRAMS := $(addprefix $(BIN)/,$(PROGRAMS))

###############################################################################
# REGLAS                                                                      #
###############################################################################

# Las reglas son como funciones.
# Cuando llamas 'make X' en la consola, se ejecuta la regla X.
# Las reglas tienen prerrequisitos: esto es lo que aparece a su derecha
# Una vez se cumplan los prerrequisitos, se ejecuta el contenido de la regla
# Si llamas 'make' a secas se ejecutará la primera regla: 'all'

# Esta regla imprime que todo está listo
# Pero solo una vez que se hayan llamado las reglas $(OBJDIR) y $(PROGRAMS)

all: $(OBJDIR) $(BIN) $(BIN_PROGRAMS)
	@echo "done compiling"

# Esta regla elimina todo registro de compilación que se haya hecho
clean:
	@rm -rfv $(BIN_PROGRAMS) && rm -rfv $(OBJ)
	@if [ -d "$(bin_dir)" ] && [ "$(bin_dir)" != "." ]; then rm -r "$(bin_dir)"; fi
	@echo "done cleaning"

# Esta regla crea los directorios donde se guardan los archivos de objeto .o
$(OBJDIR):
	@mkdir -p $@

# Crea la carpeta donde se guardan los ejecutables
$(BIN):
	@mkdir -p $@

# Esta regla mágica indica que las siguientes reglas necesitan dos pasadas
.SECONDEXPANSION:

# Dependencias locales para un archivo .o
LOCAL_DEPS = $(filter $(patsubst $(OBJ)/%, $(SRC)/%, $(dir $(1)))%, $(HDRFILES))

# Esta regla compila cada archivo de objeto .o
# Pero sólo si alguno de los siguientes fue modificado desde la última vez
## el .c respectivo del .o
## algún .h bajo la carpeta respectiva en src
## algún .h de los directorios comunes
## esta mismísima Makefile
obj/%.o: $(SRC)/%.c $$(call LOCAL_DEPS,$$@) $(DEPS) Makefile
	@$(CC) $(CFLAGS) $< -c -o $@ $(LIB) && echo "compiled '$@'"

# Esta regla conecta y compila cada programa a partir de los .o
# Pero solo una vez que se haya llamado la regla anterior con lo siguiente
## todos los .o de la carpeta respectiva del programa
## todos los .o de los directorios comunes
$(BIN_PROGRAMS): $$(filter $(OBJ)/$$(notdir $$@)/% $(foreach i, $(COMMON), $(OBJ)/$(i)/%), $(OBJFILES))
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB) && echo "compiled '$@'"


###############################################################################
#                   Cualquier duda no temas en preguntar!                     #
###############################################################################
# Disclaimer:                                                                 #
#                                                                             #
# Deberías modificar solamente el nivel de Optimización (OPT, linea 20).      #
# Modificar la Makefile si no sabes lo que está pasando o como la usamos los  #
# ayudantes puede resultar en un perjuicio en la evaluación de tu código.     #
###############################################################################