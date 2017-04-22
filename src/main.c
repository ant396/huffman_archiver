#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"



int stat[MAXCHARS] = {0};
char *huff_code[MAXCHARS] = {0};



node *queue[MAXCHARS] = {0};
int leaf_qty = 0;




void take_stat(FILE *ifile)
{
	char buff[BUFF_SIZE] = {0};
	size_t count;

    while ((count = fread(buff, sizeof(char), BUFF_SIZE, ifile))) {
		for (int index = 0; index < count; index++) {
			++stat[(int) *(buff+index)];
		}
	}

	if (feof(ifile)) {
		++stat[3];							//marker for end of file
	}
}


int comp(const void *p1, const void *p2)	//func for qsort
{
    node *p = *(node * const *) p1;
    node *q = *(node * const *) p2;
    return (p->count - q->count);
}


void build_tree(FILE *ifile)
{
	take_stat(ifile);
    for (int index = 0; index < MAXCHARS; index++) {
        if (stat[index] > 0) {
            add_node(index, stat[index], NULL, NULL);
            leaf_qty++;
        }
	}

	qsort(queue, leaf_qty, sizeof(node*), comp);

	while (leaf_qty >= 2) {
        add_node(0, 0, del_node(), del_node());
	}
}


void huff_coding(node *root, char *code)
{
    if (root->ch) {
        //huff_code[(int) root->ch] = malloc(sizeof(code));
        strcpy(huff_code[(int) root->ch], code);
        return;
    }

    char left[MAXCHARS];
    strcpy(left, code);
    strcat(left, "0");
    huff_coding(root->left, left);

    char right[MAXCHARS];
    strcpy(right, code);
    strcat(right, "1");
    huff_coding(root->right, right);
}


int main(int argc, char **argv)
{
    char code[] = "";
	int check;

	interface *current_session = input_opt(argc, argv);

	if (strcmp(current_session->opt, "-c") == 0) {                  //compress
		build_tree(current_session->ifile_p);
		huff_coding(queue[0], code);
		check = compress(current_session, stat, huff_code);

		if (check == 0) {
			/*for (int index = 0; index < MAXCHARS; index++) {
				free(huff_code[index]);
				huff_code[index] = NULL;
			}*/

			close_func(queue[0], current_session, check);
		}
	}
}


int close_func(node *root, interface *session, int status)
{
	//dealloc_tree(root);
	fclose(session->ifile_p);
	fclose(session->ofile_p);
	free(session);
	exit(status);
}
