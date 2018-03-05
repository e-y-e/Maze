#include "maze.h"

#include "location.h"
#include "node.h"
#include "action_pair.h"
#include "node_list.h"

#include <assert.h>
#include <stdlib.h>


/**
 * Gets the action available at a given location in a maze.
 *
 * This helper function simply gets the action available at the given location
 * in the maze by finding the correct entry in the actions array.
 *
 * Preconditions:
 *     The location must be within the maze.
 *
 * Returns:
 *     The action available at the given location.
 */
static enum action_t get_action(struct maze_t maze, struct location_t location)
{
    // Assert that the given location is within the maze.
    assert(check_location(maze.size, location));

    // Find the index to the action based on the location.
    size_t index = location.row * maze.size.columns + location.column;

    // Get the correct action from the pair of actions
    return index & 1 ? maze.actions[index / 2].b : maze.actions[index / 2].a;
}

/**
 * Gets all the children reachable from a given node in a maze.
 *
 * This helper function generates the nodes reachable from the given node by
 * finding the locations resulting from the action available at the node's
 * location and constructing the child nodes from the results. Each child node
 * is appended to the given list, unless the child node is among those already
 * present in the given list of explored nodes.
 */
static void get_children(struct node_list_t* out, struct node_t* node, struct node_list_t explored, struct maze_t maze)
{
    struct location_t location = node->location;

    // Get the action available for the location.
    enum action_t action = get_action(maze, location);

    // Create a generic child node variable for reuse in each action.
    struct node_t child;
    child.parent = node;

    // Insert all the child nodes reachable by the action to the list.
    if (action & NORTH)
    {
        child.location = (struct location_t) { location.row - 1, location.column };
        if (!contains_node(explored, child.location))
        {
            insert_node(out, child, out->length);
        }
    }
    if (action & WEST)
    {
        child.location = (struct location_t) { location.row, location.column - 1 };
        if (!contains_node(explored, child.location))
        {
            insert_node(out, child, out->length);
        }
    }
    if (action & SOUTH)
    {
        child.location = (struct location_t) { location.row + 1, location.column };
        if (!contains_node(explored, child.location))
        {
            insert_node(out, child, out->length);
        }
    }
    if (action & EAST)
    {
        child.location = (struct location_t) { location.row, location.column + 1 };
        if (!contains_node(explored, child.location))
        {
            insert_node(out, child, out->length);
        }
    }
}

/**
 * Calculate the estimated cost of choosing a given node.
 *
 * This helper function simply estimates the cost of the path via the given node
 * to the end of the maze. This will always be an underestimate of the true path
 * cost.
 *
 * Returns:
 *     The estimated cost of choosing the given node.
 */
static size_t cost_estimate(struct node_t node, struct maze_t maze)
{
    return 1 + location_distance(node.location, maze.end);
}

/**
 * Get the index of the estimated best node in a list to expand for a given
 * maze.
 *
 * This helper function finds the best node in the given list by calculating the
 * estimated cost of choosing each node, with the given maze. The index of this
 * node is returned so that the node can be accessed and removed from the list
 * if necessary.
 *
 * Preconditions:
 *     The given list must not be empty.
 *
 * Returns:
 *     The index of the estimated best node in the list.
 */
static size_t get_best_node(struct node_list_t frontier, struct maze_t maze)
{
    // Assert that the frontier is not empty.
    assert(frontier.length > 0);

    // Store the best node index and its estimated cost. Assume for now that the
    // first node is the best node.
    size_t best_index = 0;
    size_t best_cost = cost_estimate(*get_node(frontier, 0), maze);

    size_t length = frontier.length;

    // Search the remaining nodes for the best node.
    size_t index = 1;
    size_t cost = 0;
    for (;;)
    {
        if (index >= length) break;

        // Calculate the estimated cost of choosing the node at this index.
        cost = cost_estimate(*get_node(frontier, index), maze);

        // If necessary, update the best node.
        if (cost < best_cost)
        {
            best_index = index;
            best_cost = cost;
        }

        index++;
    }

    return best_index;
}

// Define make_maze (maze.h).
int make_maze(struct maze_t* out, struct maze_size_t size, struct location_t start, struct location_t end)
{
    // Assert that the given locations are within the maze.
    assert(check_location(size, start));
    assert(check_location(size, end));

    // Find the length of the array needed to store actions for each node.
    size_t length = (size.rows * size.columns + 1) / 2;

    // Allocate the memory required for the array.
    void* ptr = calloc(length, sizeof(struct action_pair_t));

    // Indicate failure if allocation failed.
    if (ptr == NULL) return -1;

    // Initialize maze properties.
    out->actions = (struct action_pair_t*) ptr;
    out->size = size;
    out->start = start;
    out->end = end;

    return 0;
}

// Define set_action (maze.h).
void set_action(struct maze_t maze, enum action_t action, struct location_t location)
{
    // Assert that the given location is within the maze.
    assert(check_location(maze.size, location));

    // Find the index to the action based on the location.
    size_t index = location.row * maze.size.columns + location.column;

    // Set the correct action in the pair of actions.
    if (index & 1)
    {
        maze.actions[index / 2].b = action;
    }
    else
    {
        maze.actions[index / 2].a = action;
    }
}

// Define solve_maze (maze.h).
void solve_maze(struct node_list_t* out, struct maze_t maze)
{
    // Define the maximum length for lists of nodes in the maze.
    size_t max_length = maze.size.rows * maze.size.columns;

    // Create the list that will contain all nodes in the frontier.
    struct node_list_t frontier;
    if (make_list(&frontier, max_length) != 0) return;

    // Store the index of the next node to expand.
    size_t node_index = 0;

    // Create the node that will represent the current node being explored.
    // Initially this is the end node, as this implementation works backwards.
    struct node_t node = { maze.end, NULL };

    // Insert the end node of the maze into the frontier.
    insert_node(&frontier, node, 0);

    // Search for the start node, using the given list to store explored nodes.
    for (;;)
    {
        if (frontier.length == 0) return;

        // Get the next node to expand.
        node_index = get_best_node(frontier, maze);
        node = *get_node(frontier, node_index);

        // Add the node to the list of out nodes.
        insert_node(out, node, out->length);

        // If the node is the start node, the search is complete.
        if (location_equal(node.location, maze.start)) return;

        // Remove the node from the frontier.
        remove_node(&frontier, node_index);

        // Generate the child nodes of the current node, appending them to the
        // frontier.
        get_children(&frontier, get_node(*out, out->length - 1), *out, maze);
    }
}
