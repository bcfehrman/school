CC = gcc
CPP = g++

OBJDIR = build
SRCDIR = src
INCDIR = /usr/include
LOCINCDIR = include
LIBDIR = /usr/lib
LOCLIBDIR = lib/
BINDIR = bin
EXEC = prog1
LIBRARY = lib/libvision_functions

CFLAGS = -Wall -I$(INCDIR) -I$(LOCINCDIR) -g
LFLAGS = -L$(LOCLIBDIR) -L$(LIBDIR) -lgsl -lm -lopencv_highgui -lopencv_core -lopencv_imgproc -lvision_functions


all: $(BINDIR)/$(EXEC)

$(OBJDIR)/$(EXEC).o: $(SRCDIR)/$(EXEC).cpp | $(OBJDIR)
	$(CPP) $(CFLAGS) -c -o $@ $^

$(BINDIR)/$(EXEC): $(OBJDIR)/$(EXEC).o | $(BINDIR)
	$(CPP) $(LFLAGS) -o $@ $^

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)
	
library: $(LIBRARY)

$(LIBRARY):
	$(CPP) -o $@.so $@.cpp -shared $(CFLAGS) -c


valgrind: $(BINDIR)/$(EXEC)
	valgrind --leak-check=full --show-reachable=yes $(BINDIR)/$(EXEC)

clean:
	rm -rf $(OBJDIR) $(BINDIR) lib/*.so
