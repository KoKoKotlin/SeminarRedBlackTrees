#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"
#include "include/visualize.h"
#include "include/binary.h"

#define CLOCKS_PER_MILLIS (CLOCKS_PER_SEC / 1000)

T* malloc_elem(T elem) {
    T* elem_ptr = (T*)malloc(sizeof(T));
    *elem_ptr = elem;
    return elem_ptr;
}

void print_key(struct Node *node) {
    printf(T_FORMAT " ", *(node->key));
}

void gen_rand_data(int *keys, int *searches, size_t number_of_keys)
{
    for (size_t i = 0; i < number_of_keys; i++) {
        keys[i] = rand() % number_of_keys;
        searches[i] = rand() % number_of_keys;
    }
}

void gen_degenerate_data(int *keys, int *searches, size_t number_of_keys)
{
    for (size_t i = 0; i < number_of_keys; i++) {
        keys[i] = i;
        searches[i] = i;
    }
}

void test_rbtree(int keys[], int searches[], size_t arr_size) {
    struct RBTree *rbtree = create_tree();
    clock_t start;

    srand(time(NULL));
    start = clock();
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        if (elem == NULL) printf("ALERT!!\n");
        *elem = keys[u];

        insert_node(rbtree, elem, NULL);
    }
    printf("%lf ", (double)(clock() - start) / CLOCKS_PER_MILLIS);
    printf("%zu ", find_tree_height(rbtree->root, 0));

    start = clock();
    for (size_t i = 0; i < arr_size; i++) {
        struct Node* node;
        search_node(rbtree, &searches[i], &node);
    }
    printf("%lf ", (double)(clock() - start) / CLOCKS_PER_MILLIS);

    // for (int u = NUMBER_OF_KEYS - 1; u >= 0; u--) {
    //     int key = rand() % NUMBER_OF_KEYS;
    //     delete_node(rbtree, &key);
    // }

    start = clock();
    free_tree(rbtree);
    printf("%f", (double)(clock() - start) / CLOCKS_PER_MILLIS);
}

void test_bin_tree(int keys[], int searches[], size_t arr_size) {
    struct BinTree *binTree = create_bin_tree();
    clock_t start;

    srand(time(NULL));
    start = clock();
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = keys[u];

        insert_bin_node(binTree, elem);
    }
    printf("%f ", (double)(clock() - start) / CLOCKS_PER_MILLIS);
    printf("%zu ", find_tree_height_bin(binTree->root, 0));

    start = clock();
    for (size_t i = 0; i < arr_size; i++) {
        search_bin(binTree, &searches[i]);
    }
    printf("%f ", (double)(clock() - start) / CLOCKS_PER_MILLIS);

    // for (int u = NUMBER_OF_KEYS - 1; u >= 0; u--) {
    //     int key = rand() % NUMBER_OF_KEYS;
    //     ascii_art_tree((struct RBTree*)binTree);
    //     delete_bin_node(binTree, &key);
    // }

    start = clock();
    free_bin_tree(binTree);
    printf("%f\n", (double)(clock() - start) / CLOCKS_PER_MILLIS);
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

int main()
{
    for (size_t i = 0; i < 100; i++) {
        size_t number_of_keys = (i + 1) * 10000;

        int *keys = (int*)malloc(sizeof(int) * number_of_keys);
        int *searches = (int*)malloc(sizeof(int) * number_of_keys);

        gen_rand_data(keys, searches, number_of_keys);

        test_rbtree(keys, searches, number_of_keys);
        printf(" ");
        test_bin_tree(keys, searches, number_of_keys);

        free(keys);
        free(searches);
    }
    return 0;
}
