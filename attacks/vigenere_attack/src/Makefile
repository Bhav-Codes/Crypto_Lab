CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = vigenere_cryptanalysis
SOURCES = main.cpp kasiski_analysis.cpp frequency_analyzer.cpp vigenere_cipher.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test1: $(TARGET)
	./$(TARGET) ../testcases/ciphertext1_odd.txt

test2: $(TARGET)
	./$(TARGET) ../testcases/ciphertext2_even.txt

.PHONY: all clean run test1 test2
