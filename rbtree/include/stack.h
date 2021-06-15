#ifndef RBTREE_INCLUDE_STACK_H_
#define RBTREE_INCLUDE_STACK_H_

#include <inttypes.h>

#define STACK_SUCCESS               0U
#define STACK_OUT_OF_MEM            1U
#define STACK_READ_FROM_EMTPY_STACK  2U

#define STACK_NOT_EMTPY 0U
#define STACK_EMPTY 1U


struct Stack {
    size_t current_position;
    size_t current_capacity;

    void **array;
};

struct Stack* create_stack(size_t);
void free_stack(struct Stack*);

uint8_t push(struct Stack*, void*);
uint8_t _pop(struct Stack*, void**);
#define pop(stack, elem) _pop((stack), (void**)(elem))

uint8_t _peek(struct Stack*, void**);
#define peek(stack, elem) _peek((stack), (void**)(elem))

uint8_t is_stack_empty(struct Stack*);

#endif  // RBTREE_INCLUDE_STACK_H_
