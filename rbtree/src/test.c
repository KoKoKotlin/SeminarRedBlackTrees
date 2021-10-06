#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rbtree.h"
#include "visualize.h"
#include "test_data.h"

struct Test {
    char name[20];
    char error_msg[100];
    char succ_msg[100];
    bool (*test_action)();
}; 

T* malloc_elem(T elem) {
    T* ptr = malloc(sizeof(T));
    *ptr = elem; 
    return ptr;
}

void print_node(struct Node* node)
{
    printf(T_FORMAT " ", *node->key);
}

bool height_test()
{
    struct RBTree* rbtree = create_tree();
    size_t expected = 6;

    for(int i = 0; i < 20; i++) { 
        int* elem = malloc_elem(i);
        insert_node(rbtree, elem, NULL);
    }

    size_t height = find_tree_height(rbtree->root, 0);
    bool res = expected == height;
    free_tree(rbtree);

    return res;
}

bool search_test()
{
    struct RBTree* rbtree = create_tree();

    for (size_t i = 0; i < SEARCH_DATA_SIZE; i++) {
        T* elem = malloc_elem(search_data[i]);
        insert_node(rbtree, elem, NULL);
    }

    bool res = true;
    for (size_t i = 0; i < SEARCH_TEST_DATA_SIZE; i++) {
        struct Node* node = NULL;
        
        size_t err = search_node(rbtree, successful_searches + i, &node);
        res &= node != NULL && err == RB_TREE_SUCCESS;
        printf("%zu: %d %p -> %d\n", i, successful_searches[i], node, res);
        
        err = search_node(rbtree, failed_searches + i, &node); 
        res &= node == NULL && err == RB_TREE_KEY_ERROR;
        printf("%zu: %d %p -> %d\n", i, failed_searches[i], node, res);
    }
    
    printf("Height: %zu\n", find_tree_height(rbtree->root, 0));
    free_tree(rbtree);

    return res;
}

bool delete_test()
{
    struct RBTree* rbtree = create_tree();

    for (size_t i = 0; i < SEARCH_DATA_SIZE; i++) {
        T* elem = malloc_elem(search_data[i]);
        insert_node(rbtree, elem, NULL);
    }

    bool res = true;
    for (size_t i = 0; i < SEARCH_TEST_DATA_SIZE; i++) {
        struct Node* node = NULL;
        size_t err = delete_node(rbtree, successful_searches + i);
        res &= err == RB_TREE_SUCCESS;
    }
    
    for (size_t i = 0; i < SEARCH_TEST_DATA_SIZE; i++) {
        struct Node* node = NULL;
        
        size_t err = search_node(rbtree, successful_searches + i, &node);
        res &= node == NULL && err == RB_TREE_KEY_ERROR;
        printf("%zu: %d %p -> %d\n", i, successful_searches[i], node, res);
    }

    printf("Height: %zu\n", find_tree_height(rbtree->root, 0));
    free_tree(rbtree);

    return res;
}

bool print_test() {
    struct RBTree* rbtree = create_tree();

    for(int i = 0; i < 18; i++) { 
        int* elem = malloc_elem(i);
        insert_node(rbtree, elem, NULL);
    }

    ascii_art_tree(rbtree);

    free_tree(rbtree);
    return true;
}

#define TEST_COUNT (sizeof(tests) / sizeof(struct Test))
static struct Test tests[] = {
    {"height", "Height test failed!", "Height test success!", &height_test},
    {"search", "Search test failed!", "Search test success!", &search_test},
    {"delete", "Delete test failed!", "Delete test success!", &delete_test},
    {"print", "Print test failed!", "Print test success!", &print_test},
};

void test(const char* test_name)
{
    struct Test* current = NULL;

    for (size_t i = 0; i < TEST_COUNT; i++)
        if (strcmp(test_name, tests[i].name) == 0) current = tests + i;
    
    bool res = current->test_action();

    if (res) printf("SUCCESS: %s\n", current->succ_msg);
    else printf("FAILURE: " "%s\n", current->error_msg);
}

void test_all()
{
    for (size_t i = 0; i < TEST_COUNT; i++) {
        struct Test current = tests[i];
        bool res = current.test_action();

        if (res) printf("SUCCESS: %s\n", current.succ_msg);
        else printf("FAILURE: %s\n", current.error_msg);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        test_all();
    } else {
        char* name = argv[1];
        test(name);
    }

    return 0;
}