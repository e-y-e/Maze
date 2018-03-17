#include "io.h"

#include "action.h"
#include "location.h"
#include "maze_size.h"
#include "maze.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/**
 * \internal
 *
 * Read a maze size from a given file.
 *
 * This helper function simply attempts to set the properties of the given
 * maze size based on the data read from the given file.
 *
 * \param [out] size
 *     A pointer to the maze size variable to read the size to.
 * \param [in]  fp
 *     The file handle to read data from.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
static int read_size(struct maze_size_t* size, FILE* fp);

/**
 * \internal
 *
 * Read a location from a given file.
 *
 * This helper function simply attempts to set the properties of the given
 * location based on the data read from the given file.
 *
 * \param [out] location
 *     A pointer to the location variable to read the location to.
 * \param [in]  fp
 *     The file handle to read data from.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
static int read_location(struct location_t* location, FILE* fp);

/**
 * \internal
 *
 * Read the sets of actions for a maze from a given file.
 *
 * This helper function attempts to initialize all the action sets in the maze
 * based on the data read from the given file.
 *
 * \param [in,out] maze
 *     The maze variable containing the pointer to the action set array to
 *     initialize.
 * \param [in]     fp
 *     The file handle to read data from.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
static int read_action_sets(struct maze_t maze, FILE* fp);


// Define read_maze (io.h)
int read_maze(struct maze_t* maze, FILE* fp)
{
    // Assert that the pointer to the maze variable is valid.
    assert(maze != NULL);
    // Assert that the file handle is valid.
    assert(fp != NULL);

    // Read the size of the maze.
    struct maze_size_t size;
    int read_size_result = read_size(&size, fp);
    if (read_size_result != 0) return read_size_result;

    // Read the start location of the maze.
    struct location_t start;
    int read_start_result = read_location(&start, fp);
    if (read_start_result != 0) return read_start_result;

    // Read the end location of the maze.
    struct location_t end;
    int read_end_result = read_location(&end, fp);
    if (read_end_result != 0) return read_end_result;

    // Construct the maze from the data read.
    make_maze(maze, size, start, end);

    // Read the actions of the maze.
    read_action_sets(*maze, fp);

    return 0;
}

// Define write_maze (io.h)
int write_maze(struct maze_t maze, FILE* fp)
{
    // Assert that the file handle is valid.
    assert(fp != NULL);

    size_t rows = maze.size.rows;
    size_t columns = maze.size.columns;

    unsigned char walls = 0;
    int fprintf_result = 0;

    size_t row = 0;
    size_t column = 0;

    for (; row < rows; row++)
    {
        // Write the north walls and north-east corners of this row to the file.
        for (column = 0; column < columns; column++)
        {
            walls = ~get_action_set(maze, (struct location_t) { row, column })
                  & 0x0F;

            fprintf_result = fprintf(fp,
                walls & 0x08 ? "####" : (walls & 0x04 ? "#   " : "    "));
            if (fprintf_result != 4) return -1;
        }

        // Write out any extra characters for the north-west corner of this row,
        // followed by the end of the line.
        fprintf_result = fprintf(fp, walls & 0x09 ? "#\n" : " \n");
        if (fprintf_result != 2) return -1;

        // Write the east walls of this row to the file.
        column = 0;
        for (;;)
        {
            if (column >= columns) break;

            walls = ~get_action_set(maze, (struct location_t) { row, column })
                  & 0x0F;

            fprintf_result = fprintf(fp, walls & 0x04 ? "#   " : "    ");
            if (fprintf_result != 4) return -1;

            column++;
        }

        // Write out any extra characters for the east wall of this row,
        // followed by the end of the line.
        fprintf_result = fprintf(fp, walls & 0x01 ? "#\n" : " \n");
        if (fprintf_result != 2) return -1;
    }

    // Write the south walls and south-east corners of the final row of the maze to the file.
    for (column = 0; column < columns; column++)
    {
        walls = ~get_action_set(maze, (struct location_t) { row - 1, column })
              & 0x0F;

        fprintf_result = fprintf(fp, walls & 0x02 ? "####"
                                                  : (walls & 0x04 ? "#   "
                                                                  : "    "));
        if (fprintf_result != 4) return -1;
    }

    // Write out any extra characters for the south-east corner of the maze,
    // followed by the end of the line.
    fprintf_result = fprintf(fp, walls & 0x03 ? "#\n" : " \n");
    if (fprintf_result != 2) return -1;

    return 0;
}

// Define read_size (io.c).
static int read_size(struct maze_size_t* size, FILE* fp)
{
    // If there are no lines to read, indicate an error.
    if (feof(fp)) return -1;

    // Create a buffer for the line. The line should be a maximum of 40
    // characters.
    char line[64] = "";
    char* end_ptr = NULL;

    // Attempt to read a line, indicate an error on failure or buffer overflow.
    end_ptr = fgets(line, 64, fp);
    if (end_ptr == NULL || strchr(end_ptr, '\n') == NULL) return -1;

    // Convert the first number in the buffer to the number of rows.
    size_t rows = strtoul(end_ptr, &end_ptr, 10);
    if (rows == 0) return -1;

    // Convert the next number in the buffer to the number of columns.
    size_t columns = strtoul(end_ptr, &end_ptr, 10);
    if (columns == 0) return -1;

    size->rows = rows;
    size->columns = columns;

    return 0;
}

// Define read_location (io.c).
static int read_location(struct location_t* location, FILE* fp)
{
    // If there are no lines to read, indicate an error.
    if (feof(fp)) return -1;

    // Create a buffer for the line. The line should be a maximum of 40
    // characters.
    char line[64] = "";
    char* end_ptr = NULL;

    // Attempt to read a line, indicate an error on failure or buffer overflow.
    end_ptr = fgets(line, 64, fp);
    if (end_ptr == NULL || strchr(end_ptr, '\n') == NULL) return -1;

    // Convert the first number in the buffer to the row.
    location->row = strtoul(end_ptr, &end_ptr, 10);

    // Convert the next number in the buffer to the column.
    location->column = strtoul(end_ptr, &end_ptr, 10);

    return 0;
}

// Define read_action_sets (io.c).
static int read_action_sets(struct maze_t maze, FILE* fp)
{
    size_t rows = maze.size.rows;
    size_t columns = maze.size.columns;

    // Create a buffer for the line. The line could in theory have more
    // characters, but for now it will be reported as an error.
    char line[4096] = "";
    char* end_ptr = NULL;

    size_t walls = 0;

    // Iterate through the maze, setting the action at each location.
    size_t row = 0;
    size_t column = 0;
    for (; row < rows; row++)
    {
        // If there are no lines left to read, indicate an error.
        if (feof(fp)) return -1;

        // Attempt to read a line, indicate an error on failure or buffer
        // overflow.
        end_ptr = fgets(line, 4096, fp);
        if (end_ptr == NULL || strchr(end_ptr, '\n') == NULL) return -1;

        for (column = 0; column < columns; column++)
        {
            // Convert the next number to the specification for the walls at
            // this location.
            walls = strtoul(end_ptr, &end_ptr, 10);

            // Based on the specification for the walls, set the set of actions
            // at this location.
            set_action_set(maze, (enum action_set_t) (~walls & 0xF),
                           (struct location_t) { row, column });
        }
    }

    return 0;
}
