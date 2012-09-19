CC = gcc
CPP = g++

OBJDIR = build
SRCDIR = src
INCDIR = /usr/include
LIBDIR = /usr/lib
BINDIR = bin
EXEC = prog1

CFLAGS = -Wall -I$(INCDIR) -g
LFLAGS = -L$(LIBDIR) -lgsl -lm


all: $(BINDIR)/$(EXEC)

$(OBJDIR)/$(EXEC).o: $(SRCDIR)/$(EXEC).cpp | $(OBJDIR)
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
