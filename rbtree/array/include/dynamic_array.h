#ifndef RBTREE_INCLUDE_DYNAMIC_ARRAY_H_
#define RBTREE_INCLUDE_DYNAMIC_ARRAY_H_

#include <inttypes.h>
#include <stdlib.h>

#include "rbtree.h"

struct DynArray {
    struct Node *nodes;

    size_t current_size;
};

struct DynArray* create_dyn_array();
void free_dyn_array(struct DynArray*);

void insert(struct DynArray*, size_t, struct Node*);
void get(struct DynArray*, size_t, struct Node**);

#endif  // RBTREE_INCLUDE_DYNAMIC_ARRAY_H_