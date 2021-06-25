#include <math.h>

#include "include/visualize.h"
#include "include/rbtree.h"

// source: https://www.techiedelight.com/level-order-traversal-binary-tree/
uint8_t printLevel(struct Node *root, size_t level, size_t height)
{
    if (root == NULL) {
        printf("  ");
        return 0;
    }

    if (level == 1) {
        if (root->color == RB_TREE_RED) printf("\033[1;31m" T_FORMAT " ", *(root->key));
        else printf("\033[1;34m" T_FORMAT " ", *(root->key));
        return 1;
    }

    uint8_t left = printLevel(root->left, level - 1, height);
    uint8_t right = printLevel(root->right, level - 1, height);
    return left + right;
}

void ascii_art_tree(struct RBTree *rbtree)
{
    size_t level = 1;
    size_t height = (size_t)ceil(log2(rbtree->node_count));
    while (printLevel(rbtree->root, level, height)) {
        level++;
        printf("\n");
    }
}