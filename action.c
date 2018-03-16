#include "action.h"

#include "location.h"


// Define action_result (action.h).
struct location_t action_result(struct location_t location, enum action_t action)
{
    switch (action)
    {
        case NORTH:
            return (struct location_t) { location.row - 1, location.column };
        case WEST:
            return (struct location_t) { location.row, location.column - 1 };
        case SOUTH:
            return (struct location_t) { location.row + 1, location.column };
        case EAST:
            return (struct location_t) { location.row, location.column + 1 };
        default:
            return location;
    }
}
