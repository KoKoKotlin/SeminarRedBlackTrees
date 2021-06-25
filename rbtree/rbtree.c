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
    new_node->parent = NULL;
    new_node->color = RB_TREE_RED;

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

struct Node* get_grandparent(struct Node *start)
{
    if (start->parent != NULL) return start->parent->parent;
    else return NULL;
}

uint8_t get_direction(struct Node *start_node) {
    struct Node *parent = start_node->parent;
    if (parent == NULL) return RB_TREE_NULL_ERROR;

    return (parent->left == start_node) ? RB_TREE_LEFT_CHILD : RB_TREE_RIGHT_CHILD;
}

struct Node* get_uncle(struct Node *start)
{
    struct Node *parent = start->parent;
    if (parent == NULL) {
        return NULL;
    }

    struct Node *grandparent = parent->parent;
    if (grandparent == NULL) {
        return NULL;
    }

    if (grandparent->left == parent) return grandparent->right;
    else return grandparent->left;
}

void color_flip(struct Node *start_node)
{
    // color flip parent, uncle, and grandparent
    struct Node *parent = start_node->parent;
    struct Node *grandparent = get_grandparent(start_node);
    struct Node *uncle = get_uncle(start_node);

    if (parent != NULL) parent->color = !parent->color;
    if (grandparent != NULL) grandparent->color = !grandparent->color;
    if (uncle != NULL) uncle->color = !uncle->color;

    debug_printf("Did color flip at node " T_FORMAT ".", *(start_node->key));
}

void rotate(struct Node *start_node, uint8_t direction, struct RBTree *rbtree)
{
    if (direction == LEFT_ROTATE) {
        struct Node *child = start_node->right;
        if (start_node == rbtree->root) rbtree->root = child;
        child->parent = start_node->parent;

        if (start_node->parent != NULL) {
            if (start_node->parent->left == start_node) start_node->parent->left = child;
            else start_node->parent->right = child;
        }
        
        start_node->right = child->left;
        child->left = start_node;
        start_node->parent = child;
        debug_printf("Did left rotate at node " T_FORMAT ".", *(start_node->key));
    } else if (direction == RIGHT_ROTATE) { 
        struct Node *child = start_node->left;
        if (start_node == rbtree->root) rbtree->root = child;
        child->parent = start_node->parent;
        
        if (start_node->parent != NULL) {
            if (start_node->parent->left == start_node) start_node->parent->left = child;
            else start_node->parent->right = child;
        }

        start_node->left = child->right;
        child->right = start_node;
        start_node->parent = child;
        debug_printf("Did right rotate at node " T_FORMAT ".", *(start_node->key));
    }

    if (rbtree->root->color == RB_TREE_RED) rbtree->root->color = RB_TREE_BLACK;
}

void fix_tree_insert(struct Node *start_node, struct RBTree *rbtree) {

    struct Node *current = start_node;
    struct Node *parent = start_node->parent;
    while (parent != NULL && parent->color == RB_TREE_RED && current->color == RB_TREE_RED) {
        if (parent->parent == NULL) break; // special case for root node

        uint8_t parent_direction = get_direction(parent);
        struct Node *uncle = get_uncle(current);

        if (uncle == NULL || uncle->color == RB_TREE_BLACK) { // rotate
            if ((parent_direction == RB_TREE_RIGHT_CHILD && parent->left  == current)
             || (parent_direction == RB_TREE_LEFT_CHILD  && parent->right == current)) {
                rotate(parent, parent_direction, rbtree);
                current = parent;
                parent = current->parent;
            } else {
                struct Node *grandparent = get_grandparent(current);
                rotate(grandparent, !parent_direction, rbtree);
                parent->color = RB_TREE_BLACK;
                grandparent->color = RB_TREE_RED;
                break;
            }
        }

        if (uncle->color == RB_TREE_RED) {  // color flip: P + U == RED
            color_flip(current);

            current = get_grandparent(current);
            parent = current->parent;
            debug_print("Red uncle -> colorflip and move to grandparent. Continuing");
        }
    }

    // re-color the root
    rbtree->root->color = RB_TREE_BLACK;
}

void fix_tree_delete(struct Node *start_node) {

}

uint8_t insert_node(struct RBTree *rbtree, T *key, void *value)
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    if (key == NULL) {
        debug_print("Given key was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node *new_node = _create_node(key, value);

    if (new_node == NULL) {
        debug_print("new_node was NULL! Aborting.");
        return RB_TREE_OUT_OF_MEM;
    }

    if (rbtree->root == NULL) {
        rbtree->root = new_node;
        rbtree->root->color = RB_TREE_BLACK;

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
        new_node->parent = previous;

        debug_printf("Inserting " T_FORMAT " with parent " T_FORMAT ".", *key, *(previous->key));
    }

    fix_tree_insert(new_node, rbtree);

    rbtree->node_count++;
    return RB_TREE_SUCCESS;
}

#if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_OVERRIDE_EXTERN
uint8_t override_value(struct RBTree *rbtree, T *key, void *value)
{
    struct Node *node = NULL;

    uint8_t error = search_node(rbtree, key, &node);
    if (error == RB_TREE_KEY_ERROR) return RB_TREE_KEY_ERROR;

    debug_printf("Overriding node with key: " T_FORMAT ".", *key);
    free(node->value);
    node->value = value;

    return RB_TREE_SUCCESS;
}
#endif

void get_next_smallest(struct Node *start, struct Node **next_smallest)
{
    struct Node *current  = start;

    while (current->right != NULL) {
        current = current->right;
    }

    *next_smallest = current;
}

void get_next_largest(struct Node *start, struct Node **next_smallest)
{
    struct Node *current  = start;

    while (current->left != NULL) {
        current = current->left;
    }

    *next_smallest = current;
}

// TODO: test this function again, bc of new parent pointer
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

    struct Node *node_to_delete = NULL;
    search_node(rbtree, key, &node_to_delete);

    if (node_to_delete == NULL) {
        debug_print("There is no node with the given key!");
        return RB_TREE_KEY_ERROR;
    }

    struct Node *parent_node = node_to_delete->parent;

    if (node_to_delete->left != NULL) {
        struct Node *next_smallest =  NULL;
        get_next_smallest(node_to_delete->left, &next_smallest);

        debug_printf("Deleting node " T_FORMAT " and replacing with the next smallest node " T_FORMAT ".", *(node_to_delete->key), *(next_smallest->key));

        free(node_to_delete->key);
        free(node_to_delete->value);

        node_to_delete->key = next_smallest->key;
        node_to_delete->value = next_smallest->value;

        if (next_smallest->parent == node_to_delete) next_smallest->parent->left = NULL;
        else next_smallest->parent->right = NULL;

        free(next_smallest);
    } else if (node_to_delete->right != NULL) {
        struct Node *next_largest =  NULL;
        get_next_largest(node_to_delete->right, &next_largest);

        debug_printf("Deleting node " T_FORMAT " and replacing with the next largest node " T_FORMAT ".", *(node_to_delete->key), *(next_largest->key));

        free(node_to_delete->key);
        free(node_to_delete->value);

        node_to_delete->key = next_largest->key;
        node_to_delete->value = next_largest->value;

        if (next_largest->parent == node_to_delete) next_largest->parent->right = NULL;
        else next_largest->parent->left = NULL;
        free(next_largest);
    } else {
        debug_printf("Deleting node " T_FORMAT " with no chidren.", *(node_to_delete->key));

        if (parent_node != NULL) {
            if (*(parent_node->key) < *(node_to_delete->key)) parent_node->right = NULL;
            else parent_node->left = NULL;
        }

        _free_node(node_to_delete);
    }

    fix_tree_delete(node_to_delete);

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
