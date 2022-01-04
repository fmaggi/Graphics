ENGINE = libEngine.a

ENGINE_SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
ENGINE_OBJECTS  = $(ENGINE_SRC:src/%.cpp=$(OBJ)/%.o)

OBJDIRS = $(dir $(ENGINE_OBJECTS))

engine: engine_dirs $(ENGINE_OBJECTS)
	@echo [LIB] $(ENGINE)
	@$(LL) rcs src/$(ENGINE) $(ENGINE_OBJECTS)

$(OBJ)/%.o: src/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

engine_dirs:
	@mkdir -p ./$(OBJDIRS)

engine_clean:
	@rm -rf ./$(OBJDIRS) src/$(ENGINE)
