CXX      := g++
CXXFLAGS := -Wall -O2 -std=c++17
RAYLIB   := $(shell pkg-config --cflags --libs raylib)
SRC      := src/burgers.cpp
TARGET   := burgers.exe

# Default target: build the simulator
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(RAYLIB)

# Build (if needed) and run
run: $(TARGET)
	./$(TARGET)

# Remove the compiled binary
clean:
	rm -f $(TARGET)

.PHONY: run clean
