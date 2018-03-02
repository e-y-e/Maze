#ifndef NODE_H
#define NODE_H


#include "point.h"

#include <stddef.h>


/**
 * Represents a node in a maze solving tree.
 */
struct node_t
{
    struct point_t point;
    struct node_t* parent;
    size_t path_cost;
};


#endif // NODE_H
