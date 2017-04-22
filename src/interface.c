#include <stdlib.h>
#include <string.h>
#include "main.h"


interface *input_opt(int arg_count, char **func_arg)
{
	interface *p_session, session = {0};
	p_session = malloc(sizeof(*p_session));
	*p_session = session;
	
	if (arg_count < 4) {
		fprintf(stderr, "%s: use -h for help.\n", func_arg[0]);
	}

	//check input file
	if ((p_session->ifile_p = fopen(func_arg[1], "rb")) == NULL) 
	{
		fprintf(stderr, "%s: incorrect name of input file.\n", func_arg[0]);
        exit(1);
	} else {
		p_session->ifile = func_arg[1];
	}

	if (!fseek(p_session->ifile_p, 0L, SEEK_END)) {
		int check_size = ftell(p_session->ifile_p);
		if (check_size == 0) {
			fprintf(stderr, "%s: zero size file.\n", func_arg[0]);
			exit(1);
		}
		fseek(p_session->ifile_p, 0L, SEEK_SET);
	}


	//check option of archiver
	if (strcmp(func_arg[2], "-c") == 0 || strcmp(func_arg[2], "-e") == 0) {
		p_session->opt = func_arg[2];
	} else {
		fprintf(stderr, "%s: please choose right option -c / -e.\n", func_arg[0]);
        exit(1);
	}

	//check output file
	if (func_arg[3]) {
		p_session->ofile = func_arg[3];
	} else {
		fprintf(stderr, "%s: please specify name of an archive.\n", func_arg[0]);
        exit(1);
	}

	return p_session;
}

