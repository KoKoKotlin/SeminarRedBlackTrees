#ifndef RBTREE_INCLUDE_STACK_H_
#define RBTREE_INCLUDE_STACK_H_

#include <inttypes.h>

#define STACK_SUCCESS 0
#define STACK_OUT_OF_MEM 1
#define STACK_POP_FROM_EMTPY_STACK 2

struct Stack {
    size_t current_position;
    size_t current_capacity;

    void **array;
};

struct Stack* create_stack(size_t);
void free_stack(struct Stack*);

uint8_t push(struct Stack*, void*);
uint8_t pop(struct Stack*, void**);

#endif  // RBTREE_INCLUDE_STACK_H_
