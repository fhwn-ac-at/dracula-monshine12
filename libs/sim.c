#include "sim.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Generates a random number upto given dice sides
 * 
 * @param dice_sides Highest number to be generated = sides of given dice
 * @note This implementation isn't threadsafe
 */
int roll_dice(int dice_sides) {
    return rand() % dice_sides + 1;
}

SimResults run_sim(GameBoard* board, Config* config) {
    srand(time(NULL));
    int num_fields = board->rows * board->cols;
    int num_of_total_rolls = 0;

    SimResults results = {
        .avg_rolls = 0,
        .overshots = 0,
        .shortest_num_of_rolls = -1,
        .aborted_iterations = 0,
        .num_ladders = config->num_ladders,
        .num_snakes = config->num_snakes,
        .max_simulation_steps = config->max_simulation_steps
    };
    
    for (int i = 0; i < config->num_snakes; i++) {
        results.snakes[i] = config->snakes[i];
    }
    for (int i = 0; i < config->num_ladders; i++) {
        results.ladders[i] = config->ladders[i];
    }

    
    int* roll_sequence = calloc(config->max_simulation_steps, sizeof(int));
    for (int i = 0; i < config->iterations; i++) {
        Node* current;
        int current_idx = -1;
        int sim_steps = 0; // Is also used to measure number of rolls within one iteration
        do {
            sim_steps++;
            if (sim_steps >= config->max_simulation_steps - 1) {
                logm(DEBUG, "run_sim", "Reached maximum simulation steps within this iteration, will abort iteration.");
                results.aborted_iterations++;
                break;
            }
            int roll = roll_dice(config->dice_sides);
            roll_sequence[sim_steps - 1] = roll;
            num_of_total_rolls++;

            if (current_idx + roll >= num_fields && config->allow_overshoot) {
                // If roll overshoots goal and overshoot is allowed 
                // change roll to perfectly land on finish line (also helps to prefent idx out of bounds)
                roll = num_fields - 1 - current_idx; 
                results.overshots++;
            } else if (current_idx + roll >= num_fields && !config->allow_overshoot) {
                // If roll overshoots goal and overshoot isn't allowed try again
                continue;
            }
            current_idx += roll;
            current = board->start[current_idx];
            // printf("Rolled: %d now at idx = %d", roll, current_idx + 1);

            // Move directly to successor field if FieldType Snake or Ladder
            if (current->ft == SNAKE) {
                for (int j = 0; j < config->num_snakes; j++) {
                    if (results.snakes[j].start - 1 == current_idx) {
                        results.snakes[j].times_used++;
                        current_idx = config->snakes[j].end - 1;
                        // printf("Took snake now at idx = %d", current_idx + 1);
                        break;
                    }
                }
                current = current->successors[0];
            } else if (current->ft == LADDER) {
                for (int j = 0; j < config->num_ladders; j++) {
                    if (results.ladders[j].start - 1 == current_idx) {
                        results.ladders[j].times_used++;
                        current_idx = config->ladders[j].end - 1;
                        // printf("Took ladder now at idx = %d", current_idx + 1);
                        break;
                    }
                }
                current = current->successors[0];
            } 
            
            if (current_idx == num_fields - 1) {
                // Reached end of board
                if (results.shortest_num_of_rolls == -1 || sim_steps < results.shortest_num_of_rolls) {
                    results.shortest_num_of_rolls = sim_steps;
                    int* new_shortest_roll_sequence = calloc(config->max_simulation_steps, sizeof(int));
                    for (int i = 0; i < results.shortest_num_of_rolls; i++) {
                        new_shortest_roll_sequence[i] = roll_sequence[i];
                    }
                    results.shortest_roll_sequence = new_shortest_roll_sequence;
                }
                memset(roll_sequence, 0, config->max_simulation_steps * sizeof(int));
                break;
            }
            // printf("\n");
        } while(1);
    }
    free(roll_sequence);
    results.avg_rolls = num_of_total_rolls / config->iterations;
    return results;
}

void print_sim_results(SimResults sim_results) {
    puts("========= Sim Results =========");
    printf("Avg. roll = %.2f\n", sim_results.avg_rolls);
    printf("Won with overshots = %d\n", sim_results.overshots);
    printf("Aborted iterations due to reaching max sim steps = %d\n", sim_results.aborted_iterations);
    printf("Shortest amount of rolls to win = %d\n   with following roll sequence: ", sim_results.shortest_num_of_rolls);
    for (int i = 0; i < sim_results.shortest_num_of_rolls; i++) {
        printf("%d ", sim_results.shortest_roll_sequence[i]);
    }
    printf("\n");
    int total_ladder_usages = 0;
    int total_snake_usages = 0;

    for (int i = 0; i < sim_results.num_ladders; i++) total_ladder_usages += sim_results.ladders[i].times_used;
    for (int i = 0; i < sim_results.num_snakes; i++) total_snake_usages += sim_results.snakes[i].times_used;

    printf("Ladder usages:\n");
    for (int i = 0; i < sim_results.num_ladders; i++) {
        printf("  -) From field %d to %d: %d - %.2f%%\n", 
            sim_results.ladders[i].start, 
            sim_results.ladders[i].end, 
            sim_results.ladders[i].times_used,
            (double) sim_results.ladders[i].times_used / (double) total_ladder_usages * 100
        );
    }
    printf("Snake usages:\n");
    for (int i = 0; i < sim_results.num_snakes; i++) {
        printf("  -) From field %d to %d: %d - %.2f%%\n",
            sim_results.snakes[i].start, 
            sim_results.snakes[i].end,
            sim_results.snakes[i].times_used,
            (double) sim_results.snakes[i].times_used / (double) total_snake_usages * 100
        );
    }
    puts("===============================");
}