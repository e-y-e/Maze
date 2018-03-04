#ifndef IO_H
#define IO_H


#include <stdio.h>


struct maze_t;

/**
 * Reads the contents of a file as a maze.
 *
 * This function attempts to initialize all the properties of the maze
 * referenced by the given pointer, according to the contents of the file. The
 * format of the file should be as follows, with individual values separated by
 * whitespace:
 *
 * Line 1: size of the maze - rows followed by columns.
 * Line 2: start location - row followed by column.
 * Line 3: end location - row followed by column.
 *
 * The remaining lines define the walls at each location for each row of the
 * maze. The walls are defined by a number between 1 and 15, defined as follows:
 *
 * If the location has a wall to the north the variable has 8 added to it.
 * If the location has a wall to the west the variable has 4 added to it.
 * If the location has a wall to the south the variable has 2 added to it.
 * If the location has a wall to the east the variable has 1 added to it.
 *
 * Preconditions:
 *     The file pointer must not be NULL.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
int read_maze(struct maze_t* out, FILE* fp);


#endif // IO_H
