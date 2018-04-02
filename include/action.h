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

/**
 * Determines the action taken to move from one given location to another.
 *
 * This function simply uses the relative position of the second location from
 * the first to determine which action was taken to reach the second location
 * from the first.
 *
 * \param [out] action
 *     A pointer to the variable that will hold the action.
 * \param [in] a
 *     The first of the two locations.
 * \param [in] b
 *     The second of the two locations.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
int action_taken(enum action_t* action, struct location_t a, struct location_t b);


#endif // ACTION_H
