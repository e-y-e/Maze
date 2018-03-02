#include "point.h"


// Calculates the absolute difference between two unsigned integers.
static size_t abs_difference(size_t a, size_t b)
{
    return a < b ? b - a : a - b;
}

size_t point_distance(struct point_t a, struct point_t b)
{
    size_t dx = abs_difference(a.x, b.x);
    size_t dy = abs_difference(a.y, b.y);

    return dx * dx + dy * dy;
}

bool point_equal(struct point_t a, struct point_t b)
{
    return a.x == b.x && a.y == b.y;
}
