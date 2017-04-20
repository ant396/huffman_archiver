#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define FILE_NAME 256
#define SIZE (int) sizeof(short)*8

void decode(char *arch_name, node *root)
{
	FILE *arch = fopen(arch_name, "r");
	FILE *new_file = fopen("extract", "w");

	short *temp = malloc(sizeof(short));

	node *tree = root;

	while (fread(temp, sizeof(short), 1, arch) > 0) {
		for (int n = SIZE; n > 0; n--) {
			if (*temp & (1 << (n-1))) {
				tree = tree->right;
			} else {
				tree = tree->left;
			}
			
			if ((int) tree->ch == 3) {
				break;
			}

			if (tree->ch) {
				putc(tree->ch, new_file);
				tree = root;
			}
		}
	}
	free(temp);
	fclose(arch), fclose(new_file);
}
