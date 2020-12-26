#OBLS specifies which files to compile as part of the project
OBJS = sdl_testing.cpp
CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2
OBJ_NAME = sdl_testing.out
all : $(OBJS)
	$(CC) $(OBJS) -w -lSDL2 -o sdl_testing.out

