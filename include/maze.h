#ifndef MAZE_H
#define MAZE_H


#include "action.h"
#include "location.h"
#include "maze_size.h"


struct action_pair_t;
struct node_list_t;

/**
 * Represents a maze.
 *
 * This struct contains the properties necessary to find a solution to the maze,
 * specifically a pointer to an array of actions for every location in the maze,
 * along with the size of the maze and its start and end locations.
 */
struct maze_t
{
    struct action_pair_t* actions;
    struct maze_size_t size;
    struct location_t start;
    struct location_t end;
};

/**
 * Creates a maze with a given size, end and start.
 *
 * This function attempts to initialize all the properties of the given pointer
 * after allocating a section of memory for the actions, big enough to store the
 * actions for the entire maze.
 *
 * Preconditions:
 *     The start and end locations must be within the maze.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int make_maze(struct maze_t* out, struct maze_size_t size, struct location_t start, struct location_t end);

/**
 * Sets the action available at a given location in a maze.
 *
 * This function simply sets the action available at the given location in the
 * maze by updating the correct entry in the actions array.
 *
 * Preconditions:
 *     The location must be within the maze.
 */
void set_action(struct maze_t maze, enum action_t action, struct location_t location);

/**
 * Solves a given maze using A* search.
 *
 * This function attempts to find a path from the end of the given maze back to
 * the start, using A* search, which iteratively expands the next closest node
 * to the start location. Every expanded node is inserted into the given list in
 * order, such that the final node in the list will be the start of the maze, if
 * the search was successful.
 */
void solve_maze(struct node_list_t* out, struct maze_t maze);


#endif // MAZE_H
