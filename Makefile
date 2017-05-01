CFLAGS=-c -Wall -pedantic #-std=c99
DBUG=-g -O0
SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
HUFF=huff
HUFF_DBUG=huff_test

.PHONY: all test clean clean_test

all: $(HUFF)

$(HUFF): $(OBJS)
	@gcc $^ -o $@
	@echo "Huffman arch compiled."

.o:.c
	@gcc $^ $(CFLAGS) -o $@

test: test_file

test_file: $(OBJS)
	@gcc $^ -o $(HUFF_DBUG)
	@echo "Huffman arch for test and debug compiled."

$(OBJS):$(SRCS)
	@gcc $^ $(CFLAGS) $(DBUG)

clean:
	@rm $(OBJS)
	@rm $(HUFF)
	@echo "Done."

clean_test:
	@rm $(OBJS)
	@rm $(HUFF_DBUG)
	@echo "Done."
