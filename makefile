CC=gcc
CFLAGS=-c 
SOURCES=ParSeq.c Reader.c P_LFMP.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ParSequencer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ParSequencer