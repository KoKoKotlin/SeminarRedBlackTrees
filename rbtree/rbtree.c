#include <stdlib.h>
#include <math.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"

struct RBTree* create_tree()
{
    struct RBTree *rbtree = (struct RBTree*)malloc(sizeof(struct RBTree));

    if (rbtree == NULL) {
        debug_print("Couldn't allocate memory for rbtree!");
        return NULL;
    }

    rbtree->root = NULL;
    rbtree->node_count = 0;

    debug_print("Created new RBTree.");

    return rbtree;
}

void free_tree(struct RBTree *rbtree)
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return;
    }

    postorder_traversel(rbtree, &_free_node);
    free(rbtree);

    debug_print("Freed the rbtree.");
}

struct Node* _create_node(T *key, void *value)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));

    if (new_node == NULL) {
        debug_print("Can't allocate memory for new node!");
        return NULL;
    }

    debug_printf("Creating new node with key " T_FORMAT ".", *key);

    new_node->key   = key;
    new_node->value = value;

    return new_node;
}

void _free_node(struct Node *node)
{
    debug_printf("Freeing node with key " T_FORMAT ".", *(node->key));

    // key and value must be heap allocated
    free(node->key);
    free(node->value);

    free(node);
}

uint8_t insert_node(struct RBTree* rbtree, T* key, void* value)
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    if (key == NULL) {
        debug_print("Given key was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    // TODO: implement beahvior for same key or forbid same key
    struct Node *new_node = _create_node(key, value);

    if (new_node == NULL) {
        debug_print("new_node was NULL! Aborting.");
        return RB_TREE_OUT_OF_MEM;
    }

    if (rbtree->root == NULL) {
        rbtree->root = new_node;

        debug_printf("Creating new root node with key " T_FORMAT ".", *key);
    } else {
        struct Node *previous = NULL;
        struct Node *current  = rbtree->root;

        #ifdef DEBUG
            size_t count = 0;
        #endif

        while (current != NULL) {
            #ifdef DEBUG
                count++;
            #endif

            #if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_FORBID || RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_ALLOW_EXTERN
                if (*(current->key) == *key) {
                    debug_printf("Key " T_FORMAT " already exists. Aborting...", *key);
                    return RB_TREE_DUPLICATE_KEY_ERROR;
                }
            #elif RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_OVERRIDE
                if (*(current->key) == *key) {
                    debug_printf("Key " T_FORMAT " already exists. Overriding value.", *key);
                    free(current->value);
                    current->value = value;
                    return RB_TREE_SUCCESS;
                }
            #endif

            previous = current;
            current  = (*key < *(current->key)) ? previous->left : previous->right;
        }

        debug_printf("Needed to search %zu nodes!", count);

        if (*key < *(previous->key)) previous->left = new_node;
        else previous->right = new_node;
        debug_printf("Inserting " T_FORMAT " with parent " T_FORMAT ".", *key, *(previous->key));
    }

    rbtree->node_count++;
    return RB_TREE_SUCCESS;
}

#if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_OVERRIDE_EXTERN
uint8_t override_value(struct RBTree *rbtree, T *key, void *value) {
    struct Node *node = NULL;

    uint8_t error = search_node(rbtree, key, &node);
    if (error == RB_TREE_KEY_ERROR) return RB_TREE_KEY_ERROR;

    debug_printf("Overriding node with key: " T_FORMAT ".", *key);
    free(node->value);
    node->value = value;

    return RB_TREE_SUCCESS;
}
#endif

void get_next_smallest(struct Node *start, struct Node **next_smallest, struct Node **parent) {
    struct Node *previous = *parent;
    struct Node *current  = start;

    while (current->right != NULL) {
        previous = current;
        current = current->right;
    }

    *next_smallest = current;
    *parent = previous;
}

void get_next_largest(struct Node *start, struct Node **next_smallest, struct Node **parent) {
    struct Node *previous = *parent;
    struct Node *current  = start;

    while (current->left != NULL) {
        previous = current;
        current = current->left;
    }

    *next_smallest = current;
    *parent = previous;
}


uint8_t delete_node(struct RBTree* rbtree, T* key)
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    if (key == NULL) {
        debug_print("Given key was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *parent_node = NULL;
    struct Node *node_to_delete = NULL;

    struct Node *previous = NULL;
    struct Node *current  = rbtree->root;

    while (current != NULL) {
        if (*(current->key) == *key) break;
        previous = current;
        current = (*(previous->key) < *key) ? previous->right : previous->left;
    }

    parent_node = previous;
    node_to_delete = current;

    if (node_to_delete == NULL) {
        debug_print("There is no node with the given key!");
        return RB_TREE_KEY_ERROR;
    }

    if (node_to_delete->left != NULL) {
        struct Node *next_smallest =  NULL;
        struct Node *next_smallest_parent = node_to_delete;
        get_next_smallest(node_to_delete->left, &next_smallest, &next_smallest_parent);

        debug_printf("Deleting node " T_FORMAT " and replacing with the next smallest node " T_FORMAT ".", *(node_to_delete->key), *(next_smallest->key));

        free(node_to_delete->key);
        free(node_to_delete->value);

        node_to_delete->key = next_smallest->key;
        node_to_delete->value = next_smallest->value;

        if (next_smallest_parent == node_to_delete) next_smallest_parent->left = NULL;
        else next_smallest_parent->right = NULL;

        free(next_smallest);
    } else if (node_to_delete->right != NULL) {
        struct Node *next_largest =  NULL;
        struct Node *next_largest_parent = node_to_delete;
        get_next_largest(node_to_delete->right, &next_largest, &next_largest_parent);

        debug_printf("Deleting node " T_FORMAT " and replacing with the next largest node " T_FORMAT ".", *(node_to_delete->key), *(next_largest->key));

        free(node_to_delete->key);
        free(node_to_delete->value);

        node_to_delete->key = next_largest->key;
        node_to_delete->value = next_largest->value;
        if (next_largest_parent == node_to_delete) next_largest_parent->right = NULL;
        else next_largest_parent->left = NULL;
        free(next_largest);
    } else {
        debug_printf("Deleting node " T_FORMAT " with no chidren.", *(node_to_delete->key));

        if (parent_node != NULL) {
            if (*(parent_node->key) < *(node_to_delete->key)) parent_node->right = NULL;
            else parent_node->left = NULL;
        }

        _free_node(node_to_delete);
    }

    rbtree->node_count--;
    return RB_TREE_SUCCESS;
}

uint8_t search_node(struct RBTree* rbtree, T* key, struct Node **node)
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    if (key == NULL) {
        debug_print("Given search key was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *current  = rbtree->root;

    #ifdef DEBUG
        size_t count = 0;
    #endif

    while (current != NULL) {
        #ifdef DEBUG
            count++;
        #endif

        if (*(current->key) == *(key)) break;
        current = (*key < *(current->key)) ? current->left : current->right;
    }

    if (current == NULL) {
        debug_print("Key not found!");

        *node = NULL;
        return RB_TREE_KEY_ERROR;
    }

    debug_printf("Found node for key " T_FORMAT " and had to check %zu nodes.", *key, count);
    *node = current;

    return RB_TREE_SUCCESS;
}

uint8_t preorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{
    // root - left - right

    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *current = NULL;
    struct Stack *stack = create_stack((size_t)log2((double)rbtree->node_count) + 1);
    push(stack, rbtree->root);

    debug_printf("Starting preorder traversel with tree with root key " T_FORMAT ".", *(rbtree->root->key));

    while (!is_stack_empty(stack)) {
        pop(stack, &current);
        action(current);

        if (current->right != NULL)
            push(stack, current->right);

        if (current->left != NULL)
            push(stack, current->left);
    }

    free_stack(stack);

    return RB_TREE_SUCCESS;
}

uint8_t postorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{
    // left - right - root
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *last = NULL;
    struct Node *current = rbtree->root;
    struct Stack *stack = create_stack((size_t)log2((double)rbtree->node_count) + 1);

    debug_printf("Starting postorder traversel with tree with root key " T_FORMAT ".", *(rbtree->root->key));

    while (current != NULL || !is_stack_empty(stack)) {
        if (current != NULL) {
            push(stack, current);
            current = current->left;
        } else {
            struct Node *stack_node;
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

    return RB_TREE_SUCCESS;
}

uint8_t inorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{
    // left - root - right

    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *current = rbtree->root;
    struct Stack *stack = create_stack((size_t)log2((double)rbtree->node_count) + 1);

    debug_printf("Starting inorder traversel with tree with root key " T_FORMAT ".", *(rbtree->root->key));

    while (current != NULL || !is_stack_empty(stack)) {
        if (current != NULL) {
            push(stack, current);
            current = current->left;
        } else {
            struct Node *stack_node;
            pop(stack, &stack_node);
            action(stack_node);
            current = stack_node->right;
        }
    }

    free_stack(stack);

    return RB_TREE_SUCCESS;
}
