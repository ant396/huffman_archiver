#ifndef TREE
#define TREE

#define MAXCHARS 256
#define BUFF_SIZE 4096

typedef struct tree_node {
	char symb;
	int count;
	struct tree_node *left;
	struct tree_node *right;
} node;

typedef struct {
	node **root;
	int leaf_qty;
	int *stat;
} tree;

node *add_leaf(char symb, int count);
int add_node(tree *huff_tree);
int comp(const void *a, const void *b);
int dealloc_tree(node *pointer);

#endif //TREE
