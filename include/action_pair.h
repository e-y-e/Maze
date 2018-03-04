#ifndef ACTION_PAIR_H
#define ACTION_PAIR_H


#include "action.h"


#pragma pack(push)
#pragma pack(1)

/**
 * Combines two action types into a single byte-aligned struct.
 *
 * This struct utilises bitfields to combine two actions which each only require
 * 4 bits to represent. To avoid the compiler padding the representation of this
 * struct to the alignment boundary (usually 4 bytes), compiler directives must
 * be utilised.
 */
struct action_pair_t
{
    enum action_t a : 4;
    enum action_t b : 4;
};

#pragma pack(pop)


#endif // ACTION_PAIR_H
