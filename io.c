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

    out->width = columns;
    out->height = rows;

    return 0;
}

static int read_point(struct point_t* out, FILE* fp)
{
    // Assert that the stream is valid.
    assert(fp != NULL);

    // If there are no lines to read, indicate an error.
    if (feof(fp)) return -1;

    char line[64] = "";
    char* end_ptr = NULL;

    end_ptr = fgets(line, 64, fp);
    if (end_ptr == NULL) return -1;

    // Convert the first number in the buffer to the y location.
    out->y = strtoul(end_ptr, &end_ptr, 10);

    // Convert the next number in the buffer to the x location.
    out->x = strtoul(end_ptr, &end_ptr, 10);

    return 0;
}

static int read_actions(struct maze_t maze, FILE* fp)
{
    // Assert that the stream is valid.
    assert(fp != NULL);

    size_t width = maze.size.width;
    size_t height = maze.size.height;

    char line[512] = "";
    char* end_ptr = NULL;

    size_t walls = 0;
    enum action_t action;

    size_t x = 0;
    size_t y = 0;
    for (;;)
    {
        if (y >= height) return 0;

        // If there are no lines left to read, indicate an error.
        if (feof(fp)) return -1;

        end_ptr = fgets(line, 512, fp);
        if (end_ptr == NULL) return -1;

        x = 0;
        for (;;)
        {
            if (x >= width) break;

            walls = strtoul(end_ptr, &end_ptr, 10);

            action = (enum action_t) (~walls & 0xF);

            set_action(maze, action, (struct point_t) { x, y });

            x++;
        }

        y++;
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

    // Read the start point of the maze.
    struct point_t start;
    int read_start_result = read_point(&start, fp);
    if (read_start_result != 0) return read_start_result;

    // Read the end point of the maze.
    struct point_t end;
    int read_end_result = read_point(&end, fp);
    if (read_end_result != 0) return read_end_result;

    // Construct the maze from the data read.
    make_maze(maze, size, start, end);

    // Read the actions of the maze.
    read_actions(*maze, fp);

    return 0;
}
