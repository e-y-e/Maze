#ifndef POINT_H
#define POINT_H


#include <stddef.h>


/**
 * Represents a position in 2d space.
 */
struct point_t
{
    size_t x;
    size_t y;
};

/**
 * Calculates the relative distance between two points. Proportional to the
 * square of the straight-line distance.
 */
size_t point_distance(struct point_t, struct point_t);


#endif // POINT_H
