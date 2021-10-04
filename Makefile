CC = gcc

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -Isrc
LFLAGS = $(LIBSOBJ)/glad.o $(LIBSOBJ)/libglfw3.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)

ifndef config
	config=release
endif

ifeq ($(config), debug)
	CFLAGS += -DDEBUG

	TARGET = test_debug
	OBJ = obj/debug
else
	TARGET = test
	OBJ = obj
endif

OBJECTS  = $(SRC:src/%.c=$(OBJ)/%.o)
OBJDIRS = $(dir $(OBJECTS))

DEPS = dependencies
LIBSOBJ = bin
BIN = bin

all: $(TARGET)

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
	@cd $(DEPS)/GLFW && cmake . && make -s --no-print-directory && cp ./src/libglfw3.a ../../$(LIBSOBJ)
	@echo [LIB] Glad
	@cd $(DEPS)/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c && mv src/glad.o ../../$(LIBSOBJ)
	@echo ===================== Done! ===================
	@echo

dirs:
	@mkdir -p ./$(OBJ)
	@mkdir -p ./$(LIBSOBJ)
	@mkdir -p ./$(BIN)
	@mkdir -p ./$(OBJDIRS)

clean:
	@rm -rf $(OBJ) $(BIN)