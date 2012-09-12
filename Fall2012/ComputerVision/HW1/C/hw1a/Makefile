IDIR =/usr/include
CC=gcc
CPP=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =/usr/lib

LIBS=-lcv -lhighgui -lm -lcvaux

_DEPS = cv.h cvaux.h highgui.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hw1a.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make_c: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

make_cpp: $(OBJ)
	$(CPP) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
