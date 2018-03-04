#include "io.h"

#include "action.h"
#include "location.h"
#include "maze_size.h"
#include "maze.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/**
 * Read a maze size from a given file.
 *
 * This helper function simply attempts to set the properties of the given
 * maze size based on the data read from the given file.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
static int read_size(struct maze_size_t* out, FILE* fp)
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

    out->rows = rows;
    out->columns = columns;

    return 0;
}

/**
 * Read a location from a given file.
 *
 * This helper function simply attempts to set the properties of the given
 * location based on the data read from the given file.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
static int read_location(struct location_t* out, FILE* fp)
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
    out->row = strtoul(end_ptr, &end_ptr, 10);

    // Convert the next number in the buffer to the column.
    out->column = strtoul(end_ptr, &end_ptr, 10);

    return 0;
}

/**
 * Read the actions for a maze from a given file.
 *
 * This helper function attempts to initialize all the actions in the maze based
 * on the data read from the given file.
 *
 * Returns:
 *     -1 on failure, 0 on success.
 */
static int read_actions(struct maze_t maze, FILE* fp)
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
    for (;;)
    {
        if (row >= rows) return 0;

        // If there are no lines left to read, indicate an error.
        if (feof(fp)) return -1;

        // Attempt to read a line, indicate an error on failure or buffer overflow.
        end_ptr = fgets(line, 512, fp);
        if (end_ptr == NULL || strchr(end_ptr, '\n') == NULL) return -1;

        column = 0;
        for (;;)
        {
            if (column >= columns) break;

            // Convert the next number to the specification for the walls at
            // this location.
            walls = strtoul(end_ptr, &end_ptr, 10);

            // Based on the specification for the walls, set the action at this
            // location.
            set_action(maze, (enum action_t) (~walls & 0xF),
                       (struct location_t) { row, column });

            column++;
        }

        row++;
    }
}

// Define read_maze (io.h)
int read_maze(struct maze_t* maze, FILE* fp)
{
    // Assert that the stream is valid.
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
    read_actions(*maze, fp);

    return 0;
}
