#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

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

struct BinTree *create_bin_tree()
{
    struct BinTree *binTree = (struct BinTree*)malloc(sizeof(struct BinTree));

    binTree->root = NULL;
    binTree->node_count = 0;

    return binTree;
}

struct BinNode *create_bin_node(int *key)
{
    struct BinNode *new_node = (struct BinNode*)malloc(sizeof(struct BinNode));

    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;
}

void free_bin_tree(struct BinTree *tree) {

}

void insert_bin_node(struct BinTree *tree, int *key)
{
    struct BinNode *new_node = create_bin_node(key);

    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }

    struct BinNode *previous = NULL;
    struct BinNode *current = tree->root;

    #ifdef DEBUG
        size_t count = 0;
    #endif

    while (current != NULL) {

        #ifdef DEBUG
            count++;
        #endif

        previous = current;
        current = (*current->key < *key) ? current->left : current->right;
    }

    debug_printf("Needed %$zu comparisons!", count);

    if (*key < *previous->key) previous->left = new_node;
    else previous->right = new_node;

    tree->node_count++;
}

struct BinNode *search_bin(struct BinTree *tree, int *key)
{
    struct BinNode *current = tree->root;

    while (current != NULL) {
        if (*current->key == *key) return current;
        else current = (*key < *current->key) ? current->left : current->right;
    }

    return NULL;
}

#endif