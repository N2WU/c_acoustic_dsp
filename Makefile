# Compiler
CXX := g++

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Target
TARGET := $(BUILD_DIR)/app

# Flags
CXXFLAGS := -Wall -Wextra -I$(INC_DIR)

LDFLAGS :=

# Source files
SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/RtAudio.cpp \
        $(SRC_DIR)/bpsk.cpp

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Compile rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean