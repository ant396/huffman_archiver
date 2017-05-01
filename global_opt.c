#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "global_opt.h"

global_opt *parce_args(int arg_count, char **func_arg)
{
	FILE *temp = NULL;
	int c = 0;
	char *ifile = func_arg[1];
	char *ofile = func_arg[3];
	char opt = *(func_arg[2]+1);

	while ((c = getopt(arg_count, func_arg, "c:e:h")) != -1) {
		switch(c) {
			case 'c':
				break;
			case 'e':
				break;
			case 'h':
				printf("Usage: ifile [-Compress / -Extract] ofile.\n");
				exit(0);
			case '?':
			default:
				printf("%s: use -h for help.\n", func_arg[0]);
				exit(1);
		}
	}

	if ((temp = fopen(ifile, "rb")) == NULL) {
		fprintf(stderr, "%s: incorret name of input file.\n", func_arg[0]);
		exit(1);
	}

	if (fseek(temp, 0L, SEEK_END) == 0) {
		long check_size = ftell(temp);
		if (check_size == 0) {
			fprintf(stderr, "%s: input file is empty.\n", func_arg[0]);
			exit(1);
		} else if (check_size == -1) {
			fprintf(stderr, "%s: file corrupted.\n", func_arg[0]);
			exit(1);
		} else {
			rewind(temp);
		}
	}

	if (arg_count != 4) {
		printf("Usage: ifile [-Compress / -Extract] ofile.\n");
		exit(0);
	}

	global_opt *input_opt_p, input_opt = {0};
	input_opt_p = malloc(sizeof(*input_opt_p));
	*input_opt_p = input_opt;
	
	input_opt_p->opt = opt;
	input_opt_p->ifile_p = temp;
	input_opt_p->ifile = ifile;
	input_opt_p->ofile = ofile;

	return input_opt_p;
}
