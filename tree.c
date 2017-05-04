/**
 * @file	tree
 * @brief	Build tree
 *
 * Include all necessary functions to buld tree
 * @author	 Anton Svechnikov
 * @license	 BSD
 *
 */

#include "global.h"

unsigned long *take_stat(FILE *);
int bin_search(tree *, node *);

/**
 * @brief Build tree
 *
 * Take statistics from an input file and build tree fo 
 * further huffman code
 *
 * @param Input file descriptor
 *
 * @return Pointet on structer with information about tree and 
 * its pointer
 *
 */
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
            queue[new_tree_p->leaf_qty++] = add_leaf((unsigned char) index,\
			new_tree_p->stat[index]);
        }
	}
	new_tree_p->root = queue;

	qsort(new_tree_p->root, new_tree_p->leaf_qty, sizeof(node *), comp);

	while (new_tree_p->leaf_qty > 1) {
        add_node(new_tree_p);
	}

	return new_tree_p;
}

/**
 * @brief Take statistics
 *
 * Take statistics from an input about about symbols
 *
 * @param Input file descriptor
 *
 * @return Pointer on array
 *
 */
unsigned long *take_stat(FILE *ifile)
{
	unsigned char buff[BUFF_SIZE] = {0};
	size_t count = 0;
	unsigned long *stat_table = (unsigned long *) calloc(MAXCHARS, sizeof(unsigned long));
	
	if (stat_table == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return NULL;
	}

    while ((count = fread(buff, sizeof(unsigned char), BUFF_SIZE, ifile)) != 0) {
		for (int index = 0; index < count; index++) {
			++stat_table[(int) *(buff+index)];
		}
	}

	return stat_table;
}

/**
 * @brief Add leaf to tree
 *
 * Add leaf to tree based on took statistics of input file
 *
 * @param symb Symbol from input file
 * @param count Quantity of the mentioned symbol in an input file
 *
 * @return Pointet on leaf
 *
 */
node *add_leaf(unsigned char symb, unsigned long count)
{
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

/**
 * @brief Add node to tree
 *
 * Merge nodes and leaf in one node
 *
 * @param Pointer in tree structure
 *
 * @return 1 in case of error
 *
 */
int add_node(tree *huff_tree)
{
	if (huff_tree == NULL) {
		return 1;
	}
	
	if (huff_tree->root[0] == NULL || huff_tree->root[1] == NULL) {
		return 1;
	}

	node *first_el = huff_tree->root[0];
	node *second_el = huff_tree->root[1];
	
	//Alloc a new node
    node *new_node_p, new_node = {0};
	new_node_p = malloc(sizeof(*new_node_p));
	if (new_node_p == NULL) {
		fprintf(stderr, "Error! Memory not allocated.\n");
		return 1;
	}
	*new_node_p = new_node;

	//Merge two nodes from the queue
	new_node_p->count = first_el->count + second_el->count;
	new_node_p->left = first_el;
	new_node_p->right = second_el;

	//Deleting from the queue the first and the second pointers
	for (int index = 2; index < (huff_tree->leaf_qty); index++) {
		huff_tree->root[index-2] = huff_tree->root[index];
	}
	huff_tree->root[huff_tree->leaf_qty-2] = NULL;
	huff_tree->root[huff_tree->leaf_qty-1] = NULL;
	huff_tree->leaf_qty -= 2;

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

/**
 * @brief Comparator for qsort
 *
 * Compare nodes by quantity
 *
 * @param Pointers on two elements which should be compared
 * @see qsort
 * @return Return value < 0 if the first paramets less than the secont or
 * return value > 0 if the second parameter less than the first or
 * return = 0 if the both parameters are equal
 *
 */
int comp(const void *a, const void *b)
{
	const node **first_p = (const node **) a;
	const node **second_p = (const node **) b;
	return (*first_p)->count - (*second_p)->count;
}

/**
 * @brief Binary search
 *
 * Binary search for input node into the queue
 *
 * @param tree_t Pointer on tree structure
 * @param new_node Pointer on node which shoud be input in queue
 *
 * @return Index where node should be input
 *
 */
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

/**
 * @brief Dealloc tree
 *
 * Recursively free each node and each leaf.
 *
 * @param Pointer on the node of the built tree
 *
 * @return 0 if pointer is NULL
 *
 */
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

/**
 * @brief Print tree
 *
 * Optionally print tree.
 *
 * @param Pointer on the node of the built tree
 *
 */

void print_tree(node *root_t)
{
	if (root_t == NULL) {
		return;
	}

	print_tree(root_t->left);
	print_tree(root_t->right);
	printf("%d - %li\n", root_t->symb, root_t->count);
}
