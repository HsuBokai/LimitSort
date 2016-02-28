LDIR =./lib
IDIR =./include
CC=g++
CFLAGS= -g -Wall -I$(IDIR)

TARGET1 = genData
OBJ1 = genData.o rand.o writer.o
TARGET2 = main
OBJ2 = main.o writer.o reader.o
TARGETS = $(TARGET1) $(TARGET2)

$(TARGET2): $(OBJ2)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET1): $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *~ $(INCDIR)/*~ $(TARGETS)
