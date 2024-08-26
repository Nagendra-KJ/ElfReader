CC =  gcc
CFLAGS = -Wall -g -std=c99
OBJFILE = ../Code_Example/hello_world_1.o
DEBUG = lldb
SRC_DIR = ./src
BUILD_DIR = ./build
UTILS_DIR = ./utils

.PHONY: all clean build

all: readelf
	./$(BUILD_DIR)/readelf $(OBJFILE)

utils.o: build
	$(MAKE) -C $(UTILS_DIR) BUILD_DIR=$(abspath $(BUILD_DIR))

readelf: utils.o build
	$(MAKE) -C $(SRC_DIR) BUILD_DIR=$(abspath $(BUILD_DIR)) UTILS_DIR=$(abspath $(UTILS_DIR))

debug: readelf
	$(DEBUG) ./$(BUILD_DIR)/readelf $(OBJFILE)

build:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build
	rm -rf readelf.dSYM
