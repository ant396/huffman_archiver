/**
 * @file	Huff_code
 * @brief	Make codes
 *
 * Make codes based on built tree.
 * And uses only for encoding.
 * @author	 Anton Svechnikov
 * @license	 BSD
 *
 */


#include "global.h"
#include <string.h>
#include <alloca.h>

/**
 * @brief Make codes
 *
 * Make codes based on built tree.
 *
 * @param root Pointer the node of the tree
 * @param code Current huffman code
 * @param huff_code Array with all codes
 *
 */
void huff_coding(node *root, char *code, char *huff_code[])
{
	if (root == NULL) {
		return;
	}
	
	
	char *left = (char *) alloca(sizeof(char) * MAXCHARS);
	stpcpy(stpcpy(left, code), "0");
    huff_coding(root->left, left, huff_code);
	
	char *right = (char *) alloca(sizeof(char) * MAXCHARS);
	stpcpy(stpcpy(right, code), "1");
    huff_coding(root->right, right, huff_code);

	if (root->symb) {
		char *huff_code_p = (char *) malloc(strlen(code) * sizeof(char) + 1);
        strcpy(huff_code_p, code);
		huff_code[(int) root->symb] = huff_code_p;
        return;
    }
}
