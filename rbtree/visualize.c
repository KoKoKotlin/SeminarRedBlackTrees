#include <math.h>

#include "include/visualize.h"
#include "include/rbtree.h"

#define COUNT 7
#define COLOR_RED   "\x1b[1;31m"
#define COLOR_BLUE  "\x1b[1;34m"
#define COLOR_PINK  "\x1b[1;35m"
#define COLOR_WHITE "\x1b[1;37m"

size_t find_tree_height(struct Node *current, size_t h)
{
    if (current == NULL) return h;

    size_t left = find_tree_height(current->left, h + 1);
    size_t right = find_tree_height(current->right, h + 1);

    return (left > right) ? left : right;
}

// source: https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void printTree(struct Node *root, int space)
{
    if (root == NULL) {
        return;
    }

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    printTree(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");

    if (root->color == RB_TREE_BLACK) printf(COLOR_BLUE T_FORMAT " ", *(root->key));
    else printf(COLOR_RED T_FORMAT " ", *(root->key));

    if (root->parent != NULL) printf(COLOR_PINK T_FORMAT "\n", *(root->parent->key));
    else printf("\n");

    // Process left child
    printTree(root->left, space);
}

void ascii_art_tree(struct RBTree *rbtree)
{
    size_t height = find_tree_height(rbtree->root, 0);
    printf("Tree height: %zu\n", height);

    printTree(rbtree->root, 0);
    printf(COLOR_WHITE);
}