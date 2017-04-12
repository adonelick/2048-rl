# Written by Andrew Donelick
# EELE 577 - Advanced Digital Signal Processing
# Final Project

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -std=c++11 uses the C++11 standard when compiling
CFLAGS  = -g -Wall -std=c++11

# the build target executable:
TARGETS = play2048 afterStateLearning qLearning stateLearning

all: $(TARGETS)

play2048: play2048.o state.o game.o
	$(CC) $(CFLAGS) -o play2048 play2048.o state.o game.o

afterStateLearning: afterStateLearning.o game.o state.o ntnn.o
	$(CC) $(CFLAGS) -o afterStateLearning afterStateLearning.o state.o game.o ntnn.o

qLearning: qLearning.o game.o state.o ntnn.o
	$(CC) $(CFLAGS) -o qLearning qLearning.o state.o game.o ntnn.o

stateLearning: stateLearning.o game.o state.o ntnn.o
	$(CC) $(CFLAGS) -o stateLearning stateLearning.o game.o state.o ntnn.o

clean:
	$(RM) $(TARGET) *.o


# Define the dependencies
play2048.o: play2048.cpp state.hpp game.hpp
state.o: state.cpp state.hpp
	$(CC) -std=c++11 -c -o state.o state.cpp
game.o: game.cpp game.hpp state.hpp
	$(CC) -std=c++11 -c -o game.o game.cpp
afterStateLearning.o: afterStateLearning.cpp state.hpp game.hpp ntnn.hpp
	$(CC) -std=c++11 -c -o afterStateLearning.o afterStateLearning.cpp
ntnn.o: ntnn.cpp ntnn.hpp
	$(CC) -std=c++11 -c -o ntnn.o ntnn.cpp
qLearning.o: qLearning.cpp game.hpp state.hpp ntnn.hpp
	$(CC) -std=c++11 -c -o qLearning.o qLearning.cpp
stateLearning.o: stateLearning.cpp game.hpp state.hpp ntnn.hpp
	$(CC) -std=c++11 -c -o stateLearning.o stateLearning.cpp
	