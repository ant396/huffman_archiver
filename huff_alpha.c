#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS 256

typedef struct tree_node {
    char ch;
    int count;
    struct tree_node *left;
    struct tree_node *right;
} node;

char stat[MAXCHARS] = {0};
node *queue[MAXCHARS];
int leaf_qty = 0;
char *huff_code[MAXCHARS];

void take_stat(FILE *fp) 
{
    int c;
    while ((c = fgetc(fp)) != EOF) 
        ++stat[c];
}


int comp(const void *p1, const void *p2)
{
    struct tree_node *p = *(struct tree_node *const *) p1;
    struct tree_node *q = *(struct tree_node *const *) p2;
    return (p->count - q->count);
}


void make_node(char ch, int count, node *a, node *b)
{
    node *new_node;
    int c;
    int n;

    if (count) {
        queue[leaf_qty] = (node *) malloc(sizeof(node));
        queue[leaf_qty]->ch = ch;
        queue[leaf_qty]->count = count;
    } else {
        new_node = malloc(sizeof(node));
        new_node->count = a->count + b->count;
        new_node->left = a;
        new_node->right = b;
        
        /*insert in the queue*/
        for (c=0; c < leaf_qty; c++) {
            if (leaf_qty >= 1 && new_node->count < queue[c]->count) {
                for (n = leaf_qty; n > c; n--)
                    queue[n] = queue[n-1];
                queue[c] = new_node;
                leaf_qty++;
                break;
            } else if (leaf_qty >= 1 && queue[leaf_qty-1]->count <= new_node->count) { 
                queue[leaf_qty++] = new_node;
                break;
            }
        }

        if (leaf_qty == 0)
            queue[leaf_qty++] = new_node;
    }
}


node *del_node()
{
    node *first = queue[0];
    int n;

    for (n=1; n < leaf_qty; n++) 
        queue[n-1] = queue[n];
    
    --leaf_qty;
    return first;
}

void init(FILE *fp)
{
    int c;
    
    take_stat(fp);

    for (c=0; c < MAXCHARS; c++)
        if (stat[c] > 0) {
            make_node(c,stat[c], NULL, NULL);
            leaf_qty++;
        }
    
    qsort(queue, leaf_qty, sizeof(node *), comp);
    
    while (leaf_qty >= 2) 
        make_node(0, 0, del_node(), del_node());
}


void huff_coding(node *p, char *code)
{
    
    if (p->ch) {
        huff_code[p->ch] = malloc(sizeof(code));
        strcpy(huff_code[p->ch], code);
        return;
    }
    
    char left[MAXCHARS];
    strcpy(left, code);
    strcat(left, "0");
    huff_coding(p->left, left);

    char right[MAXCHARS];
    strcpy(right, code);
    strcat(right, "1");
    huff_coding(p->right, right);
}

void print_tree(node *p)
{
    if (p != NULL) {
        print_tree(p->left);
        if (p->ch)
            printf("%c: %d\n", p->ch, p->count);
        print_tree(p->right);
    }
}

void encode(char *arg, char **code)
{   
    FILE *fp = fopen(arg, "r");
    FILE *new_file;
    char file_name[MAXCHARS];
    int c;

    strcpy(file_name, arg);
    strcat(file_name, ".huff");

    new_file = fopen(file_name, "aw");

    while ((c = getc(fp)) != EOF) 
        fputs(huff_code[c], new_file);

    fclose(new_file), fclose(fp);
}

void decode(char *name)
{
    FILE *fp = fopen(name, "r");
    FILE *new_file = fopen("unarch", "aw");
    int c;
    node *temp = queue[0];

    while ((c = getc(fp)) != EOF) {
        if (c == '0') temp = temp->left;
        else temp = temp->right;

        if (temp->ch) {
            putc(temp->ch, new_file);
            temp = queue[0];
        }
    }
}
            
                        

int main(int argc, char **argv)
{
    FILE *fp;
    char code[] = "";

    if (argc < 2) {
        fprintf(stderr, "%s: no input file.\n", argv[0]);
        exit(1);
    } else 
        fp = fopen(*++argv, "r");

    init(fp); 
    /*for (leaf_qty; leaf_qty >= 0; leaf_qty--) {
        free(queue[leaf_qty]);
        queue[leaf_qty] = NULL;
    }*/
    huff_coding(queue[0], code);
    encode(*argv, huff_code);

    char *temp = "test.huff";
    decode(temp);
    
    fclose(fp);
    exit(0);
}
