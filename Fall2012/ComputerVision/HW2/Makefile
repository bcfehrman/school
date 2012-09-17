CC = gcc
CPP = g++

OBJDIR = build
SRCDIR = src
INCDIR = /usr/include
LIBDIR = /usr/lib
BINDIR = bin
EXEC = hw2_opencv

CFLAGS = -Wall -I$(INCDIR) -g
LFLAGS = -L$(LIBDIR) -lgsl -lm -lopencv_highgui -lopencv_core


all: $(BINDIR)/$(EXEC)

$(OBJDIR)/$(EXEC).o: $(SRCDIR)/$(EXEC).c | $(OBJDIR)
	$(CPP) $(CFLAGS) -c -o $@ $^

$(BINDIR)/$(EXEC): $(OBJDIR)/$(EXEC).o | $(BINDIR)
	$(CPP) $(LFLAGS) -o $@ $^

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

valgrind: $(BINDIR)/$(EXEC)
	valgrind --leak-check=full --show-reachable=yes $(BINDIR)/$(EXEC)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
