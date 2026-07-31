# Makefile for CryptoLabX

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I.
TARGET = cryptolabx
SOURCES = main.cpp utils/logger.cpp utils/file_analyzer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f outputs/activity.log

# Run the program
run: $(TARGET)
	./$(TARGET)

# Create necessary directories
setup:
	mkdir -p outputs

# Rebuild everything
rebuild: clean all

.PHONY: all clean run setup rebuild
