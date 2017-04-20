#pragma once

#ifndef FUNCS
#define FUNCS

#include <stdio.h>
#define MAXCHARS 256
#define BUFF_SIZE 4 //4096


//tree
typedef struct tree_node {
	char ch;
	int count;
	struct tree_node *left;
	struct tree_node *right;
} node;

node *queue[MAXCHARS];

void add_node(char, int, node *, node *);
node *del_node();
int dealloc_tree(node *);

//standart return value
enum retrun_opt {
	ok = 0,
	error,
};


//shell interface
typedef struct arch_interface {
	char *opt;
	char *ifile;
	char *ofile;
	FILE *ifile_p;
	FILE *ofile_p;
} interface;

interface *input_opt(int, char **);


//huffman coding
int compress(interface *, int *, char **);
int encode(interface *, char **);
void decode(char *, node *);


int close_func(node *, interface *, int);

#endif
