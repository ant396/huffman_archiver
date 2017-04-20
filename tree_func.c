#include "main.h"
#include <stdlib.h>

extern int leaf_qty;

void add_node(char ch, int count, node *a, node *b)
{
    node *new_node;
    int n;

    if (count) {
        queue[leaf_qty] = malloc(sizeof(node));
        queue[leaf_qty]->ch = ch;
        queue[leaf_qty]->count = count;
    } else {
        new_node = malloc(sizeof(node));
        new_node->count = a->count + b->count;
        new_node->left = a;
        new_node->right = b;

        //insert in the queue
        for (int index = 0; index < leaf_qty; index++) {
            if (leaf_qty >= 1 && new_node->count < queue[index]->count) {
                for (n = leaf_qty; n > index; n--) {
                    queue[n] = queue[n-1];
				}
                queue[index] = new_node;
                leaf_qty++;
                break;
            } else if (leaf_qty >= 1 && \
			queue[leaf_qty-1]->count <= new_node->count) {
                queue[leaf_qty++] = new_node;
                break;
            }
        }

        if (leaf_qty == 0) {
            queue[leaf_qty++] = new_node;
		}
    }
}


node *del_node() //delete node only from queue
{
    node *first = queue[0];

    for (int index = 1; index < leaf_qty; index++) {
        queue[index-1] = queue[index];
	}

    --leaf_qty;
    return first;
}


int dealloc_tree(node *pointer)
{
	if (pointer == NULL) {
		return ok;
	}

	dealloc_tree(pointer->left);
	dealloc_tree(pointer->right);

	free(pointer);
	pointer = NULL;

	return ok;
}
