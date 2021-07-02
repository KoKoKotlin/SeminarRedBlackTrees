#include <stdlib.h>
#include <math.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"

uint8_t get_direction(struct Node *start_node)
{
    // convention when deleting nodes
    struct Node *parent = start_node->parent;
    if (parent == NULL) return RB_TREE_NULL_ERROR;

    return (parent->left == start_node) ? RB_TREE_LEFT_CHILD : RB_TREE_RIGHT_CHILD;
}

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

struct Node* get_parent(struct Node *node)
{
    if (node == NULL) return NULL;
    return node->parent;
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
    debug_printf("Freeing node with key " T_FORMAT " key left: " T_FORMAT " key right: " T_FORMAT ".", *(node->key), (node->left != NULL) ? *(node->left->key) : 0xFFFF, (node->right != NULL) ? *(node->right->key) : 0xFFFF);

    // key and value must be heap allocated
    if (node->key != NULL) free(node->key);
    if (node->value != NULL) free(node->value);

    free(node);
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

struct Node* get_grandparent(struct Node *start)
{
    if (get_parent(start) != NULL) return get_parent(get_parent(start));
    else return NULL;
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

uint8_t get_color(struct Node *node)
{
    return (node == NULL || node->color == RB_TREE_BLACK) ? RB_TREE_BLACK : RB_TREE_RED;
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
        if (start_node->right) start_node->right->parent = start_node;

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
        if (start_node->left) start_node->left->parent = start_node;

        child->right = start_node;
        start_node->parent = child;
        debug_printf("Did right rotate at node " T_FORMAT ".", *(start_node->key));
    }

    if (rbtree->root->color == RB_TREE_RED) rbtree->root->color = RB_TREE_BLACK;
}

void fix_tree_insert(struct Node *start_node, struct RBTree *rbtree)
{
    struct Node *current = start_node;
    struct Node *parent = start_node->parent;

    while (parent != NULL && parent->color == RB_TREE_RED && current->color == RB_TREE_RED) {
        if (parent->parent == NULL) break;  // special case for root node

        uint8_t parent_direction = get_direction(parent);
        struct Node *uncle = get_uncle(current);

        if (get_color(uncle) == RB_TREE_BLACK) {  // rotate
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
        } else if (get_color(uncle) == RB_TREE_RED) {  // color flip: P + U == RED
            color_flip(current);

            current = get_grandparent(current);
            parent = current->parent;
            debug_print("Red uncle -> colorflip and move to grandparent. Continuing");
        }
    }

    // re-color the root
    rbtree->root->color = RB_TREE_BLACK;
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
        exit(RB_TREE_OUT_OF_MEM);
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

            #if RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_FORBID || RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_OVERRIDE_EXTERN
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

        if (*key < *(previous->key)) {
            previous->left = new_node;
        } else {
            previous->right = new_node;
        }
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

void get_next_largest(struct Node *start, struct Node **next_largest)
{
    struct Node *current  = start;

    while (current->left != NULL) {
        current = current->left;
    }

    *next_largest = current;
}

void get_next_smallest(struct Node *start, struct Node **next_smallest)
{
    struct Node *current  = start;

    while (current->right != NULL) {
        current = current->right;
    }

    *next_smallest = current;
}

void swap(void **v1, void **v2)
{
    void *temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

struct Node* get_sibling(struct Node *node)
{
    struct Node *parent = node->parent;
    if (parent == NULL) return NULL;

    return (get_direction(node) == RB_TREE_LEFT_CHILD) ? node->parent->right : node->parent->left;
}

struct Node* get_nephew(struct Node *node)
{
    if (get_sibling(node) == NULL) return NULL;

    if (get_direction(node) == RB_TREE_LEFT_CHILD) return get_sibling(node)->right;
    else return get_sibling(node)->left;
}

struct Node* get_niece(struct Node *node)
{
    if (get_sibling(node) == NULL) return NULL;

    if (get_direction(node) == RB_TREE_LEFT_CHILD) return get_sibling(node)->left;
    else return get_sibling(node)->right;
}

struct Node* swap_to_leaf(struct Node *node_to_delete)
{
    struct Node *leaf;

    if (node_to_delete->left != NULL) {
        struct Node *next_smallest =  NULL;
        get_next_smallest(node_to_delete->left, &next_smallest);
        leaf = next_smallest;

        swap((void**)&node_to_delete->key, (void**)&next_smallest->key);
        swap(&node_to_delete->value, &next_smallest->value);
    } else if (node_to_delete->right != NULL) {
        struct Node *next_largest =  NULL;
        get_next_largest(node_to_delete->right, &next_largest);
        leaf = next_largest;

        swap((void**)&node_to_delete->key, (void**)&next_largest->key);
        swap(&node_to_delete->value, &next_largest->value);
    } else {
        leaf = node_to_delete;
    }

    return leaf;
}

void fix_tree_delete(struct Node *x, struct RBTree *rbtree)
{
    // at this point we can assume that the node to remove x is a leaf or at least a half leaf
    while (1) {
        if (x->parent == NULL) {
            // when removing the root -> nothing to do bc the empty tree is a valid red black tree
            break;
        } else if (get_color(x) == RB_TREE_RED) {
            // when removing a red leaf -> nothing to do because the black height on the branch is unchanged
            break;
        } else if (get_color(get_sibling(x)) == RB_TREE_RED) {
            // case 1: the sibling of the node to remove is red
            debug_print("case 1");

            // color the parent red and color the sibling black
            x->parent->color = RB_TREE_RED;
            get_sibling(x)->color = RB_TREE_BLACK;

            // afterwards rotate the sibling into the position of the parent such that the black gets pushed up the tree
            rotate(x->parent, get_direction(x), rbtree);
        } else if (get_color(get_nephew(x)) == RB_TREE_RED) {
            // case 2: the sibling is black and the nephew is red
            debug_print("case 2");
            // change the siblings color to the parents color and color the parent black
            get_sibling(x)->color = x->parent->color;
            x->parent->color = RB_TREE_BLACK;

            // color the nephew black and rotate it into the parents position to push the black up the tree
            get_nephew(x)->color = RB_TREE_BLACK;
            rotate(x->parent, get_direction(x), rbtree);

            // after this case, the red black trees properties have been recovered and the node x can be removed safely
            break;
        } else if (get_color(get_niece(x)) == RB_TREE_RED) {
            // case 3: the sibling is black, the nephew is black and the niece is red
            debug_print("case 3");

            // color the niece black and the sibling red
            get_niece(x)->color = RB_TREE_BLACK;
            get_sibling(x)->color = RB_TREE_RED;

            // rotate the niece to the parents position to push up the black up the tree
            rotate(get_sibling(x), !get_direction(x), rbtree);
        } else {
            // case 4: all nodes mentioned above are black
            debug_print("case 4");

            // color the sibling red and continue the algorithm at the parent of x
            get_sibling(x)->color = RB_TREE_RED;
            x = x->parent;
        }
    }

    // color the x after the algorithm black to recover the properties
    x->color = RB_TREE_BLACK;
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

    struct Node *node_to_delete = NULL;
    search_node(rbtree, key, &node_to_delete);

    if (node_to_delete == NULL) {
        debug_print("There is no node with the given key!");
        return RB_TREE_KEY_ERROR;
    }

    struct Node *x = swap_to_leaf(node_to_delete);
    fix_tree_delete(x, rbtree);

    if (rbtree->root == x) {
        _free_node(rbtree->root);
        rbtree->root = NULL;
        return RB_TREE_SUCCESS;
    }

    if (get_direction(x) == RB_TREE_LEFT_CHILD) x->parent->left = NULL;
    else x->parent->right = NULL;

    _free_node(x);

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

    if (rbtree->root == NULL) return RB_TREE_SUCCESS;

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

    if (rbtree->root == NULL) return RB_TREE_SUCCESS;

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

    if (rbtree->root == NULL) return RB_TREE_SUCCESS;

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
