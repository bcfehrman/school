CC = gcc

OBJDIR = build
SRCDIR = src
INCDIR = /usr/include
BINDIR = bin

CFLAGS = -Wall -I$(INCDIR) -g
LFLAGS = -lgsl

all: $(BINDIR)/hw1b

$(OBJDIR)/hw1b.o: $(SRCDIR)/hw1b.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $^

$(BINDIR)/hw1b: $(OBJDIR)/hw1b.o | $(BINDIR)
	$(CC) $(LFLAGS) -o $@ $^

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

valgrind: $(BINDIR)/hw1b
	valgrind --leak-check=full --show-reachable=yes $(BINDIR)/hw1b

clean:
	rm -rf $(OBJDIR) $(BINDIR)
