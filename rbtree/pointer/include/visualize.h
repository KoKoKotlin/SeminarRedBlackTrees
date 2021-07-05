#ifndef RBTREE_INCLUDE_VISUALIZE_H_
#define RBTREE_INCLUDE_VISUALIZE_H_

#include <math.h>
#include <stddef.h>
#include "rbtree.h"
#include "binary.h"

void ascii_art_tree(struct RBTree*);
size_t find_tree_height(struct Node*, size_t);
size_t find_tree_height_bin(struct BinNode*, size_t);

#endif  // RBTREE_INCLUDE_VISUALIZE_H_
