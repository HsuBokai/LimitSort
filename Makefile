ODIR=.
LDIR =./lib
IDIR =./include
CC=g++
CFLAGS= -g -Wall -I$(IDIR)

_DEPS = writer.h rand.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = genData.o writer.o rand.cpp
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

target = genData

$(target): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(INCDIR)/*~ $(target)
