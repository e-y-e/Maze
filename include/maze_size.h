#ifndef MAZE_SIZE_H
#define MAZE_SIZE_H


#include <stdbool.h>
#include <stddef.h>


struct location_t;

/**
 * Represents the size of a rectangular maze.
 *
 * This struct is a simple representation of the number of rows and columns that
 * define the size of a maze. The rows and columns are both unsigned, as they
 * cannot be negative.
 */
struct maze_size_t
{
    size_t rows;
    size_t columns;
};

/**
 * Checks if a given location is within a maze of a given size.
 *
 * This function simply compares the given location with the number of rows and
 * columns in the maze, and determines whether the location is within its
 * bounds.
 *
 * \param [in] size The size of the maze in which the location should be.
 * \param [in] location The location to check.
 *
 * \returns
 *     Whether the given location is within a maze of the given size.
 */
bool check_location(struct maze_size_t maze, struct location_t location);


#endif // MAZE_SIZE_H
