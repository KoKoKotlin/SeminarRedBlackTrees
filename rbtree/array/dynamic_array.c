#include "include/dynamic_array.h"

// TODO: errors

struct DynArray* create_dyn_array()
{
    // TODO: check pointers after malloc
    struct DynArray *arr = (struct DynArray*)malloc(sizeof(struct DynArray));
    arr->nodes = malloc(sizeof(struct Node) * 8);
    arr->current_size = 8;

    memset(arr->nodes, 0, sizeof(struct Node) * 8);

    return arr;
}

void free_dyn_array(struct DynArray *arr)
{
    for (size_t i = 0; i < arr->current_size; i++) {
        struct Node *node = get(arr, i);
        if (node != NULL) _free_node(node);
    }

    free(arr);
}

void _resize(struct DynArray *arr, size_t index)
{
    size_t new_size = arr->current_size * 2;
    while (new_size <= index) new_size *= 2;

    void *ptr = reallocarray(arr->nodes, sizeof(struct Node), new_size);

    arr->nodes = ptr;
    arr->current_size = new_size;
}

void insert(struct DynArray *arr, size_t index, struct Node *elem)
{
    if (index >= arr->current_size) {
        _resize(arr, index);
    }

    // dereference pointer to copy data into the array
    arr->nodes[index] = *elem;
}

struct Node *get(struct DynArray *arr, size_t index)
{
    if (index >= arr->current_size) {
        return NULL;
    }

    return arr->nodes + index;
}