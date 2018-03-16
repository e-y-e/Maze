#ifndef NODE_LIST_H
#define NODE_LIST_H


#include <stdbool.h>
#include <stddef.h>


struct location_t;
struct node_t;

/**
 * Represents a dynamically allocated list of nodes.
 *
 * This struct contains a pointer to a dynamically allocated section of memory,
 * and the necessary properties in order to use that memory to store a dynamic
 * list of nodes.
 *
 * \see test_node_list()
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
 * \param [out] out A pointer to the node list variable that will be initialized.
 * \param [in] initial_capacity The initial storage capacity of the node list.
 *
 * \returns
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
 * \param [out] list A pointer to the list to resize.
 * \param [in] new_capacity The new storage capacity of the node list.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
int resize_list(struct node_list_t* list, size_t new_capacity);

/**
 * Get the node at a given index in a list.
 *
 * This function simply indexes the given list at the given index to retrieve a
 * pointer to the node at that index.
 *
 * \param [in] list The node list.
 * \param [in] index The index of the node.
 *
 * \pre
 *     The index must be within the length of the given list.
 *
 * \returns
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
 * \param [out] list A pointer to the node list to insert the node into.
 * \param [in] node The node to insert.
 * \param [in] index The index to insert the node at.
 *
 * \pre
 *     The index must be within the length of the given list, accounting for the
 *     additional item.
 *
 * \returns
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
 * \param [out] list A pointer to the node list to remove the node from.
 * \param [in] index The index of the node.
 *
 * \pre
 *     The index must be within the length of the given list.
 */
void remove_node(struct node_list_t* list, size_t index);

/**
 * Determines if there is a node with a given location in a list.
 *
 * This function simply attempts to find a node in the given list that has a
 * location that is the same as the given location, and indicates whether it was
 * able to find one.
 *
 * \param [in] list The node list that will be searched.
 * \param [in] location The location of the node to be found.
 *
 * \returns
 *     Whether there is a node with the given location in the list.
 */
bool contains_node(struct node_list_t list, struct location_t location);

/**
 * Tests the type node_list_t and its associated functions.
 *
 * This function runs through constructing and using node lists of varying
 * sizes, and inserts nodes at various indexes, asserting that the nodes are in
 * the right position. The remove_node() function and the contains_node()
 * function are also tested with some simple inputs.
 */
void test_node_list(void);


#endif // NODE_LIST_H
