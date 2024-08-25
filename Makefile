# Makefile

C_COMPILER = gcc
CPP_COMPILER = g++
CFLAGS = -Wall
LINK = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SHADER_DIR = src/shader
BIN_DIR = build
OUTPUT_DIR = output

all: shader main

shader:
	@echo Parsing Shaders...
	@python shaderparser.py

main: third-party/glad/src/glad.o \
	  \
	  src/math/vector3.o \
	  src/math/spectrum.o \
	  \
	  src/base/camera.o \
	  src/base/shape.o \
	  src/base/material.o \
	  src/base/object.o \
	  src/base/scene.o \
	  \
	  src/image/image.o \
	  \
	  src/opengl/openglwindow.o \
	  src/opengl/shader.o \
	  src/opengl/mesh.o \
	  src/opengl/texture1D.o \
	  src/opengl/texture2D.o \
	  src/opengl/framebuffer.o \
	  \
	  src/renderer/renderer.o \
	  \
	  src/parser/sceneparser.o \
	  \
	  src/main/main.o
	@echo Linking Project...
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OUTPUT_DIR)
	@$(CPP_COMPILER) $(CFLAGS) $(LINK) $^ -o $(BIN_DIR)/$@
	@echo Successfully built binary $(BIN_DIR)/$@.

third-party/glad/src/glad.o: third-party/glad/src/glad.c
	@echo Compiling $^...
	@$(C_COMPILER) $(CFLAGS) -c $^ -o $@

%.o: %.cpp
	@echo Compiling $^...
	@$(CPP_COMPILER) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	@rm -rf $(BIN_DIR)
	@rm -f $(SHADER_DIR)/render.glsl
	@find . -name "*.o" | xargs rm -f
	@echo Project Cleaned.
