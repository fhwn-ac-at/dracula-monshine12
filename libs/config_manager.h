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
 * @brief Parses a configuration file to populate a Config structure.
 *
 * Reads and validates simulation and board parameters from a file, including grid size, number of dice sides,
 * number of snakes and ladders, and their respective positions. Also performs checks to ensure the validity of
 * snake and ladder definitions.
 *
 * Supported fields in the config file include:
 * - ITERATIONS
 * - MAXSIMSTEPS
 * - ROWS
 * - COLS
 * - DICE
 * - ALLOW_OVERSHOOT
 * - SNAKES
 * - LADDERS
 * - Snake and ladder transitions (e.g. `23:8` or `4:17`)
 *
 * @param filename Path to the configuration file.
 * @param config Pointer to the `Config` structure to populate.
 *
 * @warning Terminates the program using `exit(EXIT_FAILURE)` on file access or validation errors.
 */
void parse_config_file(const char* filename, Config* config);

/**
 * @brief Prints the contents of a Config structure in a human-readable format.
 *
 * Displays simulation settings (e.g. iterations, max steps, dice sides), board configuration,
 * and a list of defined snakes and ladders.
 *
 * @param config Pointer to a `Config` structure to print.
 */
void print_config(const Config* config);

