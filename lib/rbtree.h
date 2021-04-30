#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <inttypes.h>

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

#define RB_TREE_TRUE        1U
#define RB_TREE_FALSE       0U

#define RB_TREE_RED         1U
#define RB_TREE_BLACK       0U

struct Node;

struct Node {
    void *data;

    void *key;

    struct Node *left;
    struct Node *right;

    uint8_t color;
};

struct RBTree {
    struct Node *root;
};

uint8_t create_tree(struct RBTree *tree);
uint8_t _create_node(void *key, void *data);

uint8_t free_tree(struct RBTree *tree);
uint8_t _free_node(struct Node *node);

uint8_t insert_node(struct RBTree *tree, const void *key, const void *data);
uint8_t delete_node(struct RBTree *tree, const void *key);
uint8_t search_node(struct RBTree, const void *key);

uint8_t preorder_traversel(struct RBTree *tree, uint8_t (*action)(struct Node *node));
uint8_t postorder_traversel(struct RBTree *tree, uint8_t (*action)(struct Node *node));
uint8_t inorder_traversel(struct RBTree *tree, uint8_t (*action)(struct Node *node));

#endif  /* _RB_TREE_H_ */