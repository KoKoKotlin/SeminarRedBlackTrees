#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"
#include "include/visualize.h"
#include "include/binary.h"

// benchmark output format: CSV insert_rb, search_rb, delete_rb, height_rb, insert_bin, search_bin, delete_bin, height_bin

// source: https://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance
struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
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
    struct timespec time1, time2;
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = keys[u];

        insert_node(rbtree, elem, NULL);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (size_t i = 0; i < arr_size; i++) {
        struct Node* node;
        search_node(rbtree, &searches[i], &node);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (int u = arr_size - 1; u >= 0; u--) {
        int key = rand() % arr_size;
        delete_node(rbtree, &key);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);
    
    printf("%zu ", find_tree_height(rbtree->root, 0));
}

void test_bin_tree(int keys[], int searches[], size_t arr_size) {
    struct BinTree *binTree = create_bin_tree();
    struct timespec time1, time2;
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (size_t u = 0; u < arr_size; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = keys[u];

        insert_bin_node(binTree, elem);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (size_t i = 0; i < arr_size; i++) {
        search_bin(binTree, &searches[i]);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (int u = arr_size - 1; u >= 0; u--) {
        int key = rand() % arr_size;
        delete_bin_node(binTree, &key);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    printf("%ld ", diff(time1, time2).tv_nsec);

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
    if (argc < 2) exit(EXIT_FAILURE);
    
    srand(time(NULL));

    long number_of_keys = atol(argv[1]);
    if (number_of_keys < 0) {
        printf("ERROR: number of keys was %s parsed to %ld!", argv[1], number_of_keys);
        exit(EXIT_FAILURE);
    }

    benchmark(number_of_keys);
    return 0;
}
