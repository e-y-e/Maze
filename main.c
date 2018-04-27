#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "location.h"
#include "node.h"
#include "node_list.h"
#include "maze.h"
#include "io.h"

#ifndef TEST

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("Usage: maze [-p] input_file output_file\n");
        return -1;
    }

    bool print = false;
    size_t arg_offset = 0;

    char* arg = argv[1];
    if (strncmp(arg, "-p", 2) != 0)
    {
        print = true;
        arg_offset = 1;
    }

    if ((size_t) argc < arg_offset + 3)
    {
        printf("Usage: maze [-p] input_file output_file\n");
        return -1;
    }

    char* filename = argv[arg_offset + 1];

    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Failed to open %s\n", filename);
        return -1;
    }

    struct maze_t maze;
    int read_maze_result = read_maze(&maze, fp);

    if (read_maze_result != 0)
    {
        printf("Failed to read maze: return code %d\n", read_maze_result);
        return -1;
    }

    size_t capacity = maze.size.rows * maze.size.columns;
    struct node_list_t explored;
    int make_list_result = make_list(&explored, capacity);

    if (make_list_result != 0)
    {
        printf("Failed to make list: return code %d\n", make_list_result);
        return -1;
    }

    solve_maze(&explored, maze);

    size_t length = explored.length;

    struct node_t node;
    for (size_t index = 0; index < length; node = *get_node(&explored, index++)) {}

//     struct location_t location;
//     for (;;)
//     {
//         location = node.location;
//         printf("{ %ld, %ld }\n", location.row, location.column);
//         if (node.parent == NULL) break;
//         node = *node.parent;
//     }

    FILE* fp2 = fopen("out.txt", "w");

    write_path(&node, fp2);
    fclose(fp2);

    return 0;
}

#endif // TEST
