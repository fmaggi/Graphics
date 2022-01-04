CC = clang++
LL = llvm-ar

DEPS = dependencies

CFLAGS = -I$(DEPS)/glad/include -I$(DEPS)/GLFW/include -I$(DEPS)/glm/ -I$(DEPS)/stb -I$(DEPS)/imgui -Isrc -Wall -Werror -D_FORTIFY_SOURCE=2 -std=c++20 -O2
LFLAGS = src/$(ENGINE) $(DEPS)/glad/glad.o $(DEPS)/GLFW/src/libglfw3.a $(DEPS)/imgui/libImguiStatic.a -lm -lGL -lX11 -lpthread -lXrandr -lXi -ldl -no-pie

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

all: engine editor

fresh: clean setup libs engine editor

setup: libs

clean: editor_clean engine_clean
	@rm -rf $(OBJ)

include build/libs.mk
include build/engine.mk
include build/editor.mk
