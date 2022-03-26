ENGINE = src/libEngine.a

ENGINE_SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
SRC += $(ENGINE_SRC)

ENGINE_OBJECTS  = $(ENGINE_SRC:src/%.cpp=$(OBJ)/%.o)

OBJDIRS = $(dir $(ENGINE_OBJECTS))

JSON_ENGINE  = $(ENGINE_SRC:src/%.cpp=src/%.j)

engine: $(ENGINE)

$(ENGINE): engine_dirs $(ENGINE_OBJECTS)
	@echo [LIB] $(ENGINE)
	@$(LL) rcs $(ENGINE) $(ENGINE_OBJECTS)

$(OBJ)/%.o: src/%.cpp
	@echo [CC] $<
	@$(CC) -o $@ -c $< $(CFLAGS)

engine_dirs:
	@mkdir -p ./$(OBJDIRS)

engine_clean:
	@rm -rf ./$(OBJDIRS) $(ENGINE)

engine_setup: $(JSON_ENGINE)
	@sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(JSON_ENGINE:%.j=%.j.json) > build/compile_commands_engine.json
	@rm $(JSON_ENGINE:%.j=%.j.json)

src/%.j: src/%.cpp
	@$(CC) -MJ $@.json $(CFLAGS) -o $@ -c $<
	@rm $@
