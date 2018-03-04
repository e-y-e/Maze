#ifndef MAZE_H
#define MAZE_H


#include "action.h"
#include "location.h"
#include "maze_size.h"


struct action_pair_t;
struct node_list_t;

/**
 * Represents a maze.
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
 * Returns -1 on failure, 0 on success.
 */
int make_maze(struct maze_t* out, struct maze_size_t size, struct location_t start, struct location_t end);

/**
 * Sets the action available at a given location in a maze.
 */
void set_action(struct maze_t maze, enum action_t action, struct location_t location);

/**
 * Solves a given maze using A* search.
 */
void solve_maze(struct node_list_t* out, struct maze_t maze);


#endif // MAZE_H
