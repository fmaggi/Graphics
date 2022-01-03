CC = clang++
LL = llvm-ar

ENGINE = libEngine.a
EDITOR = Editor

DEPS = dependencies

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -I$(DEPS)/glm/ -I$(DEPS)/stb -I$(DEPS)/imgui -Isrc -D_FORTIFY_SOURCE=2 -std=c++20 -O2
LFLAGS = src/$(ENGINE) $(DEPS)/glad/glad.o $(DEPS)/GLFW/src/libglfw3.a $(DEPS)/imgui/libImguiStatic.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl -no-pie

ENGINE_SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
EDITOR_SRC = $(wildcard editor/*.cpp) $(wildcard editor/**/*.cpp) $(wildcard editor/**/**/*.cpp) $(wildcard editor/**/**/**/*.cpp)

ifndef config
	config=release
endif

ifeq ($(config), debug)
	CFLAGS += -DDEBUG -g -pg
	LFLAGS += -pg -g

	OBJ = obj/debug
else
	OBJ = obj/release
endif

ENGINE_OBJECTS  = $(ENGINE_SRC:src/%.cpp=$(OBJ)/%.o)
EDITOR_OBJECTS  = $(EDITOR_SRC:editor/%.cpp=$(OBJ)/%.o)
OBJDIRS = $(dir $(ENGINE_OBJECTS)) $(dir $(EDITOR_OBJECTS))

all: dirs engine editor

fresh: clean dirs engine editor

setup: dirs libs

engine: $(ENGINE_OBJECTS)
	@echo [LIB] $(ENGINE)
	@$(LL) rcs src/$(ENGINE) $(ENGINE_OBJECTS)

$(OBJ)/%.o: src/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ)/%.o: editor/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

editor: $(EDITOR_OBJECTS)
	@echo [EXE] $(EDITOR)
	@$(CC) -o $(EDITOR) $(EDITOR_OBJECTS) $(LFLAGS)

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
	@rm -rf ./obj src/$(ENGINE) $(EDITOR)

run:
	@ ./$(EDITOR)
