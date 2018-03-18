#ifndef ACTION_SET_H
#define ACTION_SET_H


#include "action.h"


/**
 * Represents the set of actions available at a node in a maze.
 *
 * This enumerated type provides representations for the directions in which a
 * node can be expanded. These representations allow multiple directions to be
 * combined into a set of actions using simple bitwise operations.
 */
enum action_set_t
{
    EAST_FLAG  = 1 << EAST,
    SOUTH_FLAG = 1 << SOUTH,
    WEST_FLAG  = 1 << WEST,
    NORTH_FLAG = 1 << NORTH
};


#endif // ACTION_SET_H
