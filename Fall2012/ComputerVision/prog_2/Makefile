CC = gcc
CPP = g++

OBJDIR = build
SRCDIR = src
INCDIR = /usr/include
LOCINCDIR = include
LIBDIR = /usr/lib
LIBDIR64 = /usr/lib64
BINDIR = bin
EXEC = prog_2

CFLAGS = -Wall -I$(INCDIR) -I$(LOCINCDIR) -g
LFLAGS = -L$(LIBDIR) -L$(LIBDIR64) -lgsl -lm -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_features2d


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
	rm -rf $(OBJDIR) $(BINDIR) lib/*.so
