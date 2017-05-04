/**
 * @file	 main.c
 * @brief	 main file
 *
 * Main file for huffman archiver
 * @author	 Anton Svechnikov
 * @license	 BSD
 */

#include "global.h"
#include "global_opt.h"
#include <time.h>
global_opt *parse_args(int, char **);
tree *build_tree(FILE *);
int huff_coding(node *, char *, char **);
int compress(global_opt *, unsigned long *, char **);
int extract(global_opt *);
int close_func(global_opt *, tree *, char **);

/**
 * @brief main function
 * 
 * Do necessary preparations and compile or extract target file.
 *
 * @param argc Quantity of input parameters
 * @param argv Array of input parameters'
 *
 *
 */
int main(int argc, char *argv[])
{
	clock_t start = clock();
	tree *new_tree = NULL;
	char *huff_code[MAXCHARS] = {0};
	global_opt *current_session = parse_args(argc, argv);

	if (current_session->opt == 'c') {
		new_tree = build_tree(current_session->ifile_p);
		huff_coding(new_tree->root[0], "", huff_code);
		compress(current_session, new_tree->stat, huff_code);
		clock_t finish = clock();
		double time = (double) (finish - start) / CLOCKS_PER_SEC;
		printf("%f sec\n", time);
		close_func(current_session, new_tree, huff_code);
	}

	if (current_session->opt == 'x') {
		extract(current_session);
		clock_t finish = clock();
		double time = (double) (finish - start) / CLOCKS_PER_SEC;
		printf("%f sec\n", time);
		close_func(current_session, NULL, NULL);
	}
}

/**
 * @brief Close files and free memory
 *
 * Free huffman tree, free array if huffman codes and close input
 * and output files
 *
 * @param session Pointer to structure with input and output file descriptor
 * @param huff_tree Pointer to structure with all information about tree and
 * its pointer
 * @param huff_code Array of huffman codes
 *
 */
int close_func(global_opt *session, tree *huff_tree, char *huff_code[])
{
	if (huff_tree != NULL) {
		dealloc_tree(huff_tree->root[0]);
		free(huff_tree->root);
		free(huff_tree->stat);
		free(huff_tree);
	}

	if (huff_code != NULL) {
		for (int index = 0; index < MAXCHARS; index++) {
			if (huff_code[index]) {
				free(huff_code[index]);
			}
		}
	}

	fclose(session->ifile_p);
	fclose(session->ofile_p);
	free(session);
	exit(0);
}
