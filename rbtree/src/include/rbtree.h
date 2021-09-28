#ifndef RBTREE_INCLUDE_RBTREE_POINTER_H_
#define RBTREE_INCLUDE_RBTREE_POINTER_H_

#include <inttypes.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define T int
#define TLESS(x, y) ((x) < (y))
#define TEQUAL(x, y) ((x) == (y))
#define T_FORMAT "%d"


#define RB_TREE_TRUE    1U
#define RB_TREE_FALSE   0U

#define RB_TREE_RED     1U
#define RB_TREE_BLACK   0U

#define LEFT_ROTATE  0U
#define RIGHT_ROTATE 1U

#define RB_TREE_LEFT_CHILD  0U
#define RB_TREE_RIGHT_CHILD 1U

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
    struct Node *parent;    // simplifies a lot of the algorithms

    uint8_t color;
};

struct RBTree {
    struct Node *root;

    size_t node_count;
};

struct RBTree* create_tree();
struct Node* _create_node(T* key, void* value);

void free_tree(struct RBTree*);

uint8_t insert_node(struct RBTree* rbtree, T* key, void* value);
uint8_t delete_node(struct RBTree* rbtree, T* key);
uint8_t search_node(struct RBTree* rbtree, T* key, struct Node** node);

#if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_ALLOW_EXTERN
uint8_t override_value(struct RBTree* rbtree, T* key, void* value);
#endif

uint8_t preorder_traversel(struct RBTree* rbtree,  void (*action)(struct Node*));
uint8_t postorder_traversel(struct RBTree* rbtree, void (*action)(struct Node*));
uint8_t inorder_traversel(struct RBTree* rbtree,   void (*action)(struct Node*));

#endif  // RBTREE_INCLUDE_RBTREE_POINTER_H_
