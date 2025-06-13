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
 * @brief Simulates the board game and collects statistics.
 *
 * Runs the simulation for a number of iterations based on the provided configuration.
 * Collects statistics such as average rolls to win, overshoots, snake/ladder usage, aborted iterations,
 * and the shortest roll sequence. Memory for the results and shortest roll sequence is dynamically allocated.
 *
 * @param board Pointer to an initialized GameBoard.
 * @param config Pointer to the simulation configuration.
 * @return Pointer to a dynamically allocated SimResults structure, or NULL if allocation fails.
 *
 * @note The caller must free the returned SimResults (and its shortest_roll_sequence if set).
 */
SimResults* run_sim(GameBoard* board, Config* config);

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
 * @param results A ptr to `SimResults` structure containing the results to print.
 */
void print_sim_results(SimResults* results);
