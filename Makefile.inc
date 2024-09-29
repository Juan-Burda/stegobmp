GCC=gcc 
GCCFLAGS=-Wall -std=c99 -pedantic -g $(INCLUDE) -fsanitize=address

LDFLAGS=-lm -lcrypto -lssl -fsanitize=address

INCLUDE=-Iinclude/ -Iinclude/constants/
DIRS := src src/utils src/libs
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS := $(SOURCES:.c=.o)

BIN_DIR=./bin
BIN_FILE=$(BIN_DIR)/stegobmp