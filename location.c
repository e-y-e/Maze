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
    size_t row_distance = abs_difference(a.row, b.row);
    size_t column_distance = abs_difference(a.column, b.column);

    // Combine the axis distances to obtain the square of the straight-line
    // distance.
    size_t distance_squared = row_distance * row_distance
                            + column_distance * column_distance;

    // The longest axis distance is the best starting point for approximation.
    size_t candidate = row_distance < column_distance ? column_distance
                                                      : row_distance;

    // Approximate the distance by iterating through candidate solutions.
    for (;;)
    {
        if (candidate * candidate >= distance_squared) return candidate;
        candidate++;
    }
}

// Define location_equal (location.h).
bool location_equal(struct location_t a, struct location_t b)
{
    return a.row == b.row && a.column == b.column;
}
