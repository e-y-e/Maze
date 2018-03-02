#ifndef MAZE_H
#define MAZE_H


#include "action.h"
#include "point.h"

#include <stddef.h>


struct action_pair_t;
struct node_list_t;

/**
 * Represents the size of a rectangular maze.
 */
struct maze_size_t
{
    size_t width;
    size_t height;
};

/**
 * Represents a maze.
 */
struct maze_t
{
    struct action_pair_t* actions;
    struct maze_size_t size;
    struct point_t end;
    struct point_t start;
};

/**
 * Creates a maze with a given size, end and start.
 *
 * Returns -1 on failure, 0 on success.
 */
int make_maze(struct maze_t*, struct maze_size_t, struct point_t, struct point_t);

/**
 * Sets the action available at a given point in a maze.
 */
void set_action(struct maze_t, enum action_t, struct point_t);

/**
 * Solves a given maze using A* search.
 */
struct node_list_t* solve_maze(struct maze_t);


#endif // MAZE_H
