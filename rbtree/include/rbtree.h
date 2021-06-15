#ifndef RBTREE_INCLUDE_RBTREE_H_
#define RBTREE_INCLUDE_RBTREE_H_

#include <inttypes.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define T int
#define T_FORMAT "%d"

#define RB_TREE_TRUE    1U
#define RB_TREE_FALSE   0U

#define RB_TREE_RED     1U
#define RB_TREE_BLACK   0U

#define RB_TREE_SUCCESS             0U
#define RB_TREE_OUT_OF_MEM          1U
#define RB_TREE_KEY_ERROR           2U
#define RB_TREE_NULL_ERROR          3U
#define RB_TREE_DUPLICATE_KEY_ERROR 4U

#define RB_TREE_DUPLICATE_KEYS RB_TREE_DUPLICATE_KEYS_ALLOW
#define RB_TREE_DUPLICATE_KEYS_ALLOW        0U
#define RB_TREE_DUPLICATE_FORBID            1U
#define RB_TREE_DUPLICATE_OVERRIDE          2U
#define RB_TREE_DUPLICATE_OVERRIDE_EXTERN   3U

struct Node;

struct Node {
    void *value;

    T *key;

    struct Node *left;
    struct Node *right;

    uint8_t color;
};

struct RBTree {
    struct Node *root;

    size_t node_count;
};

struct RBTree* create_tree();
struct Node* _create_node(T*, void*);

void free_tree(struct RBTree*);
void _free_node(struct Node*);

uint8_t insert_node(struct RBTree*, T*, void*);
uint8_t delete_node(struct RBTree*, T*);
uint8_t search_node(struct RBTree*, T*, struct Node**);

#if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_ALLOW_EXTERN
uint8_t override_value(struct RBTree*, T*, void*);
#endif

uint8_t preorder_traversel(struct RBTree*,  void (*)(struct Node*));
uint8_t postorder_traversel(struct RBTree*, void (*)(struct Node*));
uint8_t inorder_traversel(struct RBTree*,   void (*)(struct Node*));

#endif  // RBTREE_INCLUDE_RBTREE_H_
