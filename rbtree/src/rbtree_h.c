#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "stack.h"
#include "rbtree.h"

uint8_t get_direction(struct Node *start_node)
{
    // convention when deleting nodes
    struct Node *parent = start_node->parent;
    if (parent == NULL) return RB_TREE_NULL_ERROR;

    return (parent->left == start_node) ? RB_TREE_LEFT_CHILD : RB_TREE_RIGHT_CHILD;
}

struct RBTree* create_tree()
{
    struct RBTree *rbtree = (struct RBTree*)calloc(sizeof(struct RBTree), 1);

    if (rbtree == NULL) {
        debug_print("Couldn't allocate memory for rbtree!");
        return NULL;
    }

    rbtree->root = NULL;
    rbtree->node_count = 0;

    debug_print("Created new RBTree.");

    return rbtree;
}

struct Node* _create_node(T *key, void *value)
{
    struct Node *new_node = (struct Node*)calloc(sizeof(struct Node), 1);

    if (new_node == NULL) {
        debug_print("Can't allocate memory for new node!");
        return NULL;
    }

    debug_printf("Creating new node with key " T_FORMAT ".", *key);

    new_node->key    = key;
    new_node->value  = value;
    new_node->parent = NULL;
    new_node->color  = RB_TREE_RED;

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
    if (start->parent != NULL) return start->parent->parent;
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

        struct Node *uncle = get_uncle(current);

        // TODO: test
        if (get_color(uncle) == RB_TREE_BLACK) {  // rotate
            if (get_direction(parent) != get_direction(current)) {
                rotate(parent, get_direction(parent), rbtree);
                current = parent;
                parent = current->parent;
            } else {
                struct Node *grandparent = get_grandparent(current);
                rotate(grandparent, !get_direction(parent), rbtree);
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
                if (TEQUAL(*(current->key), *key)) {
                    debug_printf("Key " T_FORMAT " already exists. Aborting...", *key);
                    return RB_TREE_DUPLICATE_KEY_ERROR;
                }
            #elif RB_TREE_DUPLICATE_KEYS == RB_TREE_DUPLICATE_OVERRIDE
                if (TEQUAL(*(current->key), *key)) {
                    debug_printf("Key " T_FORMAT " already exists. Overriding value.", *key);
                    free(current->value);
                    current->value = value;
                    return RB_TREE_SUCCESS;
                }
            #endif

            previous = current;
            current  = (TLESS(*key, *(previous->key))) ? previous->left : previous->right;
        }

        debug_printf("Needed to search %zu nodes!", count);

        if (TLESS(*key, *(previous->key))) {
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

struct Node* _search_node(struct Node* current, T* key)
{
    if (current == NULL || TEQUAL(*current->key, *key))  {
        debug_printf("Recursive search finished with %p.", current);
        return current;
    } else if(TLESS(*key, *current->key)) {
        debug_printf("Recursive search now advances to left subtree: " T_FORMAT " < " T_FORMAT ".", *key, *current->key);
        return _search_node(current->left, key);
    } else {
        debug_printf("Recursive search now advances to right subtree: " T_FORMAT " > " T_FORMAT ".", *key, *current->key);
        return _search_node(current->right, key);
    }
}

uint8_t search_node(struct RBTree* rbtree, T* key, struct Node** node) 
{
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    if (key == NULL) {
        debug_print("Given search key was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    struct Node* found = _search_node(rbtree->root, key);

    if (found == NULL) {
        debug_print("Key not found!");

        *node = NULL;
        return RB_TREE_KEY_ERROR;
    }

    debug_printf("Found node for key " T_FORMAT ".", *key);
    *node = found;

    return RB_TREE_SUCCESS;
}

void _preorder_traversel(struct Node *current, void (*action)(struct Node*))
{
    // root - left - right
    if (current == NULL) return;

    action(current);
    _preorder_traversel(current->left, action);
    _preorder_traversel(current->right, action);
}

uint8_t preorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{

    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    _preorder_traversel(rbtree->root, action);

    return RB_TREE_SUCCESS;
}

void _postorder_traversel(struct Node *current, void (*action)(struct Node*))
{
    if (current == NULL) return;
    
    _postorder_traversel(current->left, action);
    _postorder_traversel(current->right, action);
    action(current);
}

uint8_t postorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{
    // left - right - root
    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    _postorder_traversel(rbtree->root, action);

    return RB_TREE_SUCCESS;
}

void _inorder_traversel(struct Node *current, void (*action)(struct Node*))
{
    if (current == NULL) return;

    _inorder_traversel(current->left, action);
    action(current);
    _inorder_traversel(current->right, action);
}

uint8_t inorder_traversel(struct RBTree *rbtree, void (*action)(struct Node*))
{

    if (rbtree == NULL) {
        debug_print("Given tree was null! Aborting...");
        return RB_TREE_NULL_ERROR;
    }

    _inorder_traversel(rbtree->root, action);

    return RB_TREE_SUCCESS;
}