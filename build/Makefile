# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O2

# Project root directory (relative to build directory)
PROJECT_ROOT = ..

# Include directories for header files when compiling object files
INCLUDES = -I$(PROJECT_ROOT)

# Source directories
SCENE_OBJECTS_DIR = $(PROJECT_ROOT)/scene_objects
TOOLS_DIR = $(PROJECT_ROOT)/tools

# Tell Make where to find source files
VPATH = $(PROJECT_ROOT) $(SCENE_OBJECTS_DIR) $(TOOLS_DIR)

# Object file directory
OBJ_DIR = obj

# Find all .cpp files in the directories
SCENE_OBJECTS_SRCS = $(wildcard $(SCENE_OBJECTS_DIR)/*.cpp)
TOOLS_SRCS = $(wildcard $(TOOLS_DIR)/*.cpp)
MAIN_SRC = $(PROJECT_ROOT)/main.cpp

# All source files
SRCS = $(MAIN_SRC) $(SCENE_OBJECTS_SRCS) $(TOOLS_SRCS)

# Object files (put them in obj directory, flatten the structure)
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Output executable name
TARGET = main

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Universal rule: compile any .cpp to obj/%.o
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Mark targets as phony
.PHONY: all clean rebuild