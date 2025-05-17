CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ilibs/simdjson/include -Ilibs/simdjson/src -Ilibs/raylib/src
LDFLAGS = -Llibs/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source files
SRCS = main.cpp utilities.cpp libs/simdjson/src/simdjson.cpp
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = CaseOpener.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
