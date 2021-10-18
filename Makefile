CC = gcc
TARGET_PREFIX = graphicsExe

DEPS = dependencies

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -I$(DEPS)/cglm/include -I$(DEPS)/stb -Isrc -Wall -Werror -std=c99 -D_FORTIFY_SOURCE=2
LFLAGS = $(DEPS)/glad/glad.o $(DEPS)/GLFW/src/libglfw3.a $(DEPS)/cglm/libcglm.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl -no-pie

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)

ifndef config
	config=release
endif

ifeq ($(config), debug)
	CFLAGS += -DDEBUG -g

	TARGET = $(TARGET_PREFIX)_debug
	OBJ = obj/debug
else
	TARGET = $(TARGET_PREFIX)
	OBJ = obj/release
endif

OBJECTS  = $(SRC:src/%.c=$(OBJ)/%.o)
OBJDIRS = $(dir $(OBJECTS))

all: $(TARGET)

fresh: clean setup $(TARGET)

setup: dirs libs

$(TARGET): $(OBJECTS)
	@echo [EXE] $(TARGET)
	@$(CC) -o $@  $^ $(LFLAGS)

t:
	@echo $(GENERATED)

$(OBJ)/%.o: src/%.c
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

libs:
	@echo
	@echo ================= Building libs ===============
	@echo [LIB] GLFW
	@cd $(DEPS)/GLFW && cmake . -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF && make -s --no-print-directory
	@echo [LIB] Glad
	@cd $(DEPS)/glad && $(CC) -o glad.o -Iinclude -c src/glad.c
	@echo [LIB] cglm
	@cd $(DEPS)/cglm && cmake . -DCGLM_STATIC=ON && make -s --no-print-directory
	@echo ===================== Done! ===================
	@echo

dirs:
	@mkdir -p ./$(OBJ)
	@mkdir -p ./$(OBJDIRS)

clean:
	@rm -rf ./obj $(TARGET)

run:
	@ ./$(TARGET)