TARGET = test


SRC  = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJECTS  = $(SRC:.c=.o)
GENERATED = $(addprefix $(OBJ)/, $(notdir $(OBJECTS)))
OBJ = obj

DEPS = dependencies
LIBSOBJ = bin
BIN = bin

CC = gcc

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -Isrc
LFLAGS = $(LIBSOBJ)/glad.o $(LIBSOBJ)/libglfw3.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl

all: $(TARGET)

first: dirs libs $(TARGET)

$(TARGET): $(OBJECTS)
	@echo [EXE] $(TARGET) 
	@$(CC) -o $@  $(addprefix $(OBJ)/, $(notdir $^)) $(LFLAGS)

t:
	@echo $(GENERATED)

%.o: %.c
	@echo [CC] $<
	@$(CC) -o $(OBJ)/$(notdir $@) -c $< $(CFLAGS)

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