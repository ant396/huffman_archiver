CFLAGS=-c -Wall -pedantic -std=c99
SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
HUFF=huff

.PHONY: all clean

all: $(HUFF)

$(HUFF): $(OBJS)
	@gcc $^ -o $@
	@echo "Huffman arch compiled."

.o:.c
	@gcc $^ $(CFLAGS) -o $@

clean:
	@rm -r $(TMP_DIR)
	@rm $(HUFF)
	@echo "Done."
