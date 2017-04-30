#ifndef ENCODE
#define ENCODE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "global_opt.h"
#include "tree.h"

#define BITS_LEN 8

typedef struct buff_tools {
	unsigned char bit;
	char *input;
	char *output;
	unsigned int bit_count;
	unsigned int i_count;
	unsigned int o_count;
} tools;

int encode(global_opt *, char **);
int bit_coding(tools *, global_opt *, char *);
int check_buff(tools *, global_opt *);

#endif //ENCODE
