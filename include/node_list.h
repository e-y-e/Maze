#ifndef NODE_LIST_H
#define NODE_LIST_H


#include <stddef.h>


struct node_t;

/**
 * Represents a dynamically allocated list of nodes.
 *
 * This struct contains a pointer to a dynamically allocated section of memory,
 * and the necessary properties in order to use that memory to store a dynamic
 * list of nodes.
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
 * This function attempts to initializes all the properties of the given pointer
 * after allocating a section of memory for the list using malloc, big enough to
 * store the given initial capacity.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int make_list(struct node_list_t* out, size_t initial_capacity);

/**
 * Resizes a node list to accomodate an updated capacity.
 *
 * This function attempts to update the properties of the given pointer after
 * reallocating a section of memory using realloc, big enough to store the new
 * capacity, and updates the properties of the list to reflect the new capacity.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int resize_list(struct node_list_t* list, size_t new_capacity);

/**
 * Get the node at a given index in a list.
 *
 * This function simply indexes the given list at the given index to retrieve a
 * pointer to the node at that index.
 *
 * Preconditions:
 *     The index must be within the length of the given list.
 *
 * Returns:
 *     A pointer to the node at the given index.
 */
struct node_t* get_node(struct node_list_t list, size_t index);

/**
 * Inserts a node into a node list at a given index.
 *
 * This function attempts to insert the given node into the list at the given
 * index by ensuring that the capacity is available, resizing if necessary, and
 * using direct memory operations to insert the node into the correct location
 * in memory, shifting the contents of the list where necessary.
 *
 * Preconditions:
 *     The index must be within the length of the given list, accounting for the
 *     additional item.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int insert_node(struct node_list_t* list, struct node_t node, size_t index);

/**
 * Removes the node at a given index in a node list.
 *
 * This function attempts to remove the node at the given index in the list by
 * using direct memory operations to remove the node from the correct location
 * in memory, shifting the contents of the list where necessary.
 *
 * Preconditions:
 *     The index must be within the length of the given list.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int remove_node(struct node_list_t* list, size_t index);


#endif // NODE_LIST_H
