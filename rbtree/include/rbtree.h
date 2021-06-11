#ifndef RBTREE_INCLUDE_RBTREE_H_
#define RBTREE_INCLUDE_RBTREE_H_

#include <inttypes.h>

#include <stdlib.h>
#include <stdio.h>

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

uint8_t create_tree(struct RBTree*);
uint8_t _create_node(void*, void *);

uint8_t free_tree(struct RBTree*);
uint8_t _free_node(struct Node*);

uint8_t insert_node(struct RBTree*, const void*, const void*);
uint8_t delete_node(struct RBTree*, const void*);
uint8_t search_node(struct RBTree*, const void*);

uint8_t preorder_traversel(struct RBTree*, uint8_t (*)(struct Node*));
uint8_t postorder_traversel(struct RBTree*, uint8_t (*)(struct Node*));
uint8_t inorder_traversel(struct RBTree*, uint8_t (*)(struct Node*));

#endif  // RBTREE_INCLUDE_RBTREE_H_
