#ifndef TREE
#define TREE

#define MAXCHARS 256
#define BUFF_SIZE 4096

typedef struct tree_node {
	unsigned char symb;
	struct tree_node *left;
	struct tree_node *right;
	unsigned long count;
} node;

typedef struct {
	int leaf_qty;
	node **root;
	unsigned long *stat;
} tree;

node *add_leaf(unsigned char symb, unsigned long count);
int add_node(tree *huff_tree);
int comp(const void *a, const void *b);
int dealloc_tree(node *pointer);

#endif //TREE
