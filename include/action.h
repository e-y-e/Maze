#ifndef ACTION_H
#define ACTION_H


/**
 * Represents an action available at a node in a maze.
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
