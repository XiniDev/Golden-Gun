# Bundle & App stuff
APP_NAME = Golden_Gun
APP_VERSION = 1.0
APP_INFO = © 2023 by Xini

# Check for the presence of MinGW or MSVC
ifeq ($(OS),Windows_NT)
    # assume 64 bit mingw
    COMPILER = MINGW64
else
    OS = $(shell uname -s)
    ifeq ($(OS),Darwin)
        COMPILER = NATIVE
    else
        $(error This Makefile is intended for Windows 64bit MINGW and macOS systems only)
    endif
endif

# lib and include
LIB = -L lib
INCLUDE = -I include

# Compiler and flags
ifeq ($(COMPILER),MINGW64)
    CC = x86_64-w64-mingw32-g++
    SDL_LIB = -L src/mingw/x86_64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
    SDL_INCLUDE = -I src/mingw/x86_64/include
    RM_CMD = rm -Force
else ifeq ($(COMPILER),NATIVE)
    CC = g++
    SDL_LIB = -framework SDL2 -framework SDL2_image
    SDL_INCLUDE = -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -F/Library/Frameworks
    RM_CMD = rm -f
else
    $(error Unsupported compiler: $(COMPILER))
endif

# Compile flags
CFLAGS = -std=c++23 -Wall $(INCLUDE) $(SDL_INCLUDE)
LDFLAGS = $(LIB) $(SDL_LIB)

# Source and object file
SRCS := $(wildcard *.cpp ./lib/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
# Target executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
    ifeq ($(COMPILER),MINGW64)
		powershell "echo $(TARGET).exe $(OBJS) | $(RM_CMD)"
    else ifeq ($(COMPILER),NATIVE)
		$(RM_CMD) $(TARGET) $(OBJS)
    else
        $(error Unsupported compiler: $(COMPILER))
    endif

.PHONY: all clean

# g++ -I src/mingw/x86_64/include -L src/mingw/x86_64/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2