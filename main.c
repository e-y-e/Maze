#include <stdio.h>

#include "io.h"
#include "node.h"
#include "node_list.h"
#include "maze.h"

int main()
{
    FILE* fp = fopen("file.txt", "r");

    if (fp == NULL)
    {
        printf("Failed to open file.txt\n");
        return -1;
    }

    struct maze_t maze;
    int read_maze_result = read_maze(&maze, fp);

    if (read_maze_result != 0)
    {
        printf("Failed to read maze: return code %d\n", read_maze_result);
        return read_maze_result;
    }

    struct node_list_t explored;
    int make_list_result = make_list(&explored, 64);

    if (make_list_result != 0)
    {
        printf("Failed to make list: return code %d\n", make_list_result);
        return make_list_result;
    }

    solve_maze(&explored, maze);

    struct point_t point;

    size_t length = explored.length;
    size_t index = 0;

    for (;;)
    {
        if (index >= length) break;

        point = get_node(explored, index)->point;

        printf("Explored point { %ld, %ld }\n", point.x, point.y);

        index++;
    }

    return 0;
}
