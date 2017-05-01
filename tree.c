#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

int *take_stat(FILE *ifile);
node *add_leaf(char symb, int count);
int add_node(tree *huff_tree);
int comp(const void *a, const void *b);
int bin_search(tree *tree_t, node *new_node);


tree *build_tree(FILE *ifile)
{
	tree *new_tree_p, new_tree = {0};
	new_tree_p = malloc(sizeof(*new_tree_p));
	if (new_tree_p == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}
	*new_tree_p = new_tree;

	new_tree_p->stat = take_stat(ifile);
	if (new_tree_p->stat == NULL) {
		return NULL;
	}

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


int *take_stat(FILE *ifile)
{
	char buff[BUFF_SIZE] = {0};
	size_t count = 0;
	int *stat_table = (int *) calloc(MAXCHARS, sizeof(int));
	
	if (stat_table == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}

    while ((count = fread(buff, sizeof(char), BUFF_SIZE, ifile)) != 0) {
		for (int index = 0; index < count; index++) {
			++stat_table[(int) *(buff+index)];
		}
	}

	if (feof(ifile)) {
		++stat_table[3];
	}

	return stat_table;
}


node *add_leaf(char symb, int count)
{
	if (symb == 0 || count == 0) {
		return NULL;
	}

	node *new_leaf_p, new_leaf = {0};
	new_leaf_p = malloc(sizeof(*new_leaf_p));
	if (new_leaf_p == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}
	*new_leaf_p = new_leaf;
	new_leaf_p->symb = symb;
	new_leaf_p->count = count;

	return new_leaf_p;
}


int add_node(tree *huff_tree)
{
	if (huff_tree == NULL) {
		return 1;
	}

	node *first = huff_tree->root[0];
	node *second = huff_tree->root[1];
	
	//Deleting from the queue the first and the second pointers
	for (int index = 2; index < (huff_tree->leaf_qty); index++) {
		huff_tree->root[index-2] = huff_tree->root[index];
	}
	huff_tree->root[huff_tree->leaf_qty-2] = NULL;
	huff_tree->root[huff_tree->leaf_qty-1] = NULL;
	huff_tree->leaf_qty -= 2;

	//Alloc a new node
    node *new_node_p, new_node = {0};
	new_node_p = malloc(sizeof(*new_node_p));
	if (new_node_p == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return 1;
	}
	*new_node_p = new_node;

	//Merge two nodes from the queue
	new_node_p->count = first->count + second->count;
	new_node_p->left = first;
	new_node_p->right = second;
	
	//Insert a new node in the queue
	int insrt_index = bin_search(huff_tree, new_node_p);

	if (insrt_index < huff_tree->leaf_qty) {
		for (int index = huff_tree->leaf_qty; index > insrt_index; index--) {
			huff_tree->root[index] = huff_tree->root[index-1];
		}
		huff_tree->leaf_qty++;
		huff_tree->root[insrt_index] = new_node_p;
	} else if (insrt_index == huff_tree->leaf_qty) {
		huff_tree->root[huff_tree->leaf_qty++] = new_node_p;
	}

	return 0;
}


int comp(const void *a, const void *b)
{
	node * const * p = (node * const *) a;
	node * const * q = (node * const *) b;
	return (*p)->count - (*q)->count;
}

int bin_search(tree *tree_t, node *new_node)
{
	int first = 0;
	int last = tree_t->leaf_qty;

	if (last == 0) {
		return 0; //Queue is empty
	} else if (tree_t->root[0]->count >= new_node->count) {
		return 0;
	} else if (tree_t->root[last-1]->count < new_node->count) {
		return last; //Insert in the end of queue
	}

	while (first < last) {
		int mid = first + (last - first) / 2;

		if (new_node->count <= tree_t->root[mid]->count) {
			last = mid;
		} else {
			first = mid + 1;
		}
	}
	
	return last;
}


int dealloc_tree(node *pointer)
{
	if (pointer == NULL) {
		return 0;
	}

	dealloc_tree(pointer->left);
	dealloc_tree(pointer->right);

	free(pointer);

	return 0;
}


void print_tree(node *root_t)
{
	if (root_t == NULL) {
		return;
	}

	print_tree(root_t->left);
	printf("%c - %d\n", root_t->symb, root_t->count);
	print_tree(root_t->right);
}
