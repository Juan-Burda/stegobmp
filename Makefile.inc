GCC=gcc 
GCCFLAGS=-Wall -std=c99 -pedantic -g $(INCLUDE)

LDFLAGS=-lm -lcrypto -lssl

INCLUDE=-Iinclude/
DIRS := src
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS := $(SOURCES:.c=.o)

BIN_DIR=./bin
BIN_FILE=$(BIN_DIR)/stegobmp