#include "io.h"

#include "maze.h"

#include <assert.h>
#include <stdlib.h>


static int read_size(struct maze_size_t* out, FILE* fp)
{
    // Assert that the stream is valid.
    assert(fp != NULL);

    // If there are no lines to read, indicate an error.
    if (feof(fp)) return -1;

    char line[64] = "";
    char* end_ptr = NULL;

    end_ptr = fgets(line, 64, fp);
    if (end_ptr == NULL) return -1;

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

static int read_location(struct location_t* out, FILE* fp)
{
    // Assert that the stream is valid.
    assert(fp != NULL);

    // If there are no lines to read, indicate an error.
    if (feof(fp)) return -1;

    char line[64] = "";
    char* end_ptr = NULL;

    end_ptr = fgets(line, 64, fp);
    if (end_ptr == NULL) return -1;

    // Convert the first number in the buffer to the row.
    out->row = strtoul(end_ptr, &end_ptr, 10);

    // Convert the next number in the buffer to the column.
    out->column = strtoul(end_ptr, &end_ptr, 10);

    return 0;
}

static int read_actions(struct maze_t maze, FILE* fp)
{
    // Assert that the stream is valid.
    assert(fp != NULL);

    size_t rows = maze.size.rows;
    size_t columns = maze.size.columns;

    char line[512] = "";
    char* end_ptr = NULL;

    size_t walls = 0;
    enum action_t action;

    size_t row = 0;
    size_t column = 0;
    for (;;)
    {
        if (row >= rows) return 0;

        // If there are no lines left to read, indicate an error.
        if (feof(fp)) return -1;

        end_ptr = fgets(line, 512, fp);
        if (end_ptr == NULL) return -1;

        column = 0;
        for (;;)
        {
            if (column >= columns) break;

            walls = strtoul(end_ptr, &end_ptr, 10);

            action = (enum action_t) (~walls & 0xF);

            set_action(maze, action, (struct location_t) { row, column });

            column++;
        }

        row++;
    }
}

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
