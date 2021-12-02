CC = g++
TARGET = libImguiStatic.a

BUILD = build

CFLAGS = -D_FORTIFY_SOURCE=2 -I./ -I../GLFW/include

OBJECTS = imgui_draw.o imgui_tables.o imgui_widgets.o imgui.o imgui_demo.o backends/imgui_impl_glfw.o backends/imgui_impl_opengl3.o

all: lib

%.o: %.cpp
	@$(CC) -o $@ -c $< $(CFLAGS)

lib: $(OBJECTS)
	ar -rcs $(TARGET) $(OBJECTS)
	rm *.o
