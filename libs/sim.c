#include "sim.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Rolls a die and returns a random value between 1 and the number of sides.
 *
 * This function generates a pseudo-random integer between 1 and `dice_sides`, simulating
 * the roll of a die with the specified number of sides.
 *
 * @param dice_sides The number of sides on the die. Must be greater than 0.
 * @return A pseudo-random number in the range [1, dice_sides].
 *
 * @note This implementation uses `rand()` and is not thread-safe.
 */
int roll_dice(int dice_sides) {
    return rand() % dice_sides + 1;
}

SimResults run_sim(GameBoard* board, Config* config) {
    srand(time(NULL));

    const int num_fields = board->rows * board->cols;
    int total_rolls = 0;

    SimResults results = {
        .avg_rolls = 0,
        .overshots = 0,
        .shortest_num_of_rolls = -1,
        .aborted_iterations = 0,
        .num_ladders = config->num_ladders,
        .num_snakes = config->num_snakes,
        .max_simulation_steps = config->max_simulation_steps,
        .elapsed_time = (double) clock() / CLOCKS_PER_SEC
    };
    
    memcpy(results.snakes, config->snakes, sizeof(Transition) * config->num_snakes);
    memcpy(results.ladders, config->ladders, sizeof(Transition) * config->num_ladders);
    
    int* roll_sequence = calloc(config->max_simulation_steps, sizeof(int));
    if (!roll_sequence) {
        logm(ERROR, "run_sim", "Memory allocation failed for roll_sequence.");
        free_board(board);
        free(config);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < config->iterations; i++) {
        int sim_steps = 0;
        int current_idx = -1;

        while (1) {
            if (++sim_steps >= config->max_simulation_steps) {
                results.aborted_iterations++;
                break;
            }

            int roll = roll_dice(config->dice_sides);
            int target_idx = current_idx + roll;

            // Overshoot handling
            if (target_idx >= num_fields) {
                if (config->allow_overshoot) {
                    roll = num_fields - 1 - current_idx;
                    results.overshots++;
                    target_idx = current_idx + roll;
                } else {
                    // Retry this roll
                    continue;
                }
            }

            roll_sequence[sim_steps - 1] = roll;
            total_rolls++;
            current_idx = target_idx;

            Node* current = board->start[current_idx];

            // Handle Snake
            if (current->ft == SNAKE) {
                for (int j = 0; j < config->num_snakes; j++) {
                    if (results.snakes[j].start - 1 == current_idx) {
                        results.snakes[j].times_used++;
                        current_idx = results.snakes[j].end - 1;
                        break;
                    }
                }
            } 
            // Handle Ladder
            else if (current->ft == LADDER) {
                for (int j = 0; j < config->num_ladders; j++) {
                    if (results.ladders[j].start - 1 == current_idx) {
                        results.ladders[j].times_used++;
                        current_idx = results.ladders[j].end - 1;
                        break;
                    }
                }
            }

            if (current_idx == num_fields - 1) {
                // Reached end of board
                if (results.shortest_num_of_rolls == -1 || sim_steps < results.shortest_num_of_rolls) {
                    results.shortest_num_of_rolls = sim_steps;
                    int* seq_copy = calloc(config->max_simulation_steps, sizeof(int));
                    if (!seq_copy) {
                        logm(ERROR, "run_sim", "Memory allocation failed for seq_copy.");
                        free_board(board);
                        free(config);
                        free(roll_sequence);
                        exit(EXIT_FAILURE);
                    }
                    memcpy(seq_copy, roll_sequence, sizeof(int) * sim_steps);
                    results.shortest_roll_sequence = seq_copy;
                }

                // Reset roll sequence for next iteration
                memset(roll_sequence, 0, sizeof(int) * config->max_simulation_steps);
                break;
            }
        }
    }
    free(roll_sequence);

    int completed_iterations = config->iterations - results.aborted_iterations;
    results.avg_rolls = (completed_iterations > 0) ? (double) total_rolls / completed_iterations : 0.0;
    results.elapsed_time = ((double) clock() / CLOCKS_PER_SEC) - results.elapsed_time;

    return results;
}

void print_sim_results(SimResults sim_results) {
    puts("\n=========== Simulation Results ===========\n");

    printf("Total simulation time:             %.3f seconds\n", sim_results.elapsed_time);
    printf("Average roll to win (-overshots):  %.2f\n", sim_results.avg_rolls);
    printf("Games won with overshots:          %d\n", sim_results.overshots);
    printf("Aborted due to max sim steps:      %d\n", sim_results.aborted_iterations);

    printf("\nShortest win:\n");
    printf("  - Rolls needed:                  %d\n", sim_results.shortest_num_of_rolls);
    printf("  - Roll sequence:                 ");
    for (int i = 0; i < sim_results.shortest_num_of_rolls; i++) {
        printf("%d ", sim_results.shortest_roll_sequence[i]);
    }
    printf("\n");

    int total_ladder_usages = 0;
    int total_snake_usages = 0;

    for (int i = 0; i < sim_results.num_ladders; i++)
        total_ladder_usages += sim_results.ladders[i].times_used;

    for (int i = 0; i < sim_results.num_snakes; i++)
        total_snake_usages += sim_results.snakes[i].times_used;

    printf("\nLadder Usage Statistics (Total: %d):\n", total_ladder_usages);
    for (int i = 0; i < sim_results.num_ladders; i++) {
        double percent = (total_ladder_usages > 0) ?
            (double)sim_results.ladders[i].times_used / total_ladder_usages * 100 : 0.0;
        printf("  - From %3d to %3d:  %4d uses (%.2f%%)\n",
            sim_results.ladders[i].start,
            sim_results.ladders[i].end,
            sim_results.ladders[i].times_used,
            percent);
    }

    printf("\nSnake Usage Statistics (Total: %d):\n", total_snake_usages);
    for (int i = 0; i < sim_results.num_snakes; i++) {
        double percent = (total_snake_usages > 0) ?
            (double)sim_results.snakes[i].times_used / total_snake_usages * 100 : 0.0;
        printf("  - From %3d to %3d:  %4d uses (%.2f%%)\n",
            sim_results.snakes[i].start,
            sim_results.snakes[i].end,
            sim_results.snakes[i].times_used,
            percent);
    }

    puts("\n==========================================\n");
}
