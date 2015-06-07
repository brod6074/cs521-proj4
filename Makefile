CXXFLAGS = `pkg-config --cflags playerc++`
CFLAGS = `pkg-config --cflags playerc`
LDLIBS = `pkg-config --libs playerc++`
CC = g++
NAME = proj4

all: 
	$(CC) $(CXXFLAGS) navigator.cpp pilot.cpp act.cpp proj4.cc $(LDLIBS) -o $(NAME)