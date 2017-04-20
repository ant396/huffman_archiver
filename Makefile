CC=gcc
CFLAGS=-c -Wall -pedantic -std=c99
DEBUG=-g
TMP_DIR=out
SRC_DIR=src
SRCS=decode.c encode.c main.c interface.c tree_func.c 
OBJS=$(SRCS:.c=.o)
TARGET=huff

.PHONY: all clean

all: preperation $(TARGET)

preperation:
	@mkdir $(TMP_DIR)

$(TARGET): $(OBJS)
	@$(CC) $(addprefix $(TMP_DIR), $(addprefix /, $^)) -o $@
	@echo "Huffman arch compiled."

%.o: $(addprefix $(SRC_DIR), $(addprefix /, %.c))
	@$(CC) $^ $(CFLAGS) -o $(addprefix $(TMP_DIR), $(addprefix /, $@))

clean:
	@rm $(TARGET) -r $(TMP_DIR)
	@echo "Done."
