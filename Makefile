CC=gcc
CFLAGS=-c -Wall -pedantic -std=c99
TMP_DIR=out
SRC_DIR=src
SRCS=decode.c encode.c huff.c interface.c tree_func.c 
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=huff

all: start $(TARGET)
	
start:
	mkdir $(TMP_DIR)

$(TARGET): $(SRS)
	$(CC) $(CFLAGS) -o $^
