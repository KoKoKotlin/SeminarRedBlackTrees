#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"

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

    T *one  = malloc_elem(1);
    T *two  = malloc_elem(2);
    T *three  = malloc_elem(3);
    T *four  = malloc_elem(4);
    T *five  = malloc_elem(5);
    T *six  = malloc_elem(6);
    T *seven  = malloc_elem(7);

    // creating balanced tree
    insert_node(rbtree, four, NULL);
    insert_node(rbtree, two, NULL);
    insert_node(rbtree, one, NULL);
    insert_node(rbtree, three, NULL);
    insert_node(rbtree, six, NULL);
    insert_node(rbtree, five, NULL);
    insert_node(rbtree, seven, NULL);

    int key = 3;
    struct Node *snode = NULL;
    search_node(rbtree, &key, &snode);

    preorder_traversel(rbtree, &print_key);
    printf("\n");

    inorder_traversel(rbtree, &print_key);
    printf("\n");

    postorder_traversel(rbtree, &print_key);
    printf("\n");

    delete_node(rbtree, &key);
    inorder_traversel(rbtree, &print_key);
    printf("\n");

    key = 2;
    delete_node(rbtree, &key);
    inorder_traversel(rbtree, &print_key);
    printf("\n");

    key = 5;
    delete_node(rbtree, &key);
    inorder_traversel(rbtree, &print_key);
    printf("\n");

    key = 6;
    delete_node(rbtree, &key);
    inorder_traversel(rbtree, &print_key);
    printf("\n");

/*
    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < 10000; u++) {
        T *elem = malloc_elem((T)rand());
        insert_node(rbtree, elem, NULL);
    }

    preorder_traversel(rbtree, &print_key);
    printf("\n");

    inorder_traversel(rbtree, &print_key);
    printf("\n");

    postorder_traversel(rbtree, &print_key);
    printf("\n");
*/
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
        int *elem = malloc_elem(rand());
        error = pop(stack, &elem);
        printf("elem %zu: %d with error code %u\n", u, *elem, error);
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
