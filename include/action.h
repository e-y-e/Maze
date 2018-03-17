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

#pragma pack(push)
#pragma pack(1)

/**
 * Combines two action set types into a single byte-aligned struct.
 *
 * This struct utilises bitfields to combine two action sets which each only
 * require 4 bits to represent. To avoid the compiler padding the representation
 * of this struct to the alignment boundary (usually 4 bytes), compiler
 * directives must be utilised.
 */
struct action_set_pair_t
{
    enum action_set_t a : 4;
    enum action_set_t b : 4;
};

#pragma pack(pop)


#endif // ACTION_H
