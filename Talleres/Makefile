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

# Directorios que serán compilados a un programa (busca de forma recursiva en la carpeta)
PROGRAMS = puffle \
	cardjitsu qwerty

# Directorios con elementos de uso común para todos los binarios (busca de forma recursiva en la carpetas incluidas distintas del binario)
COMMON = \
    include \
    common \
    lib

# La carpeta donde va todo el código
SRC = src

# TODO: Hacer que acepte el caso borde "BIN = " (sin directorio)
# La carpeta donde quedan los ejecutables
BIN = ./

# El compilador a usar: Gnu C Compiler, Standard 2011 con extensiones GNU
CC = gcc -std=gnu11

# La carpeta donde van los .o
OBJ = ./obj

COLORS = true

###############################################################################
########========--------           Librerías           --------========########
###############################################################################

# Lista de librerías a incluir (descomenta las que necesites)
LIB  = \
    -lm
    # -lpng \
    # -lpthread \
    # -ljpg





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
# OPTIMIZACIÓN POR COMPILADOR, según el flag OPTIMIZED_MODE                   #
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
	#
	OPT = -O3 -s -flto
	# Si estamos en modo optimizado, se suprimen los mensajes de entrada/salida de
	# directorios en las llamadas recursivas a make.
	MAKEFLAGS += --no-print-directory
endif

###############################################################################
# Definición de comandos                                                      #
###############################################################################

RMDIR_CMD := rm -rf$(if $(filter true,$(DEBUG_MODE)),v)  # Verbose solo en Debug Mode
RM_CMD := rm -f
MKDIR_CMD := mkdir -p
NULL_DEVICE := /dev/null


###############################################################################
#                        Funciones de Recursividad                            #
###############################################################################

rwildcard = $(foreach d,$(wildcard $1*), \
               $(call rwildcard,$d/,$2) \
               $(filter $(subst *,%,$(2)),$(d)) \
             )

# Equivalente a la búsqueda, pero sin 'find'
find_files = $(if $(wildcard $(SRC)/$(1)), \
                 $(call rwildcard,$(SRC)/$(1)/,$(2)), \
                 $(error El directorio $(SRC)/$(1) no existe pero está \
                  en PROGRAMS o COMMON) )


###############################################################################
#             Definición Recursiva de Archivos Unix-Compatible (sin find)     #
###############################################################################

# Si COMMON no está vacío, busca archivos .c en cada directorio listado en COMMON.
COMMON_SRC := $(if $(strip $(COMMON)), \
               $(foreach d,$(COMMON), \
                 $(if $(wildcard $(SRC)/$(d)), \
                   $(call find_files,$(d),*.c), \
                   $(if $(filter true,$(strip $(DEBUG_MODE))), \
                     $(info $(RED_BOLD_SHELL)Warning: el directorio $(SRC)/$(d) no existe y será omitido.$(RESET_SHELL))) \
                 ) \
               ),)

# Para cada programa en PROGRAMS:
# - Se define <prog>_SRC: archivos fuente (.c) + COMMON_SRC
# - Se define <prog>_OBJS: convierte cada fuente (<SRC>/%.c) en su objeto correspondiente (<OBJ>/%.o).
$(foreach prog,$(PROGRAMS), \
  $(eval $(prog)_SRC := $(call find_files,$(prog),*.c) $(COMMON_SRC)) \
  $(eval $(prog)_OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$($(prog)_SRC))) )

# Binarios a compilar (filtrados en casos borde de BIN de root, y //)
BIN_DIR := $(if $(filter %/,$(BIN)),$(BIN),$(BIN)/)
BIN_PROGRAMS := $(if $(strip $(BIN)), \
    $(addprefix $(BIN_DIR),$(PROGRAMS)), \
    $(addprefix ./,$(PROGRAMS)))

# Cálculo de archivos para mostrar porcentaje de avance.
ALL_OBJ_FILES := $(foreach prog,$(PROGRAMS),$($(prog)_OBJS))
TOTAL_OBJS   := $(words $(ALL_OBJ_FILES))
ifeq ($(TOTAL_OBJS),0)  # Evita zero division error
  TOTAL_OBJS := 1
endif

# Busca recursivamente todos los archivos de encabezado (.h) dentro de SRC.
HDR_FILES := $(call rwildcard,$(SRC)/,*.h)
# Obtiene la lista de directorios donde se encuentran los archivos objeto, ordenada y sin duplicados.
OBJ_DIR   := $(sort $(dir $(ALL_OBJ_FILES)))


###############################################################################
# PARÁMETROS ADICIONALES (Legacy code)                                        #
###############################################################################
CFLAGS = $(OPT)

###############################################################################
# Evitamos que se borren los .o después de linkear en debug mode              #
###############################################################################
ifeq ($(strip $(DEBUG_MODE)),true)
.SECONDARY: $(ALL_OBJ_FILES)
endif

###############################################################################
# REGLAS                                                                      #
###############################################################################
THIS_MAKEFILE := $(firstword $(MAKEFILE_LIST))

.PHONY: all clean recreate _reset_counter _main_comp _cleanup _calc_total

all:
	@echo "$(YELLOW)$(BOLD)Compilation details:$(RESET)"
	@echo "        $(YELLOW)Compiler $(BOLD)CC=$(CC)$(NO_BOLD) will be used with flags $(BOLD)CFLAGS=$(OPT)$(RESET)"
	@echo ""
	@$(MAKE) -f $(THIS_MAKEFILE) _reset_counter
	@$(MAKE) -f $(THIS_MAKEFILE) _calc_total
	@-$(MAKE) -f $(THIS_MAKEFILE) _main_comp
	@-$(MAKE) -f $(THIS_MAKEFILE) _cleanup
	@echo "$(CYAN)$(BOLD)Done compiling$(RESET)"

_cleanup:
	@$(RM_CMD) .compile_count 2>$(NULL_DEVICE) || true

_main_comp: $(OBJ_DIR) $(BIN) $(BIN_PROGRAMS)
	@true  # solo agrupar dependencias

recreate:
	@echo "==> Recreating all. Cleaning first..."
	@$(MAKE) -f $(THIS_MAKEFILE) clean
	@echo "==> Compiling from scratch..."
	@$(MAKE) -f $(THIS_MAKEFILE) all

###############################################################################
#                          Limpieza detallada                                 #
###############################################################################
clean:
	@echo "$(BOLD)$(GREEN)Cleaning...$(RESET)"
	@echo "$(BOLD)$(CYAN)Entering folder: $(CURDIR) ...$(RESET)"
	@echo "Deleting folder $(OBJ)..."
	@$(RMDIR_CMD) $(OBJ) 2>$(NULL_DEVICE) || true
	@$(foreach bin, $(BIN_PROGRAMS), \
		$(if $(filter true,$(DEBUG_MODE)),echo "Deleting binary $(patsubst ./%,%,$(bin))";) \
		$(RM_CMD) $(bin) 2>$(NULL_DEVICE) || true; )
	@$(RM_CMD) .compile_count 2>$(NULL_DEVICE) || true
	@echo "$(BOLD)$(CYAN)Leaving folder: $(CURDIR) ...$(RESET)"
	@echo "$(BOLD)$(GREEN)Done cleaning$(RESET)"


_reset_counter:
	@$(RM_CMD) .compile_count 2>$(NULL_DEVICE) || true

###############################################################################
#          Regla para calcular la cantidad real de archivos a compilar        #
###############################################################################
_calc_total:
	@total=0; \
	for obj in $(ALL_OBJ_FILES); do \
	  src=`echo $$obj | sed 's|$(OBJ)|$(SRC)|; s/\.o$$/.c/'`; \
	  if [ ! -f $$obj ] || [ $$src -nt $$obj ]; then \
	     total=$$((total+1)); \
	  fi; \
	done; \
	if [ $$total -eq 0 ]; then total=1; fi; \
	echo $$total > .total_to_compile;

###############################################################################
#  Crear directorios                                                          #
###############################################################################
$(OBJ_DIR):
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(BOLD)$(CYAN)Creating objects folder: $(NO_BOLD)$(CYAN)$@$(RESET)")
	@$(MKDIR_CMD) $@

$(BIN):
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(BOLD)$(CYAN)Creating bin folder: $(NO_BOLD)$(CYAN)$@$(RESET)")
	@$(MKDIR_CMD) $@

###############################################################################
# Regla mágica para manejar dependencias                                      #
###############################################################################
.SECONDEXPANSION:
LOCAL_DEPS = $(filter $(patsubst $(OBJ)/%, $(SRC)/%, $(dir $(1)))%, $(HDR_FILES))

###############################################################################
#  Regla de compilación con porcentaje de avance                              #
###############################################################################
$(OBJ)/%.o: $(SRC)/%.c $$(call LOCAL_DEPS,$$@) $(HDR_FILES) Makefile
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(GREEN)Compiling $(notdir $<)...$(RESET)")
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(CC) $(CFLAGS) $< -c -o $@ $(LIB)")
	@$(MKDIR_CMD) $(dir $@)
	@$(CC) $(CFLAGS) $< -c -o $@ $(LIB)
	@ CURRENT=$$(cat .compile_count 2>$(NULL_DEVICE) || echo 0); \
	   CURRENT=$$((CURRENT+1)); \
	   echo $$CURRENT > .compile_count; \
	   TOTAL=`cat .total_to_compile`; \
	   PCT=$$((100*CURRENT/TOTAL)); \
	   echo "$(GREEN)$(BOLD)[ $${PCT}% ] compiled '$(notdir $<)'$(RESET)"

###############################################################################
# Linkeo final                                                                #
###############################################################################
$(BIN)/%: $$($$*_OBJS)
	@echo "$(GREEN)Linking $(notdir $@)...$(RESET)"
	@$(if $(filter true,$(DEBUG_MODE)),echo "$(CC) $(CFLAGS) $^ -o $@ $(LIB)")
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB)
	@echo "$(GREEN)$(BOLD)  compiled '$(notdir $@)'$(RESET)"




###############################################################################
# XXXXXXX Known bugs / issues + pending nice to have XXXXXXX                  #
###############################################################################
# 1. El porcentaje no comienza desde cero si la compilación ya tiene cacheado al menos una fracción.
# 2. `make clean` falla si el binario no está en la lista.
# 3. Hacer que acepte el caso borde "BIN = " (sin directorio).
# 4. Todo lo que está en `COMMON` debe encontrarse dentro de la carpeta `/src`. Sería ideal que no fuese requisito
###############################################################################




###############################################################################
#                  ¡Cualquier duda no temas en preguntar!                     #
###############################################################################
# Disclaimer:                                                                 #
#   Deberías modificar solamente el nivel de Optimización (`DEBUG_MODE`) y    #
#   las carpetas de archivos a compilar (`PROGRAMS` y `COMMON`). Modificar la #
#   Makefile si no sabes lo que está pasando o cómo la usamos los ayudantes   #
#   puede resultar en un perjuicio en la evaluación de tu código.             #
###############################################################################
