#include "maze.h"

#include "node.h"
#include "action_pair.h"
#include "node_list.h"

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


/**
 * Checks if a point is within a maze of a given size.
 */
static bool check_point(struct maze_size_t size, struct point_t point)
{
    return point.x < size.width && point.y < size.height;
}

/**
 * Gets the action available for a given point in a maze.
 */
static enum action_t get_action(struct maze_t maze, struct point_t point)
{
    // Assert that the given point is within the maze.
    assert(check_point(maze.size, point));

    // Find the index to the action based on the position.
    size_t index = point.y * maze.size.width + point.x;

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
    // Get the action available for the point.
    enum action_t action = get_action(maze, node->point);

    // Create a generic child node variable for reuse in each action.
    struct node_t child;
    child.parent = node;
    child.path_cost = node->path_cost + 1;

    // Insert all the child nodes reachable by the action to the list.
    if (action & NORTH)
    {
        child.point = (struct point_t) { node->point.x, node->point.y - 1 };
        insert_node(out, child, out->length);
    }
    if (action & EAST)
    {
        child.point = (struct point_t) { node->point.x + 1, node->point.y };
        insert_node(out, child, out->length);
    }
    if (action & SOUTH)
    {
        child.point = (struct point_t) { node->point.x, node->point.y + 1 };
        insert_node(out, child, out->length);
    }
    if (action & WEST)
    {
        child.point = (struct point_t) { node->point.x - 1, node->point.y };
        insert_node(out, child, out->length);
    }
}

int make_maze(struct maze_t* out, struct maze_size_t size, struct point_t end, struct point_t start)
{
    // Assert that the given points are within the maze.
    assert(check_point(size, end));
    assert(check_point(size, start));

    // Find the length of the array needed to store actions for each node.
    size_t length = (size.width * size.height + 1) / 2;

    // Allocate the memory required for the array.
    void* ptr = calloc(length, sizeof(struct action_pair_t));

    // Indicate failure if allocation failed.
    if (ptr == NULL) return -1;

    // Initialize maze properties.
    out->actions = (struct action_pair_t*) ptr;
    out->size = size;
    out->end = end;
    out->start = start;

    return 0;
}

void set_action(struct maze_t maze, enum action_t action, struct point_t point)
{
    // Assert that the given point is within the maze.
    assert(check_point(maze.size, point));

    // Find the index to the action based on the position.
    size_t index = point.y * maze.size.width + point.x;

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
