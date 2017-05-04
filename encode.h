#ifndef ENCODE
#define ENCODE

#include "global.h"
#include <string.h>
#include "global_opt.h"

#define BITS_LEN 8

typedef struct buff_tools {
	unsigned char bit;
	char *input;
	char *output;
	int bit_count;
	int i_count;
	int o_count;
} tools;

int encode(global_opt *, char **);
int bit_coding(tools *, global_opt *, char *);
int check_buff(tools *, global_opt *);

#endif //ENCODE
