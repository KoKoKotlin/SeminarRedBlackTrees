#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "stack.h"

struct BinNode;

struct BinNode {
    int *key;

    struct BinNode *left;
    struct BinNode *right;
    struct BinNode *parent;
};

struct BinTree {
    struct BinNode *root;

    size_t node_count;
};

struct BinTree *create_bin_tree();
void free_bin_tree(struct BinTree*);

void insert_bin_node(struct BinTree*, int*);
struct BinNode *search_bin(struct BinTree*, int*);

void postorder_traversel_bin(struct BinTree*, void (*)(struct BinNode*));

#endif