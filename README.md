# Maze

A maze solving program implemented in pure C for EE20084 Structured Programming.


## Design

The overall structure of the maze solver is based around using a search
algorithm to determine how to reach the end location in a given maze from a
given start location. To do this, a central data type (maze_t) is used to store
the start and end locations of a maze along with the list of actions available
from each location. To work through the maze, a node data type (node_t) is used
which stores its location along with the parent node it was generated from. A
list type (node_list_t) is used to store the nodes such that all explored nodes
can be retained. To represent the action available from each location, an
enumerated type (#action_t) is used which consists of the cardinal directions in
which a node can be expanded.

These data types are employed in the search algorithm, in which a list of all
explored nodes is kept along with a list of all candidate nodes which are
iteratively expanded based on the best node to expand next. Once a solution has
been found, the list of explored nodes will contain the linked list of nodes
leading from the start to the end location, along with all other nodes that were
expanded along the way. The search algorithm is implemented in the solve_maze()
function.
