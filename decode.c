/**
 * @file	decode
 * @brief	Decode archive
 *
 * Decode input archive.
 * @author	 Anton Svechnikov
 * @license	 BSD
 *
 */

#include "encode.h"

tree *build_tree_arch(FILE *ifile);

/**
 * @brief Extract data from input file
 *
 * Decode information from input archive
 *
 * @param Pointer on structer with all input 
 * options and parameters
 *
 */
int extract(global_opt *session)
{
	session->ofile_p = fopen(session->ofile, "wb");
	tree *arch_tree = build_tree_arch(session->ifile_p);
	
	tools buff = {0};
	buff.input = calloc(BUFF_SIZE, sizeof(char));
	buff.output = calloc(BUFF_SIZE, sizeof(char));
	buff.bit_count = BITS_LEN;

	node *temp_tree_root = arch_tree->root[0];

	size_t current_size;
	while ((current_size = fread(buff.input, sizeof(char), BUFF_SIZE, session->ifile_p))) {
		for ( ; buff.i_count < current_size; buff.i_count++) {
			for (int count = buff.bit_count; count > 0; count--) {
				if (buff.input[buff.i_count] & (1 << (count - 1))) {
					temp_tree_root = temp_tree_root -> right;
				} else {
					temp_tree_root = temp_tree_root->left;
				}

				if ((int) temp_tree_root->symb == 3) {
					break;
				}

				if (temp_tree_root->symb) {
					if (buff.o_count == BUFF_SIZE) {
						fwrite(buff.output, sizeof(char), BUFF_SIZE,\
						 session->ofile_p);
						memset(buff.output, 0, BUFF_SIZE);
						buff.o_count = 0;
					}
					buff.output[buff.o_count++] = temp_tree_root->symb;
					temp_tree_root = arch_tree->root[0];
				}
			}
		}
		buff.i_count = 0;
	}

	if (buff.o_count > 0) {
		fwrite(buff.output, sizeof(char), buff.o_count, session->ofile_p);
	}
	
	dealloc_tree(arch_tree->root[0]);
	free(arch_tree->root);
	free(arch_tree->stat);
	free(arch_tree);
	free(buff.input);
	free(buff.output);
	
	return 0;
}

/**
 * @brief Build tree
 *
 * Build tree based on input archive data
 *
 * @param Pointer file descriptor
 *
 * @return Pointer on structer with information 
 * about built tree
 *
 */
tree *build_tree_arch(FILE *ifile)
{
	tree *new_tree_p, new_tree = {0};
	new_tree_p = malloc(sizeof(*new_tree_p));
	if (new_tree_p == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}
	*new_tree_p = new_tree;

	new_tree_p->stat = calloc(MAXCHARS, sizeof(int));
	if (new_tree_p->stat == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}

	fread(new_tree_p->stat, sizeof(int), MAXCHARS, ifile);

	node **queue = calloc(MAXCHARS, sizeof(node *));

    for (int index = 0; index < MAXCHARS; index++) {
        if (new_tree_p->stat[index] > 0) {
            queue[new_tree_p->leaf_qty++] = add_leaf((char) index,\
			 new_tree_p->stat[index]);
        }
	}
	new_tree_p->root = queue;

	qsort(queue, new_tree_p->leaf_qty, sizeof(node*), comp);

	while (new_tree_p->leaf_qty > 1) {
        add_node(new_tree_p);
	}

	return new_tree_p;
}
