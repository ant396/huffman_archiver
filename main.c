#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "global_opt.h"


global_opt *parce_args(int arg_count, char **func_args);
tree *build_tree(FILE *ifile);
int compress(global_opt *session, int *stat, char **huff_code);
int close_func(global_opt *session, tree *huff_tree, char *huff_code[]);




void huff_coding(node *root, char *code, char *huff_code[])
{
	if (root == NULL) {
		return;
	}
	
	if (root->symb) {
		char *huff_code_p = malloc(sizeof(code));
        strcpy(huff_code_p, code);
		huff_code[(int) root->symb] = huff_code_p;
        return;
    }

	char left[MAXCHARS] = {0};
    strcpy(left, code);
    strcat(left, "0");
    huff_coding(root->left, left, huff_code);

    char right[MAXCHARS] = {0};
    strcpy(right, code);
    strcat(right, "1");
    huff_coding(root->right, right, huff_code);
}


int main(int argc, char **argv)
{
	tree *new_tree = NULL;
	char *huff_code[MAXCHARS] = {0};
	global_opt *current_session = parce_args(argc, argv);
	
	if (current_session->opt == 'c') {
		new_tree = build_tree(current_session->ifile_p);
		huff_coding(new_tree->root[0], "", huff_code);

		for (int c = 0; c < MAXCHARS; c++) {
			if (huff_code[c]) {
				printf("%c >> %s\n", c, huff_code[c]);
			}
		}

		compress(current_session, new_tree->stat, huff_code);
		close_func(current_session, new_tree, huff_code);
	}
}


int close_func(global_opt *session, tree *huff_tree, char *huff_code[])
{
	dealloc_tree(huff_tree->root[0]);
	free(huff_tree->stat);
	free(huff_tree);
	
	for (int index = 0; index < MAXCHARS; index++) {
		if (huff_code[index]) {
			free(huff_code[index]);
		}
	}

	fclose(session->ifile_p);
	fclose(session->ofile_p);
	free(session);
	exit(0);
}
