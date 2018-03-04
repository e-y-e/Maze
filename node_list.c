#include "node_list.h"

#include "location.h"
#include "node.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


// Define make_list (node_list.h).
int make_list(struct node_list_t* out, size_t initial_capacity)
{
    // Allocate required memory.
    void* ptr = malloc(initial_capacity * sizeof(struct node_t));

    // Indicate failure if allocation failed, if allocation was required.
    if (ptr == NULL && initial_capacity != 0) return -1;

    // Initialize list properties.
    out->nodes = (struct node_t*) ptr;
    out->length = 0;
    out->capacity = initial_capacity;

    return 0;
}

// Define resize_list (node_list.h).
int resize_list(struct node_list_t* list, size_t new_capacity)
{
    // Allocate required memory.
    void* ptr = realloc((void*) list->nodes, new_capacity * sizeof(struct node_t));

    // Indicate failure if allocation failed, if allocation was required.
    if (ptr == NULL && new_capacity != 0) return -1;

    // Update list properties to reflect new capacity.
    list->nodes = (struct node_t*) ptr;
    list->capacity = new_capacity;

    return 0;
}

// Define get_node (node_list.h).
struct node_t* get_node(struct node_list_t list, size_t index)
{
    // Assert that the index cannot be past the end of the list.
    assert(index < list.length);

    return &list.nodes[index];
}

// Define insert_node (node_list.h).
int insert_node(struct node_list_t* list, struct node_t node, size_t index)
{
    // Assert that the index cannot be past where the end of the list would be
    // after inserting another node.
    assert(index < list->length + 1);

    // If the capacity has been reached, resize the list.
    if (list->length >= list->capacity)
    {
        // Resize according to 2 * previous capacity.
        size_t new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        int resize_result = resize_list(list, new_capacity);

        // Indicate failure if resize failed.
        if (resize_result != 0) return -1;
    }

    // Find the pointer to the position in the list referenced by the index.
    char* insert_ptr = (char*) &list->nodes[index];

    // Move the nodes after the insert position along to accomodate the new node.
    memmove(insert_ptr + sizeof(struct node_t), insert_ptr, (list->length - index) * sizeof(struct node_t));

    // Insert the node into the list using direct memcpy.
    memcpy(insert_ptr, (char*) &node, sizeof(struct node_t));

    // Update list length to reflect additional node.
    list->length++;

    return 0;
}

// Define remove_node (node_list.h).
void remove_node(struct node_list_t* list, size_t index)
{
    // Assert that the index cannot be past the end of the list.
    assert(index < list->length);

    // No need to move memory around when the position indicates the last
    // node in the list. Simply decrease the length.
    if (index == list->length - 1)
    {
        list->length--;
        return;
    }

    // Find the pointer to the position in the list referenced by the index.
    char* remove_ptr = (char*) &list->nodes[index];

    // Move the nodes after the remove position to the position of the removed node.
    memmove(remove_ptr, remove_ptr + sizeof(struct node_t), (list->length - index) * sizeof(struct node_t));

    // Update list length to reflect removed node.
    list->length--;
}

// Define contains_node (node_list.h)
bool contains_node(struct node_list_t list, struct location_t location)
{
    size_t length = list.length;

    // Search the list for a node at the given location
    size_t index = 0;
    for (;;)
    {
        // Indicate if no node was found.
        if (index >= length) return false;

        // If the node at this index is at the given location, indicate that the
        // node was found.
        if (location_equal(list.nodes[index].location, location)) return true;

        index++;
    }
}
