# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Target executable name
TARGET = maze_game

# Source and Object files
SRCS = main.cpp character.cpp entity.cpp key.cpp ladder.cpp trap.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

# Libraries required
LIBS = -lncurses

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts (binaries and object files)
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean