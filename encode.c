#include <string.h>
#include <stdlib.h>
#include "main.h"

#define BITS_LEN 8

typedef struct buff_tools {
	unsigned char bit;
	char *input;
	char *output;
	unsigned int bit_count;
	unsigned int i_count;
	unsigned int o_count;
} tools;

int encode(interface *, char **);
int bit_coding(tools *, interface *, char *);
int check_buff(tools *, interface *);


int compress(interface *session, int *stat, char **code)
{
	session->ofile_p = fopen(session->ofile, "wb");
	int check;
	
	fwrite(*stat, sizeof(int), MAXCHARS, session->ofile_p);

	check = encode(session, code);
	
	if (check == ok) {
		return ok;
	} else {
		return error;
	}
}


int encode(interface *session, char **code)
{
	tools buff = {
		.bit = 0;
		.input = calloc(BUFF_SIZE, sizeof(char));
		.output = calloc(BUFF_SIZE, sizeof(char));
		.bit_count = BITS_LEN;
		.i_count = 0;
		.o_count = 0;
	}
	
	session->ofile_p = fopen(session->ofile, "wb");
	fseek(session->ifile_p, 0L, SEEK_SET);
	
	size_t current_size;
	while ((current_size = fread(buff.input, sizeof(char), BUFF_SIZE, session->ifile_p))) {
		for ( ; buff.i_count < current_size; buff.i_count++) {
			bit_coding(&buff, session, code[(int) biff.input[buff.i_count]]);
		}
		buff.i_count = 0;
	}



int bit_coding(tools *p_buff, interface *session, char *code)
{
	int index;
	int code_len = strlen(code);

	for (index = 0; index < code_len; index++) {
		check_buff(p_buff, session);
		if (code[index] == '1') {
			p_buff->bit |= (1 << (p_buff->bit_count - 1));
		}
		p_buff->bit_count--;
	}

	return 0;
}



int chek_buff(tools *buff, interface *session)
{
	//check, write and cleam output buffer
	if (buff->o_count < BUFF_SIZE) {
		;
	} else {
		fwrite(buff->output, sizeof(char), buff->o_count, session->ofile_p);
		memset(buff->output, 0, BUFF_SIZE);
		buff->o_count = 0;
	}

	//check, write and clean raw bit
	if (buff->bit_count > 0) {
		;
	} else {
		buff->output[buff->o_count++] = buff->bit;
		buff->bit = 0;
	}

	return 0;
}
