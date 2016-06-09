CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
INCDIR=
SOURCES=main.c, sched.c, mmu.c, tss.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all : $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
clean: 
	rm -rf $(EXECUTABLE) $(OBJECTS)
.c.o:
	$(CC) $(CFLAGS) $< -c -o $@
