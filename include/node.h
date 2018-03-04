#ifndef NODE_H
#define NODE_H


#include "point.h"

#include <stddef.h>


/**
 * Represents a node in a maze.
 *
 * This struct contains the information necessary to construct a node in the
 * search tree for a maze problem, that can then be solved with a search
 * algorithm. As this node is designed for use in an A* search algorithm, it
 * also needs to store the total path cost of the node as an additional field.
 */
struct node_t
{
    struct point_t point;
    struct node_t* parent;
    size_t path_cost;
};


#endif // NODE_H
