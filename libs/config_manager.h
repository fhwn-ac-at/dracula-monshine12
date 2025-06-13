#pragma once
#include "logger.h"
#define MAX_SNAKES 100
#define MAX_LADDERS 100
#define MAX_LINE_LENGTH 256

typedef struct {
    int start;
    int end;
    int times_used;
} Transition;

typedef struct {
    int iterations;
    int max_simulation_steps;

    int rows;
    int cols;
    int dice_sides;
    int allow_overshoot;

    int num_snakes;
    Transition snakes[MAX_SNAKES];

    int num_ladders;
    Transition ladders[MAX_SNAKES];
} Config;

/**
 * @brief Parses the game configuration from a file into a Config structure.
 *
 * Reads a configuration file line-by-line to initialize a `Config` struct with game parameters
 * such as board size, dice properties, and snakes/ladders definitions. Performs extensive validation
 * and logs errors, warnings, and info messages accordingly.
 *
 * Configuration keys supported:
 * - ITERATIONS (must be > 0)
 * - MAXSIMSTEPS (must be > 0)
 * - ROWS (must be > 0)
 * - COLS (must be > 0)
 * - DICE (must be ≥ 2, otherwise defaults to 6 with a warning)
 * - ALLOW_OVERSHOOT (true/false)
 * - SNAKES= followed by snake definitions (format: `start:end`)
 * - LADDERS= followed by ladder definitions (format: `start:end`)
 *
 * @param filename Path to the configuration file.
 * @param config Pointer to a Config structure that will be populated.
 *
 * @return int Returns `0` on success, `1` on failure (e.g., invalid input, file error).
 *
 * @note Lines beginning with '#' are treated as comments.
 * @note Whitespace is ignored; values are trimmed and validated.
 * @note Snakes must start at a higher square than they end; ladders the opposite.
 * @note Duplicate or overlapping snake/ladder positions are skipped with an info message.
 */
int parse_config_file(const char* filename, Config* config);

/**
 * @brief Prints the contents of a Config structure in a human-readable format.
 *
 * Displays simulation settings (e.g. iterations, max steps, dice sides), board configuration,
 * and a list of defined snakes and ladders.
 *
 * @param config Pointer to a `Config` structure to print.
 */
void print_config(const Config* config);

