#include "maze_size.c"

#include "location.h"


// Define check_location (maze_size.h).
bool check_location(struct maze_size_t size, struct point_t location)
{
    return location.row < size.rows && location.column < size.columns;
}
