#include "location.h"
#include "maze_size.h"
#include "action.h"
#include "node.h"
#include "node_list.h"
#include "maze.h"
#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef TEST

static void test_location_distance()
{
    // Test simple distances.
    assert(location_distance((struct location_t) {.row = 0, .column = 0},
                             (struct location_t) {.row = 0, .column = 0}) == 0);
    assert(location_distance((struct location_t) {.row = 5, .column = 1},
                             (struct location_t) {.row = 5, .column = 1}) == 0);
    assert(location_distance((struct location_t) {.row = 0, .column = 0},
                             (struct location_t) {.row = 1, .column = 0}) == 1);
    assert(location_distance((struct location_t) {.row = 0, .column = 0},
                             (struct location_t) {.row = 0, .column = 1}) == 1);
    assert(location_distance((struct location_t) {.row = 0, .column = 0},
                             (struct location_t) {.row = 5, .column = 0}) == 5);
    assert(location_distance((struct location_t) {.row = 1, .column = 0},
                             (struct location_t) {.row = 5, .column = 0}) == 4);
    assert(location_distance((struct location_t) {.row = 5, .column = 0},
                             (struct location_t) {.row = 1, .column = 0}) == 4);


    // Test Euclidean distances (pythagorean triples).
    assert(location_distance((struct location_t) {.row =  3, .column =   0},
                             (struct location_t) {.row =  0, .column =   4}) ==   5);
    assert(location_distance((struct location_t) {.row =  5, .column =   0},
                             (struct location_t) {.row =  0, .column =  12}) ==  13);
    assert(location_distance((struct location_t) {.row = 68, .column =   0},
                             (struct location_t) {.row =  0, .column = 285}) == 293);


    // Test typical distances (only ensuring an upper limit).
    assert(location_distance((struct location_t) {.row =  1, .column =   5},
                             (struct location_t) {.row =  5, .column =   1}) <=   7);
    assert(location_distance((struct location_t) {.row =  3, .column =  10},
                             (struct location_t) {.row = 25, .column =   4}) <=  23);
    assert(location_distance((struct location_t) {.row = 20, .column =   0},
                             (struct location_t) {.row =  0, .column = 132}) <= 134);
    assert(location_distance((struct location_t) {.row =  1, .column =   1},
                             (struct location_t) {.row =  9, .column =  18}) <=  19);


    // Test extreme distances.
    assert(location_distance((struct location_t) {.row =        0, .column =     0},
                             (struct location_t) {.row =        0, .column = 65536}) ==    65536);
    assert(location_distance((struct location_t) {.row =     4096, .column =     0},
                             (struct location_t) {.row =        0, .column = 65536}) <=    65664);
    assert(location_distance((struct location_t) {.row = 16777216, .column =     0},
                             (struct location_t) {.row =        0, .column =  4096}) <= 16777217);
}

static void test_location_equal()
{
    // Test equality.
    assert(location_equal((struct location_t) {.row = 0, .column = 0},
                          (struct location_t) {.row = 0, .column = 0}));
    assert(location_equal((struct location_t) {.row = 5, .column = 1},
                          (struct location_t) {.row = 5, .column = 1}));

    // Test inequality.
    assert(!location_equal((struct location_t) {.row =    5, .column =     1},
                           (struct location_t) {.row =    0, .column =     0}));
    assert(!location_equal((struct location_t) {.row = 4096, .column =     0},
                           (struct location_t) {.row =    0, .column = 65536}));
}

static void test_check_location()
{
    // Test that the maze with zero dimensions cannot contain any locations.
    assert(!check_location((struct maze_size_t) {.rows = 0, .columns = 0},
                           (struct location_t)  {.row  = 0, .column  = 0}));
    assert(!check_location((struct maze_size_t) {.rows = 0, .columns = 0},
                           (struct location_t)  {.row  = 4, .column  = 5}));

    // Test that the location 0, 0 is within any maze with non-zero dimensions.
    assert( check_location((struct maze_size_t) {.rows =    1, .columns =     1},
                           (struct location_t)  {.row  =    0, .column  =     0}));
    assert( check_location((struct maze_size_t) {.rows =    4, .columns =     5},
                           (struct location_t)  {.row  =    0, .column  =     0}));
    assert( check_location((struct maze_size_t) {.rows = 4096, .columns = 65536},
                           (struct location_t)  {.row  =    0, .column  =     0}));

    // Test some boundary cases.
    assert( check_location((struct maze_size_t) {.rows =   1, .columns =   1},
                           (struct location_t)  {.row  =   0, .column  =   0}));
    assert(!check_location((struct maze_size_t) {.rows =   1, .columns =   1},
                           (struct location_t)  {.row  =   0, .column  =   1}));
    assert( check_location((struct maze_size_t) {.rows = 256, .columns = 256},
                           (struct location_t)  {.row  = 255, .column  =   0}));
    assert(!check_location((struct maze_size_t) {.rows = 256, .columns = 256},
                           (struct location_t)  {.row  = 256, .column  =   0}));
    assert( check_location((struct maze_size_t) {.rows = 256, .columns = 256},
                           (struct location_t)  {.row  = 255, .column  = 255}));
    assert(!check_location((struct maze_size_t) {.rows =   1, .columns =   1},
                           (struct location_t)  {.row  = 256, .column  = 256}));
}


static void test_action_result()
{
    // Test the four cardinal directions.
    assert(location_equal(action_result((struct location_t) {.row = 1, .column = 1}, EAST),
                          (struct location_t) {.row = 1, .column = 2}));
    assert(location_equal(action_result((struct location_t) {.row = 1, .column = 1}, SOUTH),
                          (struct location_t) {.row = 2, .column = 1}));
    assert(location_equal(action_result((struct location_t) {.row = 1, .column = 1}, WEST),
                          (struct location_t) {.row = 1, .column = 0}));
    assert(location_equal(action_result((struct location_t) {.row = 1, .column = 1}, NORTH),
                          (struct location_t) {.row = 0, .column = 1}));
}

static void test_action_taken()
{
    enum action_t action;

    // Test the four cardinal directions.
    assert(action_taken(&action, (struct location_t) {.row = 1, .column = 1},
                                 (struct location_t) {.row = 1, .column = 2}) == 0);
    assert(action == EAST);
    assert(action_taken(&action, (struct location_t) {.row = 1, .column = 1},
                                 (struct location_t) {.row = 2, .column = 1}) == 0);
    assert(action == SOUTH);
    assert(action_taken(&action, (struct location_t) {.row = 1, .column = 1},
                                 (struct location_t) {.row = 1, .column = 0}) == 0);
    assert(action == WEST);
    assert(action_taken(&action, (struct location_t) {.row = 1, .column = 1},
                                 (struct location_t) {.row = 0, .column = 1}) == 0);
    assert(action == NORTH);

    // Test an impossible action.
    assert(action_taken(&action, (struct location_t) {.row = 1, .column = 1},
                                 (struct location_t) {.row = 2, .column = 2}) != 0);
}

static void test_node_list()
{
    struct node_list_t node_list;

    // Test make_list with various initial capacities.
    assert(make_list(&node_list, 1) == 0);
    assert(node_list.capacity == 1);
    assert(make_list(&node_list, 4) == 0);
    assert(node_list.capacity == 4);
    assert(make_list(&node_list, 16) == 0);
    assert(node_list.capacity == 16);
    assert(make_list(&node_list, 64) == 0);
    assert(node_list.capacity == 64);

    // Test make_list with an extreme initial capacity.
    assert(make_list(&node_list, 16777216) == 0);
    assert(node_list.capacity == 16777216);

    make_list(&node_list, 64);

    // Test resize_list with various new capacities.
    assert(resize_list(&node_list, 64) == 0);
    assert(node_list.capacity == 64);
    assert(resize_list(&node_list, 1) == 0);
    assert(node_list.capacity == 1);
    assert(resize_list(&node_list, 4) == 0);
    assert(node_list.capacity == 4);
    assert(resize_list(&node_list, 16) == 0);
    assert(node_list.capacity == 16);

    // Test insert_node at various indexes.
    size_t indexes[8] = {0, 1, 2, 3, 0, 2, 4, 6};

    struct node_t nodes[8];

    for (size_t i = 0; i < 8; i++)
    {
        nodes[i] = (struct node_t)
        {
            .location = {.row = i, .column = 0},
            .parent = NULL
        };
        assert(insert_node(&node_list, &nodes[i], indexes[i]) == 0);
    }

    size_t order[8] = {4, 0, 5, 1, 6, 2, 7, 3};

    // Check that the nodes were inserted in the correct order.
    for (size_t i = 0; i < 8; i++)
    {
        assert(get_node(&node_list, i)->location.row == order[i]);
    }

    // Test remove_node with various indexes.
    for (size_t i = 0; i < 4; i++)
    {
        remove_node(&node_list, 0);
    }

    // Check that the node list contains the correct remaining values.
    for (size_t i = 0; i < 4; i++)
    {
        assert(get_node(&node_list, i)->location.row == order[i + 4]);
    }
}

static void test_solve_maze()
{
    static char* maze_files[4] =
    {
        "tests/maze1.txt",
        "tests/maze2.txt",
        "tests/maze3.txt",
        "tests/maze4.txt"
    };

    static char* solution_files[4] =
    {
        "tests/solution1.txt",
        "tests/solution2.txt",
        "tests/solution3.txt",
        "tests/solution4.txt"
    };

    static char* results_files[4] =
    {
        "tests/results1.txt",
        "tests/results2.txt",
        "tests/results3.txt",
        "tests/results4.txt"
    };

    char solution[1024] = "";
    char result[1024] = "";

    for (size_t i = 0; i < 4; i++)
    {
        FILE* fp = fopen(maze_files[i], "r");
        assert(fp != NULL);

        // Read in a maze from the maze file.
        struct maze_t maze;
        assert(read_maze(&maze, fp) == 0);

        fclose(fp);

        // Read the solution from the solution file.
        fp = fopen(solution_files[i], "r");
        assert(fp != NULL);

        assert(fgets(solution, 65536, fp) != NULL);

        fclose(fp);

        printf("HELLO!\n");

        // Solve the maze.
        struct node_list_t explored;
        printf("%p\n", (void*) &explored);
        assert(make_list(&explored, 65536) == 0);
        solve_maze(&explored, maze);

        // Write the result to the result file.
        struct node_t* start = get_node(&explored, explored.length - 1);
        fp = fopen(results_files[i], "w+");
        assert(fp != NULL);

        write_path(start, fp);

        fclose(fp);
        resize_list(&explored, 0);

        // Check that the result is equal to the solution.
        fp = fopen(results_files[i], "r");
        assert(fp != NULL);

        assert(fgets(result, 65536, fp) != NULL);

        fclose(fp);

        assert(strncmp(solution, result, 65536) == 0);
    }
}

int main()
{
    test_location_distance();
    test_location_equal();
    test_check_location();
    test_action_result();
    test_action_taken();
    test_node_list();
    test_solve_maze();
    return 0;
}

#endif // TEST
