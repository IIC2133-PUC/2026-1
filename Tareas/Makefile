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

COLORS = true


###############################################################################
#                          CONFIGURACIÓN INTERNA                              #
#        (No modificar de aquí en adelante si no sabes lo que haces)          #
###############################################################################

# Eliminar archivos en caso de error de compilación
.DELETE_ON_ERROR:

# Hacer que Make aborte en el primer error (-e) y ejecute en modo comando (-c)
.SHELLFLAGS := -ec

###############################################################################
# Colores                                                                     #
###############################################################################

# ANSI/VT100 Códigos de Escapes
# Colores ANSI para salida en consola (válido en Unix-like)
ifeq ($(strip $(COLORS)),true)
  YELLOW         := \033[33m
  BOLD           := \033[1m
  NO_BOLD        := \033[22m
  CYAN           := \033[36m
  GREEN          := \033[32m
  RED_BOLD_SHELL := $(shell printf '\033[31m')
  RESET          := \033[0m
  RESET_SHELL    := $(shell printf '\033[0m')
else
  YELLOW    :=
  BOLD      :=
  NO_BOLD   :=
  CYAN      :=
  GREEN     :=
  RED       :=
  RESET     :=
endif

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
# Definición de comandos                                                      #
###############################################################################

RMDIR_CMD := rm -rf$(if $(filter true,$(DEBUG_MODE)),v)  # Verbose solo en Debug Mode
RM_CMD    := rm -f
MKDIR_CMD := mkdir -p
NULL_DEVICE := /dev/null

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

all: $(OBJDIR) $(BIN) $(BIN_PROGRAMS)
	@echo "$(CYAN)$(BOLD)Done compiling$(RESET)"

clean:
	@echo "$(BOLD)$(GREEN)Cleaning...$(RESET)"
	@echo "$(BOLD)$(CYAN)Entering folder: $(CURDIR) ...$(RESET)"
	@echo "Deleting folder $(OBJ)..."
	@$(RMDIR_CMD) $(OBJ) 2>$(NULL_DEVICE) || true
	@$(foreach bin, $(BIN_PROGRAMS), \
		$(RM_CMD) $(bin) 2>$(NULL_DEVICE) || true; )
	@echo "$(BOLD)$(CYAN)Leaving folder: $(CURDIR) ...$(RESET)"
	@echo "$(BOLD)$(GREEN)Done cleaning$(RESET)"

# Esta regla crea los directorios donde se guardan los archivos de objeto .o
$(OBJDIR):
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(BOLD)$(CYAN)Creating objects folder: $(NO_BOLD)$(CYAN)$@$(RESET)")
	@$(MKDIR_CMD) $@

# Crea la carpeta donde se guardan los ejecutables
$(BIN):
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(BOLD)$(CYAN)Creating bin folder: $(NO_BOLD)$(CYAN)$@$(RESET)")
	@$(MKDIR_CMD) $@

.SECONDEXPANSION:

LOCAL_DEPS = $(filter $(patsubst $(OBJ)/%, $(SRC)/%, $(dir $(1)))%, $(HDRFILES))

obj/%.o: $(SRC)/%.c $$(call LOCAL_DEPS,$$@) $(DEPS) Makefile
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(GREEN)Compiling $(notdir $<)...$(RESET)")
	@$(CC) $(CFLAGS) $< -c -o $@ $(LIB) && echo "$(GREEN)$(BOLD)compiled '$(notdir $<)'$(RESET)"

$(BIN_PROGRAMS): $$(filter $(OBJ)/$$(notdir $$@)/% $(foreach i, $(COMMON), $(OBJ)/$(i)/%), $(OBJFILES))
	@echo "$(GREEN)Linking $(notdir $@)...$(RESET)"
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB)
	@echo "$(GREEN)$(BOLD)  compiled '$(notdir $@)'$(RESET)"


###############################################################################
#                   Cualquier duda no temas en preguntar!                     #
###############################################################################
# Disclaimer:                                                                 #
#                                                                             #
# Deberías modificar solamente el nivel de Optimización (OPT, linea 20).      #
# Modificar la Makefile si no sabes lo que está pasando o como la usamos los  #
# ayudantes puede resultar en un perjuicio en la evaluación de tu código.     #
###############################################################################