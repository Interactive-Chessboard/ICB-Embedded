# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src -I./lib/func

# Source directories
SRC_DIR := src
LIB_DIR := lib/func

# Target executable
TARGET := main

# All .cpp files in src and lib/func
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(LIB_DIR)/*.cpp)

# Default: compile and run
all: $(TARGET)
	./$(TARGET)

# Build the executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(TARGET)

.PHONY: all clean
