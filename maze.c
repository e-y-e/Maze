#include "maze.h"

#include "node.h"
#include "action_pair.h"
#include "node_list.h"

#include <assert.h>
#include <stdlib.h>


/**
 * Gets the action available for a given location in a maze.
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
 * Appends the child nodes to the end of a given node list.
 */
static void get_children(struct node_list_t* out, struct maze_t maze, struct node_t* node)
{
    // Get the action available for the location.
    enum action_t action = get_action(maze, node->location);

    // Create a generic child node variable for reuse in each action.
    struct node_t child;
    child.parent = node;
    child.path_cost = node->path_cost + 1;

    // Insert all the child nodes reachable by the action to the list.
    if (action & NORTH)
    {
        child.location = (struct location_t) { node->location.row - 1, node->location.column };
        insert_node(out, child, out->length);
    }
    if (action & WEST)
    {
        child.location = (struct location_t) { node->location.row, node->location.column - 1 };
        insert_node(out, child, out->length);
    }
    if (action & SOUTH)
    {
        child.location = (struct location_t) { node->location.row + 1, node->location.column };
        insert_node(out, child, out->length);
    }
    if (action & EAST)
    {
        child.location = (struct location_t) { node->location.row, node->location.column + 1 };
        insert_node(out, child, out->length);
    }
}

/**
 * Calculate the estimated cost of choosing a given node.
 */
static size_t cost_estimate(struct node_t node, struct maze_t maze)
{
    return node.path_cost + location_distance(node.location, maze.end);
}

/**
 * Get the frontier index of the estimated best node to expand next in a given maze.
 */
static size_t get_best_node(struct node_list_t frontier, struct maze_t maze)
{
    // Assert that the frontier is not empty.
    assert(frontier.length > 0);

    // Store the best node index and its estimated cost.
    size_t best_index = 0;
    size_t best_cost = cost_estimate(*get_node(frontier, 0), maze);

    size_t length = frontier.length;

    // Store the current node index and its estimated cost.
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

void solve_maze(struct node_list_t* out, struct maze_t maze)
{
    // Define the maximum length for lists of nodes in the maze.
    size_t max_length = maze.size.rows * maze.size.columns;

    // Create the list that will contain all nodes in the frontier.
    struct node_list_t frontier;
    if (make_list(&frontier, max_length) != 0) return;

    // Store the index of the next node to expand.
    size_t node_index = 0;

    // Create the node that will represent the current end of the path.
    struct node_t node = { maze.start, NULL, 0 };

    // Insert the first node of the maze into the frontier.
    insert_node(&frontier, node, 0);

    // Search for the end node, using the given list to store explored nodes.
    for (;;)
    {
        if (frontier.length == 0) return;

        // Get the next node to expand.
        node_index = get_best_node(frontier, maze);
        node = *get_node(frontier, node_index);

        // Add the node to the list of out nodes.
        insert_node(out, node, out->length);

        // If the node is the goal node, the search is complete.
        if (location_equal(node.location, maze.end)) return;

        // Remove the node from the frontier.
        remove_node(&frontier, node_index);

        // Generate the child nodes of the current node, appending them to the
        // frontier.
        get_children(&frontier, maze, get_node(*out, out->length - 1));
    }
}
