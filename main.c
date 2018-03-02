#include <stdio.h>

#include "node.h"
#include "node_list.h"
#include "maze.h"

int main()
{
    struct maze_t maze;
    make_maze(&maze, (struct maze_size_t) { 5, 5 }, (struct point_t) { 0, 0 }, (struct point_t) { 4, 4 });

    enum action_t action = (enum action_t) (NORTH | EAST | SOUTH | WEST);

    size_t x = 0;
    size_t y = 0;
    for (;;)
    {
        if (y >= 5) break;

        x = 0;
        for (;;)
        {
            if (x >= 5) break;

            set_action(maze, action, (struct point_t) { x, y });

            x++;
        }

        y++;
    }

    struct node_list_t path;
    make_list(&path, 32);

    solve_maze(&path, maze);

    size_t length = path.length;

    struct point_t point;
    size_t index = 0;
    for (;;)
    {
        if (index >= length) break;
        point = get_node(path, index)->point;
        printf("Point: { %ld, %ld }\n", point.x, point.y);
        index++;
    }

    return 0;
}
