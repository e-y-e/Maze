#ifndef NODE_LIST_H
#define NODE_LIST_H


#include <stddef.h>


struct node_t;

/**
 * Represents a dynamically allocated list of nodes.
 */
struct node_list_t
{
    struct node_t* nodes;
    size_t length;
    size_t capacity;
};

/**
 * Creates a node list with an initial storage capacity.
 *
 * Allocates the required memory using malloc, and initializes the properties of
 * the list.
 *
 * Returns -1 on failure, 0 on success.
 */
int make_list(struct node_list_t*, size_t);

/**
 * Resizes a node list to accomodate an updated capacity.
 *
 * Reallocates the required memory using realloc, and updates the properties of
 * the list to reflect the new capacity.
 *
 * Returns -1 on failure, 0 on success.
 */
int resize_list(struct node_list_t*, size_t);

/**
 * Get the node at a given index in a list.
 *
 * Returns NULL on failure.
 */
struct node_t* get_node(struct node_list_t, size_t);

/**
 * Inserts a node into a node list at a given index.
 *
 * Inserts a node into the list at the position given by the index. If
 * necessary, resizes the list to accomodate the growth.
 *
 * Returns -1 on failure, 0 on success.
 */
int insert_node(struct node_list_t*, struct node_t, size_t);

/**
 * Removes the node at a given index in a node list.
 *
 * Returns -1 on failure, 0 on success.
 */
int remove_node(struct node_list_t*, size_t);


#endif // NODE_LIST_H
