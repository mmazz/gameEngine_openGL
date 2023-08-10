CC = gcc
CXX = g++
CFLAGS= -C -Wall -I lib/include
CXXFLAGS = -g -Wall -Wextra -std=c++2a -Ilib/include -I/usr/include/freetype2

LDLIBS = -lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11  -lpthread -ldl -lXinerama -lXcursor -lfreetype

OBJDIR = obj
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRC))

APP = app
all: $(APP)

app: dir $(OBJDIR)/glad.o $(OBJ)
	$(CXX) $(CXXFLAGS)  $(LDLIBS) $(OBJDIR)/glad.o $(OBJ) -o $(APP)

$(OBJDIR)/glad.o: src/glad.c
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dir:
	mkdir -p $(OBJDIR)
.PHONY: clean run
clean:
	find . -type f | xargs touch
	rm -rf obj/*
	rm ./$(APP)
