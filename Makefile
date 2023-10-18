CC = gcc
CFLAGS = -Wall
LDFLAGS =
TARGET = mouse_movement_simulator
SOURCES = main.c
OS := $(shell uname)

ifeq ($(OS), Darwin)
    LDFLAGS += -lX11 -lXtst
    TARGET = mouse_movement_simulator_macos
else
ifeq ($(OS), Linux)
    LDFLAGS += -lX11 -lXtst
    TARGET = mouse_movement_simulator_linux
else
ifeq ($(OS), Windows_NT)
    LDFLAGS += -luser32
    TARGET = mouse_movement_simulator.exe
endif
endif
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)
