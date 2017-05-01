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

int dealloc_tree(node *pointer);

#endif //TREE
