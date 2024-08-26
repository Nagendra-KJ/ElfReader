CC =  gcc
CFLAGS = -Wall -g -std=c99
OBJFILE = ../Code_Example/hello_world_1.o
DEBUG = lldb

.PHONY: all clean

all: readelf
	./readelf $(OBJFILE)

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o utils.o utils.c

readelf: readelf.c readelf.h utils.o
	$(CC) $(CFLAGS) -o readelf readelf.c utils.o

debug: readelf
	$(DEBUG) ./readelf $(OBJFILE)

clean:
	rm readelf *.o
	rm -rf readelf.dSYM
