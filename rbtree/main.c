#include <time.h>

#include "include/log.h"
#include "include/rbtree.h"
#include "include/stack.h"

void test_stack();

int main()
{
    test_stack();
    return 0;
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
        int *elem = NULL;
        error = pop(stack, (void**)&elem);
        printf("elem %zu: %d with error code %u\n", u, *elem, error);
    }

    srand(time(NULL));
    // fill stack with lots of heap allocated random numbers
    for (size_t u = 0; u < 10000; u++) {
        int *elem = (int*)malloc(sizeof(int));
        *elem = rand();

        push(stack, (void*)elem);
    }

    // get the random numbers back
    for (size_t u = 0; u < 10000; u++) {
        int *elem;

        error = pop(stack, (void**)&elem);
        printf("elem %zu: %d with error code %u\n", u, *elem, error);
    }

    // pop empty stack
    int *elem;
    error = pop(stack, (void**)&elem);
    printf("(nil) == %p, error: %d\n", elem, error);

    // free the data structure
    free_stack(stack);
}
