#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"
#include "include/visualize.h"

void test_stack();
void test_rbtree();

int main()
{
    // test_stack();
    test_rbtree();

    return 0;
}

T* malloc_elem(T elem) {
    T* elem_ptr = (T*)malloc(sizeof(T));
    *elem_ptr = elem;
    return elem_ptr;
}

void print_key(struct Node *node) {
    printf(T_FORMAT " ", *(node->key));
}

void test_rbtree() {
    struct RBTree *rbtree = create_tree();

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < 18; u++) {
        T *elem = malloc_elem((T)u);
        insert_node(rbtree, elem, NULL);
    }

    ascii_art_tree(rbtree);

    for (int key = 0; key < 18; key++) {
        delete_node(rbtree, &key);
        ascii_art_tree(rbtree);
    }

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < 10000; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = u;

        insert_node(rbtree, elem, NULL);
    }

    for (int u = 10000 - 1; u >= 0; u--) {
        int key = rand() % 10000;
        delete_node(rbtree, &key);
    }

    free_tree(rbtree);
}

void test_stack() {
    struct Stack *stack = create_stack(2);

    // example elements
    int i = 10;
    int j = 43;
    int k = 56;

    // push 2 elements -> no resize
    uint8_t error = push(stack, &i);
    printf("Error: %u\n", error);

    error = push(stack, &j);
    printf("Error: %u\n", error);

    // push the third element -> resize to 4
    push(stack, &k);

    // retreiving 3 elements back
    for (size_t u = 0; u < 3; u++) {
        T *elem = malloc_elem((T)rand());
        error = pop(stack, &elem);
        printf("elem %zu:" T_FORMAT " with error code %u\n", u, *elem, error);
    }

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < 10000; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = rand();

        push(stack, elem);
    }

    // get the random numbers back
    for (size_t u = 0; u < 10000; u++) {
        int *elem;

        error = pop(stack, &elem);
        printf("elem %zu: %d with error code %u\n", u, *elem, error);
    }

    // pop empty stack
    int *elem;
    error = pop(stack, &elem);
    printf("(nil) == %p, error: %d\n", elem, error);

    // free the data structure
    free_stack(stack);
}
