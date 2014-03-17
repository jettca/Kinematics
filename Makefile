CC = g++
CFLAGS = -g -Wall
TARGET = bin/main
OBJDIR = obj
SRCDIR = src
LIBRARIES = -I /usr/X11R6/include/ -L /usr/X11R6/lib64/ -lglut -lGL -lGLU -lX11 -lXmu -lXi -lm -I./src/Eigen
UNAME = $(shell uname)
OPS = -Wno-deprecated

ifeq ($(UNAME), Darwin)
	LIBRARIES = -framework OpenGL -framework GLUT
endif

# Link for target
$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/skeleton.o $(OBJDIR)/joint.o $(OBJDIR)/quaternion.o $(OBJDIR)/point.o
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJDIR)/* -o $(TARGET) $(LIBRARIES)

# Compile objects
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(OPS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/skeleton.o: $(SRCDIR)/skeleton.cpp $(SRCDIR)/skeleton.h $(SRCDIR)/joint.h
	$(CC) $(CFLAGS) $(OPS) -c $(SRCDIR)/skeleton.cpp -o $(OBJDIR)/skeleton.o

$(OBJDIR)/joint.o: $(SRCDIR)/joint.cpp $(SRCDIR)/joint.h $(SRCDIR)/point.h
	$(CC) $(CFLAGS) $(OPS) -c $(SRCDIR)/joint.cpp -o $(OBJDIR)/joint.o

$(OBJDIR)/quaternion.o: $(SRCDIR)/quaternion.cpp $(SRCDIR)/quaternion.h $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/quaternion.cpp -o $(OBJDIR)/quaternion.o

$(OBJDIR)/point.o: $(SRCDIR)/point.cpp $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/point.cpp -o $(OBJDIR)/point.o


.PHONY: clean
clean:
	rm -rf bin obj

.PHONY: all
all: clean $(TARGET)
