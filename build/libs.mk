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
