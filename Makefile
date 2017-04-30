CC=gcc
CFLAGS=-c -Wall -pedantic -std=c99
DEBUG=-g
TMP_DIR=out
SRCS=encode.c main.c global_opt.c tree.c 
OBJS=$(SRCS:.c=.o)
HUFF=huff

.PHONY: all clean

all: $(HUFF)

$(HUFF): $(OBJS)
	@$(CC) $(addprefix $(TMP_DIR), $(addprefix /, $^)) -o $@
	@echo "Huffman arch compiled."

%.o: %.c
	@mkdir $(TMP_DIR)
	@$(CC) $^ $(CFLAGS) -o $(addprefix $(TMP_DIR), $(addprefix /, $@))

clean:
	@rm $(HUFF) -r $(TMP_DIR)
	@echo "Done."
