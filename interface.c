#include <stdlib.h>
#include <string.h>
#include "main.h"


interface *input_opt(int arg_count, char **func_arg)
{
	interface *new = malloc(sizeof(interface));
	
	if (arg_count < 4) {
		fprintf(stderr, "%s: use -h for help.\n", func_arg[0]);
	}

	//check input file
	if ((new->ifile_p = fopen(func_arg[1], "rb")) == NULL) 
	{
		fprintf(stderr, "%s: incorrect name of input file.\n", func_arg[0]);
        exit(1);
	} else {
		new->ifile = func_arg[1];
	}

	if (!fseek(new->ifile_p, 0L, SEEK_END)) {
		int check_size = ftell(new->ifile_p);
		if (check_size == 0) {
			fprintf(stderr, "%s: zero size file.\n", func_arg[0]);
			exit(1);
		}
		fseek(new->ifile_p, 0L, SEEK_SET);
	}


	//check option of archiver
	if (strcmp(func_arg[2], "-c") == 0 || strcmp(func_arg[2], "-e") == 0) {
		new->opt = func_arg[2];
	} else {
		fprintf(stderr, "%s: please choose right option -c / -e.\n", func_arg[0]);
        exit(1);
	}

	//check output file
	if (func_arg[3]) {
		new->ofile = func_arg[3];
	} else {
		fprintf(stderr, "%s: please specify name of an archive.\n", func_arg[0]);
        exit(1);
	}

	return new;
}

