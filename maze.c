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
 *     A pointer to the list of currently explored nodes.
 * \param [in]     maze
 *     The maze that the nodes are contained within.
 *
 * \pre
 *     The pointer to the node list variable must not be NULL.
 * \pre
 *     The pointer to the node variable must not be NULL.
 * \pre
 *     The pointer to the explored node list variable must not be NULL.
 */
static void get_children(struct node_list_t* list, struct node_t* node, struct node_list_t* explored, struct maze_t maze);

/**
 * \internal
 *
 * Get the index of the estimated best node in a list to expand for a given
 * maze start location.
 *
 * This helper function finds the best node in the given list by calculating the
 * estimated cost of choosing each node, with the given maze start location. The
 * index of this node is returned, and its contents are copied to the given
 * pointer so that the node can be accessed and removed from the list if
 * necessary.
 *
 * \param [out] best
 *     The pointer to the node variable which will contain the best node.
 * \param [in]  frontier
 *     A pointer to the list of possible candidate nodes.
 * \param [in]  start
 *     The start location of the maze that the nodes are contained within.
 *
 * \pre
 *     The pointer to the node variable must not be NULL.
 * \pre
 *     The pointer to the node list variable must not be NULL.
 * \pre
 *     The given list must not be empty.
 *
 * \returns
 *     The index of the estimated best node in the list.
 */
static size_t get_best_node(struct node_t* best, struct node_list_t* frontier, struct location_t start);


// Define make_maze (maze.h).
int make_maze(struct maze_t* maze, struct maze_size_t size, struct location_t start, struct location_t end)
{
    // Assert that the pointer to the maze variable is valid.
    assert(maze != NULL);
    // Assert that the given locations are within the maze.
    assert(check_location(size, start));
    assert(check_location(size, end));

    // Find the length of the array needed to store action sets for each node.
    size_t length = size.rows * size.columns;

    // Allocate the memory required for the array.
    void* ptr = calloc(length, sizeof(enum action_set_t));

    // Indicate failure if allocation failed.
    if (ptr == NULL) return -1;

    // Initialize maze properties.
    maze->action_sets = (enum action_set_t*) ptr;
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

    maze.action_sets[index] = action_set;
}

// Define get_action_set (maze.h).
enum action_set_t get_action_set(struct maze_t maze, struct location_t location)
{
    // Assert that the given location is within the maze.
    assert(check_location(maze.size, location));

    // Find the index to the action set based on the location.
    size_t index = location.row * maze.size.columns + location.column;

    // Get the correct action set.
    return maze.action_sets[index];
}

// Define solve_maze (maze.h).
void solve_maze(struct node_list_t* list, struct maze_t maze)
{
    // Assert that the pointer to the node list variable is valid.
    assert(list != NULL);

    // Define the maximum length for the frontier of nodes in the maze. Based on
    // the maximum proportion of the area that discovered but unexpanded nodes
    // can take up.
    size_t max_length = 2 * maze.size.rows * maze.size.columns / 3;

    // Create the list that will contain all nodes in the frontier.
    struct node_list_t frontier;
    if (make_list(&frontier, max_length) != 0) return;

    // Create the node that will represent the current node being explored.
    // Initially this is the end node, as this implementation works backwards.
    struct node_t node = { maze.end, NULL };

    // Insert the end node of the maze into the frontier.
    insert_node(&frontier, &node, 0);

    // Store the index of the next node to expand.
    size_t node_index = 0;

    // Search for the start node, using the given list to store explored nodes.
    while (frontier.length > 0)
    {
        // Get the next node to expand.
        node_index = get_best_node(&node, &frontier, maze.start);

        // Add the node to the list of explored nodes.
        insert_node(list, &node, list->length);

        // If the node is the start node, the search is complete.
        if (location_equal(node.location, maze.start)) return;

        // Remove the node from the frontier.
        remove_node(&frontier, node_index);

        // Generate the child nodes of the current node, appending them to the
        // frontier.
        get_children(&frontier, get_node(list, list->length - 1), list, maze);
    }
}

// Define check_location (maze.c).
static bool check_location(struct maze_size_t size, struct location_t location)
{
    return location.row < size.rows && location.column < size.columns;
}

// Define get_children (maze.c).
static void get_children(struct node_list_t* list, struct node_t* node, struct node_list_t* explored, struct maze_t maze)
{
    // Assert that the pointer to the node list variable is valid.
    assert(list != NULL);
    // Assert that the pointer to the node variable is valid.
    assert(node != NULL);
    // Assert that the pointer to the explored node list variable is valid.
    assert(explored != NULL);

    struct location_t location = node->location;

    // Get the set of actions available for the location.
    enum action_set_t action_set = get_action_set(maze, location);

    // Create a generic child node variable for reuse in each action.
    struct node_t child;
    child.parent = node;

    // Insert the child nodes reachable by each action to the list.
    for (enum action_t action = EAST; action <= NORTH; action++)
    {
        // Check if the action is contained in the set of actions.
        if (!(action_set & (1 << action))) continue;

        // Get the location reachable by the action.
        child.location = action_result(location, action);
        // Check that there is not already an explored node with this location.
        if (contains_node(explored, child.location)) continue;

        // Insert the child node into the list.
        insert_node(list, &child, list->length);
    }
}

// Define get_best_node (maze.c).
static size_t get_best_node(struct node_t* best, struct node_list_t* frontier, struct location_t start)
{
    // Assert that the pointer to the node variable is valid.
    assert(best != NULL);
    // Assert that the pointer to the node list variable is valid.
    assert(frontier != NULL);
    // Assert that the frontier is not empty.
    assert(frontier->length > 0);

    size_t length = frontier->length;

    // Store the best estimated cost and the index of the best node.
    size_t best_cost = (size_t) -1;
    size_t best_index = 0;

    // Search the nodes for the best node.
    struct node_t node;
    size_t cost = 0;
    for (size_t index = 0; index < length; index++)
    {
        // Calculate the estimated cost of choosing the node at this index.
        node = *get_node(frontier, index);
        cost = location_distance(node.location, start);

        // If necessary, update the best node, cost and index.
        if (cost < best_cost)
        {
            *best = node;
            best_cost = cost;
            best_index = index;
        }
    }

    return best_index;
}
