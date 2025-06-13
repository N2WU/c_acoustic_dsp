# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -lasound -lpthread -D__LINUX_ALSA__

# Target executable
TARGET = rt_sine

# For deleting the target
TARGET_DEL = rt_sine.exe

# Source files
SRCS = RtAudio.cpp rt_sine.cpp

# Object files
OBJS = rt_sine.o

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)