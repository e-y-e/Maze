#include "location.h"

#include <assert.h>


/**
 * \internal
 *
 * Calculates the absolute difference between two unsigned integers.
 *
 * This helper function allows the effective distance between two unsigned
 * integers to be calculated without having to convert to a signed
 * representation.
 *
 * \param [in] a
 *     The first of the unsigned integers.
 * \param [in] b
 *     The second of the unsigned integers.
 *
 * \returns
 *     The absolute difference between the two unsigned integers.
 */
static size_t abs_difference(size_t a, size_t b);

/**
 * \internal
 *
 * Estimates the value of the square root of a given number, starting from a
 * given estimate.
 *
 * This helper function uses the Newton-Raphson approximation to find an
 * approximate value of the square root of the given number, using the given
 * estimate as a starting point for iteration.
 *
 * \param [in] y
 *     The number to estimate the square root of.
 * \param [in] x
 *     A reasonable estimate used as a starting point for the estimation.
 *
 * \pre
 *     If the number is greater than 1, the candidate cannot be zero.
 *
 * \returns
 *     An estimate of the square root of the given number.
 */
static size_t estimate_sqrt(size_t y, size_t x);

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

    // The longest axis distance is a good starting point for approximation.
    size_t candidate = row_distance < column_distance ? column_distance
                                                      : row_distance;

    return estimate_sqrt(distance_squared, candidate);
}

// Define location_equal (location.h).
bool location_equal(struct location_t a, struct location_t b)
{
    return a.row == b.row && a.column == b.column;
}

// Define abs_difference (location.c).
static size_t abs_difference(size_t a, size_t b)
{
    return a < b ? b - a : a - b;
}

// Define estimate_sqrt (location.c).
static size_t estimate_sqrt(size_t y, size_t x)
{
    // Special case 0 and 1 as y = sqrt(y).
    if (y < 2) return y;

    // Assert that the candidate is not zero.
    assert(x != 0);

    // Perform 3 iterations of the Newton-Raphson approximation.
    size_t x_prime = 0;
    for (size_t index = 0; index < 3; index++)
    {
        x_prime = y / x;
        x = (x + x_prime) / 2;
    }

    return x;
}
