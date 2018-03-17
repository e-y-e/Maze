#include "maze.h"

#include "location.h"
#include "node.h"
#include "node_list.h"

#include <assert.h>
#include <stdlib.h>


/**
 * \internal
 *
 * Checks if a given location is within a maze of a given size.
 *
 * This helper function simply compares the given location with the number of
 * rows and columns in the maze, and determines whether the location is within
 * its bounds.
 *
 * \param [in] size
 *     The size of the maze in which the location should be.
 * \param [in] location
 *     The location to check.
 *
 * \returns
 *     Whether the given location is within a maze of the given size.
 */
static bool check_location(struct maze_size_t size, struct location_t location);

/**
 * \internal
 *
 * Gets all the children reachable from a given node in a maze.
 *
 * This helper function generates the nodes reachable from the given node by
 * finding the locations resulting from the set of actions available at the
 * node's location and constructing the child nodes from the results. Each child
 * node is appended to the given list, unless the child node is among those
 * already present in the given list of explored nodes.
 *
 * \param [in,out] list
 *     A pointer to the node list to which the child nodes are appended.
 * \param [in]     node
 *     A pointer to the parent node.
 * \param [in]     explored
 *     The list of currently explored node.
 * \param [in]     maze
 *     The maze that the nodes are contained within.
 */
static void get_children(struct node_list_t* list, struct node_t* node, struct node_list_t explored, struct maze_t maze);

/**
 * \internal
 *
 * Get the index of the estimated best node in a list to expand for a given
 * maze.
 *
 * This helper function finds the best node in the given list by calculating the
 * estimated cost of choosing each node, with the given maze. The index of this
 * node is returned so that the node can be accessed and removed from the list
 * if necessary.
 *
 * \param [in] frontier
 *     The list of possible candidate nodes.
 * \param [in] maze
 *     The maze that the nodes are contained within.
 *
 * \pre
 *     The given list must not be empty.
 *
 * \returns
 *     The index of the estimated best node in the list.
 */
static size_t get_best_node(struct node_list_t frontier, struct maze_t maze);


// Define make_maze (maze.h).
int make_maze(struct maze_t* maze, struct maze_size_t size, struct location_t start, struct location_t end)
{
    // Assert that the pointer to the maze variable is valid.
    assert(maze != NULL);
    // Assert that the given locations are within the maze.
    assert(check_location(size, start));
    assert(check_location(size, end));

    // Find the length of the array needed to store action sets for each node.
    size_t length = (size.rows * size.columns + 1) / 2;

    // Allocate the memory required for the array.
    void* ptr = calloc(length, sizeof(struct action_set_pair_t));

    // Indicate failure if allocation failed.
    if (ptr == NULL) return -1;

    // Initialize maze properties.
    maze->action_sets = (struct action_set_pair_t*) ptr;
    maze->size = size;
    maze->start = start;
    maze->end = end;

    return 0;
}

// Define set_action_set (maze.h).
void set_action_set(struct maze_t maze, enum action_set_t action_set, struct location_t location)
{
    // Assert that the given location is within the maze.
    assert(check_location(maze.size, location));

    // Find the index to the action set based on the location.
    size_t index = location.row * maze.size.columns + location.column;

    // Set the correct action set in the pair of action sets.
    if (index & 1)
    {
        maze.action_sets[index / 2].b = action_set;
    }
    else
    {
        maze.action_sets[index / 2].a = action_set;
    }
}

// Define get_action_set (maze.h).
enum action_set_t get_action_set(struct maze_t maze, struct location_t location)
{
    // Assert that the given location is within the maze.
    assert(check_location(maze.size, location));

    // Find the index to the action set based on the location.
    size_t index = location.row * maze.size.columns + location.column;

    // Get the correct action set from the pair of action sets
    return index & 1 ? maze.action_sets[index / 2].b
                     : maze.action_sets[index / 2].a;
}

// Define solve_maze (maze.h).
void solve_maze(struct node_list_t* list, struct maze_t maze)
{
    // Assert that the pointer to the node list variable is valid.
    assert(list != NULL);

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
    for (; frontier.length > 0;)
    {
        // Get the next node to expand.
        node_index = get_best_node(frontier, maze);
        node = *get_node(frontier, node_index);

        // Add the node to the list of out nodes.
        insert_node(list, node, list->length);

        // If the node is the start node, the search is complete.
        if (location_equal(node.location, maze.start)) return;

        // Remove the node from the frontier.
        remove_node(&frontier, node_index);

        // Generate the child nodes of the current node, appending them to the
        // frontier.
        get_children(&frontier, get_node(*list, list->length - 1), *list, maze);
    }
}

// Define check_location (maze.c).
static bool check_location(struct maze_size_t size, struct location_t location)
{
    return location.row < size.rows && location.column < size.columns;
}


// Define get_children (maze.c).
static void get_children(struct node_list_t* list, struct node_t* node, struct node_list_t explored, struct maze_t maze)
{
    struct location_t location = node->location;

    // Get the set of actions available for the location.
    enum action_set_t action_set = get_action_set(maze, location);

    // Create a generic child node variable for reuse in each action.
    struct node_t child;
    child.parent = node;

    // Insert the child nodes reachable by each action to the list.
    for (enum action_t action = (enum action_t) 0; action <= (enum action_t) 3; action++)
    {
        // Check if the action is contained in the set of actions.
        if (!(action_set & (1 << action))) continue;

        // Get the location reachable by the action.
        child.location = action_result(location, action);
        // Check that there is not already an explored node with this location.
        if (contains_node(explored, child.location)) continue;

        // Insert the child node into the list.
        insert_node(list, child, list->length);
    }
}

// Define get_best_node (maze.c).
static size_t get_best_node(struct node_list_t frontier, struct maze_t maze)
{
    // Assert that the frontier is not empty.
    assert(frontier.length > 0);

    // Store the best node index and its estimated cost.
    size_t best_index = 0;
    size_t best_cost = (size_t) -1;

    size_t length = frontier.length;

    // Search the nodes for the best node.
    size_t cost = 0;
    for (size_t index = 0; index < length; index++)
    {
        // Calculate the estimated cost of choosing the node at this index.
        cost = location_distance(get_node(frontier, index)->location, maze.start);

        // If necessary, update the best node.
        if (cost < best_cost)
        {
            best_index = index;
            best_cost = cost;
        }
    }

    return best_index;
}
