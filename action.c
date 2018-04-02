#include "action.h"

#include "location.h"

#include <assert.h>


// Define action_result (action.h).
struct location_t action_result(struct location_t location, enum action_t action)
{
    switch (action)
    {
        case EAST:
            return (struct location_t) { location.row, location.column + 1 };
        case SOUTH:
            return (struct location_t) { location.row + 1, location.column };
        case WEST:
            return (struct location_t) { location.row, location.column - 1 };
        case NORTH:
            return (struct location_t) { location.row - 1, location.column };
    }
}

// Define action_taken (action.h).
int action_taken(enum action_t* action, struct location_t a, struct location_t b)
{
    // Assert that the pointer to the action variable is valid.
    assert(action != NULL);

    if (location_equal(b, (struct location_t) { a.row, a.column + 1 }))
    {
        *action = EAST;
        return 0;
    }
    if (location_equal(b, (struct location_t) { a.row + 1, a.column }))
    {
        *action = SOUTH;
        return 0;
    }
    if (location_equal(b, (struct location_t) { a.row, a.column - 1 }))
    {
        *action = WEST;
        return 0;
    }
    if (location_equal(b, (struct location_t) { a.row - 1, a.column }))
    {
        *action = NORTH;
        return 0;
    }
    return -1;
}
