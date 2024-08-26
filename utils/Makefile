BUILD_DIR? = build
CC = gcc
CFLAGS = -Wall -g -std=c99 -c

.PHONY: all clean

all: utils.o

clean:
	rm $(BUILD_DIR)/utils.o

utils.o:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/utils.o utils.c
