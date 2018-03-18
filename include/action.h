#ifndef ACTION_H
#define ACTION_H


struct location_t;

/**
 * Represents a single action available at a node in a maze.
 *
 * This enumerated type provides representations for the directions in which a
 * node can be expanded.
 */
enum action_t
{
    EAST  = 0,
    SOUTH = 1,
    WEST  = 2,
    NORTH = 3
};

/**
 * Calculates the resulting location when a given action is taken at a given
 * location.
 *
 * This function simply calculates what the resulting location would be when the
 * given action is taken by adding or subtracting a unit length on the relevant
 * axis from the given location.
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
