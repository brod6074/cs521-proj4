CXXFLAGS = `pkg-config --cflags playerc++`
CFLAGS = `pkg-config --cflags playerc`
LDLIBS = `pkg-config --libs playerc++`
CC = g++
NAME = test

all: 
	$(CC) $(CXXFLAGS) navigator.cpp test.cc $(LDLIBS) -o $(NAME)