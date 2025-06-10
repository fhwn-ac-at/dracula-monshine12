#include "sim.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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
    int num_of_rolls = 0;
    int overshots = 0;

    for (int i = 0; i < config->iterations; i++) {
        Node* current;
        int current_idx = -1;
        int sim_steps = 0;
        do {
            sim_steps++;
            int roll = roll_dice(config->dice_sides);
            num_of_rolls++;

            if (current_idx + roll >= num_fields && config->allow_overshoot) {
                // If roll overshoots goal and overshoot is allowed 
                // change roll to perfectly land on finish line (also helps to prefent idx out of bounds)
                roll = num_fields - 1 - current_idx; 
                overshots++;
            } else if (current_idx + roll >= num_fields && !config->allow_overshoot) {
                // If roll overshoots goal and overshoot isn't allowed try again
                continue;
            }
            current_idx += roll;
            current = board->start[current_idx];

            // Move directly to successor field if FieldType Snake or Ladder
            if (current->ft == SNAKE || current->ft == LADDER) {
                current = current->successors[0];
            } 

            if (current_idx == num_fields - 1 || sim_steps > config->max_simulation_steps) {
                // End current iteration
                break;
            }
        } while(1);
    }

    SimResults results = {
        .avg_rolls = num_of_rolls / config->iterations,
        .overshots = overshots,
        .shortest_num_rolls = 1
    };
    return results;
}

void print_sim_results(SimResults sim_results) {
    puts("========= Sim Results =========");
    printf("Avg. roll = %.2f\n", sim_results.avg_rolls);
    printf("Won through overshots = %d\n", sim_results.overshots);
    puts("===============================");
}