CC =  gcc
CFLAGS = -Wall -g -std=c99

.PHONY: all clean

all: readelf

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o utils.o utils.c

readelf: readelf.c readelf.h utils.o
	$(CC) $(CFLAGS) -o readelf readelf.c utils.o

clean:
	rm readelf *.o
	rm -rf readelf.dSYM
