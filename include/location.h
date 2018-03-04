#ifndef LOCATION_H
#define LOCATION_H


#include <stdbool.h>
#include <stddef.h>


/**
 * Represents a location of a node in a rectangular maze.
 *
 * This struct is a simple representation of the row and column that define the
 * location of a node. The row and column are both unsigned, as they cannot be
 * negative.
 */
struct location_t
{
    size_t row;
    size_t column;
};

/**
 * Calculates a consistent measure of the distance between two locations.
 *
 * This function simply calculates the square of the straight-line distance
 * between the two locations, since taking the square root of integers would
 * impose an unnecessary overhead. This measure is still a consistent heuristic,
 * because the ordering is retained.
 *
 * Returns:
 *     A consistent measure of distance between the two locations.
 */
size_t location_distance(struct location_t, struct location_t);

/**
 * Determines if two locations are equal to each other.
 *
 * Simply compares the row and column of each location for exact equality.
 *
 * Returns:
 *     Whether the two locations are equal.
 */
bool location_equal(struct location_t, struct location_t);


#endif // LOCATION_H
