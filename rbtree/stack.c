#include <stdio.h>
#include <stdlib.h>

#include "include/stack.h"
#include "include/log.h"

struct Stack* create_stack(size_t initial_capacity)
{
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->current_position = 0;
    stack->current_capacity = initial_capacity;

    stack->array = malloc(sizeof(void*) * initial_capacity);

    if (stack->array == NULL) {
        debug_print("Couldn't allocate memory for stack!");
        return NULL;
    }

    debug_printf("Allocated stack for %zu elements!", initial_capacity);
    return stack;
}

void free_stack(struct Stack *stack)
{
    debug_print("Freed stack memory.");
    free(stack->array);
    free(stack);
}

uint8_t push(struct Stack *stack, void *element)
{
    if (stack->current_position == stack->current_capacity) {
        void *ptr = reallocarray(stack->array, sizeof(void*), stack->current_capacity * 2);
        if (ptr == NULL) {
            debug_printf("Couldn't realloc array of stack with size %zu!", stack->current_capacity);
            return STACK_OUT_OF_MEM;
        }

        stack->array = ptr;
        stack->current_capacity *= 2;
        debug_printf("Realloced the stack memory. Now has size %zu.", stack->current_capacity);
    }

    stack->array[stack->current_position++] = element;
    debug_printf("Pushed element at %p to stack. Current position is now at %zu.", element, stack->current_position);

    return STACK_SUCCESS;
}

uint8_t pop(struct Stack *stack, void **element)
{
    if (stack->current_position == 0) {
        debug_print("Prevented pop from empty stack!");
        *element = NULL;
        return STACK_POP_FROM_EMTPY_STACK;
    }

    *element = stack->array[--(stack->current_position)];
    debug_printf("Popped element at %p from stack. Current position is now at %zu", element, stack->current_position);
    return STACK_SUCCESS;
}
