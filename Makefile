CC = g++
TARGET_PREFIX = graphicsExe

DEPS = dependencies

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -I$(DEPS)/glm/ -I$(DEPS)/stb -I$(DEPS)/imgui -Isrc -D_FORTIFY_SOURCE=2 -std=c++20
LFLAGS = $(DEPS)/glad/glad.o $(DEPS)/GLFW/src/libglfw3.a $(DEPS)/glm/glm/libglm_static.a $(DEPS)/imgui/libImguiStatic.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl -no-pie

SRC  = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)

ifndef config
	config=release
endif

ifeq ($(config), debug)
	CFLAGS += -DDEBUG -g -pg
	LFLAGS += -pg -g

	TARGET = $(TARGET_PREFIX)_debug
	OBJ = obj/debug
else
	TARGET = $(TARGET_PREFIX)
	OBJ = obj/release
endif

OBJECTS  = $(SRC:src/%.cpp=$(OBJ)/%.o)
OBJDIRS = $(dir $(OBJECTS))

all: $(TARGET)

fresh: clean setup $(TARGET)

setup: dirs libs

$(TARGET): $(OBJECTS)
	@echo [EXE] $(TARGET)
	@$(CC) -o $@  $^ $(LFLAGS)

$(OBJ)/%.o: src/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

libs:
	@echo
	@echo ================= Building libs ===============
	@echo [LIB] GLFW
	@cd $(DEPS)/GLFW && cmake . -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF && make -s --no-print-directory
	@echo [LIB] Glad
	@cd $(DEPS)/glad && gcc -o glad.o -Iinclude -c src/glad.c
	@echo [LIB] glm
	@cd $(DEPS)/glm && cmake . -DBUILD_TESTING=OFF && make -s --no-print-directory
	@echo [LIB] ImGui
	@cp -p dependencies/makefiles/imgui.make dependencies/imgui/Makefile
	@cd $(DEPS)/imgui && make -s --no-print-directory
	@echo ===================== Done! ===================
	@echo

dirs:
	@mkdir -p ./$(OBJ)
	@mkdir -p ./$(OBJDIRS)

clean:
	@rm -rf ./obj $(TARGET)

run:
	@ ./$(TARGET)
