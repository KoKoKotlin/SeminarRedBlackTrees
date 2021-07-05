#include "include/binary.h"


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


void _free_bin_node(struct BinNode *node)
{
    free(node->key);
    free(node);
}

void free_bin_tree(struct BinTree *tree) {
    postorder_traversel_bin(tree, &_free_bin_node);
    free(tree);
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
        current = (*key < *current->key) ? current->left : current->right;
    }

    debug_printf("Needed %zu comparisons!", count);

    if (*key < *previous->key) previous->left = new_node;
    else previous->right = new_node;

    new_node->parent = previous;

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

void postorder_traversel_bin(struct BinTree *tree, void (*action)(struct BinNode*))
{

    if (tree->root == NULL) return;

    struct BinNode *last = NULL;
    struct BinNode *current = tree->root;
    struct Stack *stack = create_stack((size_t)log2((double)tree->node_count) + 1);

    while (current != NULL || !is_stack_empty(stack)) {
        if (current != NULL) {
            push(stack, current);
            current = current->left;
        } else {
            struct BinNode *stack_node;
            peek(stack, &stack_node);

            if (stack_node->right != NULL && last != stack_node->right) {
                current = stack_node->right;
            } else {
                action(stack_node);
                pop(stack, &last);
            }
        }
    }

    free_stack(stack);
}