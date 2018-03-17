#ifndef MAZE_H
#define MAZE_H


#include "action.h"
#include "location.h"
#include "maze_size.h"


struct node_list_t;

/**
 * Represents a maze.
 *
 * This struct contains the properties necessary to find a solution to the maze,
 * specifically a pointer to an array of sets of actions for every location in
 * the maze, along with the size of the maze and its start and end locations.
 */
struct maze_t
{
    struct action_set_pair_t* action_sets;
    struct maze_size_t size;
    struct location_t start;
    struct location_t end;
};

/**
 * Creates a maze with a given size, end and start.
 *
 * This function attempts to initialize all the properties of the given pointer
 * after allocating a section of memory for the actions, big enough to store the
 * actions for the entire maze.
 *
 * \param [out] maze
 *     A pointer to the maze variable that will store the maze.
 * \param [in]  size
 *     The size of the maze.
 * \param [in]  start
 *     The location of the start of the maze.
 * \param [in]  end
 *     The location of the end of the maze.
 *
 * \pre
 *     The pointer to the maze variable must not be NULL.
 * \pre
 *     The start and end locations must be within the maze.
 *
 * \returns
 *     -1 on failure, 0 on success.
 */
int make_maze(struct maze_t* maze, struct maze_size_t size, struct location_t start, struct location_t end);

/**
 * Sets the set of actions available at a given location in a maze.
 *
 * This function simply sets the set of actions available at the given location
 * in the maze by updating the correct entry in the action set array.
 *
 * \param [in,out] maze
 *     The maze variable containing the pointer to the action set array in which
 *     the set of actions is to be set.
 * \param [in]     action_set
 *     The set of actions to set.
 * \param [in]     location
 *     The location in the maze that the set of actions relates to.
 *
 * \pre
 *     The location must be within the maze.
 */
void set_action_set(struct maze_t maze, enum action_set_t action_set, struct location_t location);

/**
 * Gets the set of actions available at a given location in a maze.
 *
 * This function simply gets the set of actions available at the given location
 * in the maze by finding the correct entry in the action set array.
 *
 * \param [in] maze
 *     The maze containing the action set array to get the specified action
 *     from.
 * \param [in] location
 *     The location of the specified action.
 *
 * \pre
 *     The location must be within the maze.
 *
 * \returns
 *     The set of actions available at the given location.
 */
enum action_set_t get_action_set(struct maze_t maze, struct location_t location);

/**
 * Solves a given maze using A* search.
 *
 * This function attempts to find a path from the end of the given maze back to
 * the start, using A* search, which iteratively expands the next closest node
 * to the start location. Every expanded node is inserted into the given list in
 * order, such that the final node in the list will be the start of the maze, if
 * the search was successful.
 *
 * \see test_solve_maze()
 *
 * \param [out] list
 *     A pointer to the node list variable that will be used to store the
 *     explored nodes in the search, including the linked list of nodes that
 *     form the path.
 * \param [in]  maze
 *     The maze to solve.
 *
 * \pre
 *     The pointer to the node list variable must not be NULL.
 */
void solve_maze(struct node_list_t* list, struct maze_t maze);

/**
 * Tests the function solve_maze().
 *
 * This function runs through a series of test cases defined in the file
 * test.txt. The test cases are each a 5x5 maze, some of which have no solution.
 * The function asserts that for those with a solution, the solve_maze()
 * function provides a complete path from start to finish.
 */
void test_solve_maze(void);


#endif // MAZE_H
