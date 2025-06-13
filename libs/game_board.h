#pragma once
#include "logger.h"
#include "config_manager.h"

typedef enum {
    SNAKE, LADDER, DEFAULT
} FieldType;

typedef struct Node {
    FieldType ft;
    struct Node** successors;
} Node;

typedef struct {
    int rows;
    int cols;
    Node** start;
} GameBoard;

/**
 * @brief Creates and initializes a new game board based on the given configuration.
 *
 * This function allocates and initializes a `GameBoard` structure with a grid of `Node` elements
 * representing the fields of the game. Each node is initialized with its appropriate type
 * (`DEFAULT`, `SNAKE`, or `LADDER`), and sets up successor pointers for dice moves, ladders, or snakes.
 *
 * @param config Pointer to the configuration structure containing board dimensions, dice sides, snakes, and ladders.
 * @return Pointer to the dynamically allocated `GameBoard`. Must be freed using `free_board`.
 */
GameBoard* create_game_board(Config* config);

/**
 * @brief Frees the memory associated with a game board.
 *
 * This function safely deallocates all memory allocated by `create_game_board`,
 * including all `Node` structures and their successor arrays.
 *
 * @param board Pointer to the `GameBoard` to be deallocated. If NULL, the function does nothing.
 */
void free_board(GameBoard* board);

/**
 * @brief Prints a formatted representation of the game board to the console.
 *
 * Displays each field in the board, indicating whether it is a normal node (`N`),
 * a snake (`S -> X`), or a ladder (`L -> X`) and its corresponding destination.
 * Output is formatted as a grid according to the board's row and column configuration.
 *
 * @param board Pointer to the `GameBoard` to be printed. If NULL or uninitialized, the function does nothing.
 */
void print_game_board(GameBoard* board);
