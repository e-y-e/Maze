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
 * \param [out] out A pointer to the maze variable that will store the maze.
 * \param [in] fp The file handle to read data from.
 *
 * \pre
 *     The file pointer must not be NULL.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
int read_maze(struct maze_t* out, FILE* fp);

/**
 * Writes an ascii character representation of a maze to a file.
 *
 * This function uses the character '#' along with whitespace to print out the
 * given maze to the file. Each location in the maze is represented by a 3x4
 * (rowsxcolumns) block of characters, apart from two of the edges, which have
 * an additional row and/or column to represent the edge. As an example, a 3x3
 * maze might be printed as follows:
 *
 * \code{.unparsed}
 * #############
 * #           #
 * #########   #
 * #       #   #
 * #   #       #
 * #   #       #
 * #############
 * \endcode
 *
 * \param [in] maze The maze to write to the file.
 * \param [in] fp The file handle to write the maze to.
 *
 * \pre
 *     The file pointer must not be NULL.
 */
int write_maze(struct maze_t maze, FILE* fp);


#endif // IO_H
