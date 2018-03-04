#ifndef ACTION_H
#define ACTION_H


/**
 * Represents the action available at a node in a maze.
 *
 * This enumerated type provides representations for the directions in which a
 * node can be expanded. These representations allow multiple directions to be
 * combined using simple bitwise operations.
 */
enum action_t
{
    NORTH = (1 << 3),
    WEST  = (1 << 2),
    SOUTH = (1 << 1),
    EAST  = (1 << 0),
    NONE  = 0
};


#endif // ACTION_H
