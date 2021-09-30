#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"
#include "include/visualize.h"
#include "include/binary.h"

// benchmark output format: CSV insert_rb, search_rb, delete_rb, height_rb, insert_bin, search_bin, delete_bin, height_bin

// source: https://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance
struct timespec timer_start()
{
    struct timespec start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    return start_time;
}

long timer_end(struct timespec start_time)
{
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    return diffInNanos;
}

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
    struct timespec time1;
    
    time1 = timer_start();
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = keys[u];

        insert_node(rbtree, elem, NULL);
    }
    printf("%ld ", timer_end(time1));
    
    time1 = timer_start();
    for (size_t i = 0; i < arr_size; i++) {
        struct Node* node;
        search_node(rbtree, &searches[i], &node);
    }
    printf("%ld ", timer_end(time1));
    
    time1 = timer_start();
    for (int u = arr_size - 1; u >= 0; u--) {
        int key = rand() % arr_size;
        delete_node(rbtree, &key);
    }
    printf("%ld ", timer_end(time1));
    
    printf("%zu ", find_tree_height(rbtree->root, 0));
}

void test_bin_tree(int keys[], int searches[], size_t arr_size) {
    struct BinTree *binTree = create_bin_tree();
    struct timespec time1;
    
    time1 = timer_start();
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = keys[u];

        insert_bin_node(binTree, elem);
    }
    printf("%ld ", timer_end(time1));

    time1 = timer_start();
    for (size_t i = 0; i < arr_size; i++) {
        search_bin(binTree, &searches[i]);
    }
    printf("%ld ", timer_end(time1));

    time1 = timer_start();
    for (int u = arr_size - 1; u >= 0; u--) {
        int key = rand() % arr_size;
        delete_bin_node(binTree, &key);
    }
    printf("%ld ", timer_end(time1));

    printf("%zu ", find_tree_height_bin(binTree->root, 0));
}

void benchmark(size_t number_of_keys)
{
    int *keys = (int*)malloc(sizeof(int) * number_of_keys);
    int *searches = (int*)malloc(sizeof(int) * number_of_keys);

    gen_rand_data(keys, searches, number_of_keys);

    test_rbtree(keys, searches, number_of_keys);
    test_bin_tree(keys, searches, number_of_keys);

    free(keys);
    free(searches);

}

int main(int argc, char** argv)
{   
    if (argc < 2) {
        printf("Usage: ./rbtree.out <number-of-keys>\n");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));

    long number_of_keys = atol(argv[1]);
    if (number_of_keys < 0) {
        printf("ERROR: number of keys was %s parsed to %ld!", argv[1], number_of_keys);
        exit(EXIT_FAILURE);
    }

    benchmark(number_of_keys);
    return 0;
}
