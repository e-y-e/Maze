#ifndef ACTION_PAIR_H
#define ACTION_PAIR_H


#include "action.h"


#pragma pack(push)
#pragma pack(1)

/**
 * Combines two action types into a single byte-aligned struct.
 */
struct action_pair_t
{
    enum action_t a : 4;
    enum action_t b : 4;
};

#pragma pack(pop)


#endif // ACTION_PAIR_H
