CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
CPPFLAGS += -Isrc/engine -Isrc/game

RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
RAYLIB_FRAMEWORKS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework AudioToolbox -framework CoreVideo
endif

SRC := main.cpp $(wildcard src/engine/*.cpp) $(wildcard src/game/*.cpp)
BIN := pacman

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(RAYLIB_CFLAGS) $(SRC) -o $@ $(RAYLIB_LIBS) $(RAYLIB_FRAMEWORKS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean
