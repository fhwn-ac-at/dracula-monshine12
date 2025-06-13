#pragma once
#include "config_manager.h"
#include "game_board.h"

typedef struct {
    float avg_rolls;
    int overshots;
    int shortest_num_of_rolls;
    int aborted_iterations;
    int num_snakes;
    Transition snakes[MAX_SNAKES];
    int num_ladders;
    Transition ladders[MAX_SNAKES];
    int* shortest_roll_sequence;
    int max_simulation_steps;
    double elapsed_time;
} SimResults;

/**
 * @brief Runs a simulation of the board game based on the provided configuration.
 *
 * Simulates multiple iterations of the game, collecting statistics such as average rolls
 * to win, overshoots, ladder and snake usage, aborted iterations, and the shortest win sequence.
 *
 * The function supports both normal gameplay and overshoot rules, and handles ladders and snakes
 * as configured. Memory for shortest roll sequence is dynamically allocated and should be managed externally.
 *
 * @param board Pointer to the initialized `GameBoard` structure.
 * @param config Pointer to the `Config` structure containing simulation settings.
 * @return A populated `SimResults` structure containing simulation statistics.
 *
 * @warning Terminates the program with `exit(EXIT_FAILURE)` if memory allocation fails during the simulation.
 */
SimResults run_sim(GameBoard* board, Config* config);

/**
 * @brief Prints the results of a simulation in a readable format.
 *
 * Outputs simulation statistics such as:
 * - Total elapsed simulation time
 * - Average rolls to win (excluding overshoots)
 * - Number of overshot victories
 * - Aborted iterations due to step limits
 * - Shortest roll sequence and number of rolls
 * - Snake and ladder usage breakdown with percentages
 *
 * @param sim_results The `SimResults` structure containing the results to print.
 */
void print_sim_results(SimResults sim_results);
