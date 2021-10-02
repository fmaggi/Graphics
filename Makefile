TARGET = test

SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJECTS  = $(SRC:.c=.o)
OBJ = obj

DEPS = dependencies
LIBSOBJ = obj/libs
BIN = bin

CC = gcc

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include
LFLAGS = $(LIBSOBJ)/glad.o $(LIBSOBJ)/libglfw3.a -lm -ldl -lpthread

all: dirs libs project

$(TARGET): $(OBJECTS)
	@echo [EXE] $(TARGET) 
	@$(CC) -o $(BIN)/$@ $(OBJ)/$(notdir $^) $(LFLAGS)

%.o: %.c
	@echo [CC] $<
	@$(CC) -o $(OBJ)/$(notdir $@) -c $< $(CFLAGS)

project:
	@echo
	@echo ============== Building project ===============
	@make --no-print-directory $(TARGET)
	@echo ===================== Done! ===================
	@echo

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

clean:
	@rm -rf $(OBJ) $(BIN)