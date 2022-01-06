EDITOR = Editor

EDITOR_SRC = $(wildcard editor/*.cpp) $(wildcard editor/**/*.cpp) $(wildcard editor/**/**/*.cpp) $(wildcard editor/**/**/**/*.cpp)

EDITOR_OBJECTS  = $(EDITOR_SRC:editor/%.cpp=$(OBJ)/%.o)
E_OBJDIRS = $(dir $(EDITOR_OBJECTS))

editor: $(EDITOR)

$(OBJ)/%.o: editor/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

$(EDITOR): editor_libs $(EDITOR_OBJECTS)
	@echo [EXE] $(EDITOR)
	@$(CC) -o $(EDITOR) $(EDITOR_OBJECTS) $(LFLAGS)

editor_libs:
	@mkdir -p ./$(E_OBJDIRS)

editor_clean:
	@rm -rf ./$(E_OBJDIRS) $(EDITOR)
