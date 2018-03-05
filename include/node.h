#ifndef NODE_H
#define NODE_H


#include "location.h"


/**
 * Represents a node in a maze.
 *
 * This struct contains the information necessary to construct a node in the
 * search tree for a maze problem, that can then be solved with a search
 * algorithm. Since the path needs to be reconstructed via the final node
 * visited in the search algorithm, each node stores a pointer to its parent
 * node.
 */
struct node_t
{
    struct location_t location;
    struct node_t* parent;
};


#endif // NODE_H
