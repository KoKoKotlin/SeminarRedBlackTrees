#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "stack.h"

#define BIN_TREE_LEFT_CHILD  0U 
#define BIN_TREE_RIGHT_CHILD 1U

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
void delete_bin_node(struct BinTree* tree, int* key);

void postorder_traversel_bin(struct BinTree*, void (*)(struct BinNode*));

#endif