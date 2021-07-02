#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"
#include "include/visualize.h"
#include "include/binary.h"

#define NUMBER_OF_KEYS 10

void test_stack();
void test_rbtree();
void test_bin_tree();

int main()
{
    int keys[NUMBER_OF_KEYS];
    int searches[NUMBER_OF_KEYS];

    for (size_t i = 0; i < NUMBER_OF_KEYS; i++) {
        keys[i] = rand() % NUMBER_OF_KEYS;
        searches[i] = rand() % NUMBER_OF_KEYS;
    }

    // test_stack();
    clock_t start = clock();
    test_rbtree();
    printf("Time Red-Black-Tree: %.6fms, number of elements: %d \n", (double)(clock() - start) / (CLOCKS_PER_SEC / 1000), NUMBER_OF_KEYS);

    start = clock();
    test_bin_tree();
    printf("Time normal bintree: %.6fms, number of elements: %d \n", (double)(clock() - start) / (CLOCKS_PER_SEC / 1000), NUMBER_OF_KEYS);

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

void test_rbtree(int keys[], int searches[]) {
    struct RBTree *rbtree = create_tree();

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < NUMBER_OF_KEYS; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = u;

        insert_node(rbtree, elem, NULL);
    }



    // for (int u = NUMBER_OF_KEYS - 1; u >= 0; u--) {
    //     int key = rand() % NUMBER_OF_KEYS;
    //     delete_node(rbtree, &key);
    // }

    free_tree(rbtree);
}

void test_bin_tree(int keys[], int searches[]) {
    struct BinTree *binTree = create_bin_tree();

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < NUMBER_OF_KEYS; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = rand() % NUMBER_OF_KEYS;

        insert_bin_node(binTree, elem);
    }

    // for (int u = NUMBER_OF_KEYS - 1; u >= 0; u--) {
    //     int key = rand() % NUMBER_OF_KEYS;
    //     ascii_art_tree((struct RBTree*)binTree);
    //     delete_bin_node(binTree, &key);
    // }

    free_bin_tree(binTree);
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
