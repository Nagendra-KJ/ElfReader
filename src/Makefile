CC = gcc
CFLAGS = -Wall -g -std=c99

.PHONY: all clean

all: readelf

clean:
	rm $(BUILD_DIR)/readelf

readelf:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/readelf readelf.c $(BUILD_DIR)/utils.o -I $(UTILS_DIR)
