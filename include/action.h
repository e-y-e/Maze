#ifndef ACTION_H
#define ACTION_H


struct location_t;

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
    EAST  = (1 << 0)
};

/**
 * Calculates the resulting location when a given action is taken at a given
 * location.
 *
 * This function simply calculates what the resulting location would be when the
 * given action is taken by adding or subtracting a unit length on the relevant
 * axis from the given location. Note that the action should only represent one
 * direction, otherwise the behaviour of this function is to return the original
 * location.
 *
 * \param [in] location
 *     The original location before the action is taken.
 * \param [in] action
 *     The action to be taken.
 *
 * \returns
 *     The resulting location when the given action is taken at the given
 *     location.
 */
struct location_t action_result(struct location_t location, enum action_t action);


#endif // ACTION_H
