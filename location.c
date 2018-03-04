#include "location.h"


/**
 * Calculates the absolute difference between two unsigned integers.
 *
 * This helper function allows the effective distance between two unsigned
 * integers to be calculated without having to convert to a signed
 * representation.
 *
 * Returns: the absolute difference between the two unsigned integers.
 */
static size_t abs_difference(size_t a, size_t b)
{
    return a < b ? b - a : a - b;
}

// Define location_distance (location.h).
size_t location_distance(struct location_t a, struct location_t b)
{
    // Calculate the distances on each axis.
    size_t dx = abs_difference(a.row, b.row);
    size_t dy = abs_difference(a.column, b.column);

    // Combine the axis distances to obtain the square of the straight-line
    // distance.
    return dx * dx + dy * dy;
}

// Define location_equal (location.h).
bool location_equal(struct location_t a, struct location_t b)
{
    return a.row == b.row && a.column == b.column;
}
