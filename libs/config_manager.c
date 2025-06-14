#include "config_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Checks for duplicate snake or ladder positions in the configuration.
 *
 * Ensures that the given start or end position does not already belong to an existing snake or ladder,
 * either as a start or an end point. Prevents overlapping or conflicting transitions.
 *
 * @param start The proposed start position of a new snake or ladder.
 * @param end The proposed end position of a new snake or ladder.
 * @param snake_idx Number of snakes already added to the configuration.
 * @param ladder_idx Number of ladders already added to the configuration.
 * @param config Pointer to the configuration data where snakes and ladders are stored.
 * @return 1 if a conflict exists; 0 otherwise.
 */
int check_for_existence(int start, int end, int snake_idx, int ladder_idx, Config* config) {
    // Check if there's a snake with given start/end position
    for (int i = 0; i < snake_idx; i++) {
        if (config->snakes[i].start == start || config->snakes[i].end == end || 
            config->snakes[i].start == end || config->snakes[i].end == start) {
            return 1;
        }
    }

    // Check if there's a ladder with given start/end position
    for (int i = 0; i < ladder_idx; i++) {
        if (config->ladders[i].start == start || config->ladders[i].end == end ||
            config->ladders[i].start == end || config->ladders[i].end == start) {
            return 1;
        }
    }

    return 0;
}

int parse_config_file(const char* filename, Config* config) {
    if (!config) {
        logm(ERROR, "parse_config_file", "Invalid Config (NULL pointer).");
        return 1;
    }

    // Set a few default values for the config
    config->iterations = 100;
    config->rows = 10;
    config->cols = 10;
    config->max_simulation_steps = 1000;
    config->allow_overshoot = 1;
    config->dice_sides = 6;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        logm(ERROR, "parse_config_file", "Encounterd error when trying to open given file, it might not exists!");
        return 1;
    }

    char* original_ptr = malloc(sizeof(char) * MAX_LINE_LENGTH);
    char* line = original_ptr;
    int parsing_snakes = 0, parsing_ladders = 0, snake_idx = 0, ladder_idx = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Strip comments and newlines
        char* comment = strchr(line, '#');
        if (comment) *comment = '\0';

        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        // Skip empty lines
        if (strlen(line) == 0) continue;

        // Trim whitespace
        while (isspace(*line)) line++;

        if (strncmp(line, "ITERATIONS=", 11) == 0) {
            int iterations = atoi(line + 11);
            if (iterations <= 0) {
                logm(ERROR, "parse_config_file", "Number of iterations must not be negative or zero. Check your config file.");
                return 1;
            }
            config->iterations = iterations;
        } else if (strncmp(line, "MAXSIMSTEPS=", 12) == 0) {
            int max_simulation_steps = atoi(line + 12);  
            if (max_simulation_steps <= 0) {
                logm(ERROR, "parse_config_file", "Number of maximum simulation steps must not be negative or zero. Check your config file.");
                return 1;
            }
            config->max_simulation_steps = max_simulation_steps;
        } else if (strncmp(line, "ROWS=", 5) == 0) {
            int rows = atoi(line + 5);
            if (rows <= 0) {
                logm(ERROR, "parse_config_file", "Number of rows must not be negative or zero, will now play with default number of rows (10).");
                config->rows = 10;
            } else {
                config->rows = rows;
            }
        } else if (strncmp(line, "COLS=", 5) == 0) {
            int cols = atoi(line + 5);
            if (cols <= 0) {
                logm(ERROR, "parse_config_file", "Number of cols must not be negative or zero, will now play with default number of cols (10).");
                config->cols = 10;
            } else {
                config->cols = cols;
            }
        } else if (strncmp(line, "DICE=", 5) == 0) {
            int dice_sides = atoi(line + 5);
            if (dice_sides <= 1) {
                logm(ERROR, "parse_config_file", "Dice needs to be atleast 2, will now play with default dice sides (6).");
                config->dice_sides = 6;
            } else {
                config->dice_sides = dice_sides;
            }
        } else if (strncmp(line, "ALLOW_OVERSHOOT=", 16) == 0) {
            config->allow_overshoot = (strncmp(line + 16, "true", 4) == 0);
        } else if (strncmp(line, "SNAKES=", 7) == 0) {
            int num_snakes = atoi(line + 7);
            if (num_snakes < 0) {
                logm(ERROR, "parse_config_file", "Number of snakes must not be negative. Check your config file.");
                return 1;
            }
            config->num_snakes = num_snakes;
            parsing_snakes = 1;
            parsing_ladders = 0;
        } else if (strncmp(line, "LADDERS=", 8) == 0) {
            int num_ladders = atoi(line + 8);
            if (num_ladders < 0) {
                logm(ERROR, "parse_config_file", "Number of ladders must not be negative. Check your config file.");
                return 1;
            }
            config->num_ladders = num_ladders;
            parsing_snakes = 0;
            parsing_ladders = 1;
        } else {
            int start, end;
            if (sscanf(line, "%d:%d", &start, &end) == 2) {
                if (start == end) {
                    logm(INFO, "parse_config_file", "No snake or ladder should start or end on the same square as itself. Therefore it will not be included on the board.");
                } else if (start == config->cols * config->rows) { // rows and cols are 1-based 
                    logm(INFO, "parse_config_file", "No snake or ladder should start at the last square. It will not be included on the board.");
                } else if (check_for_existence(start, end, snake_idx, ladder_idx, config)){
                    logm(INFO, "parse_config_file", "No snake or ladder should start or end on the same square as any other snake or ladder. It will not be included on the board.");
                } else if (start <= 0 || start > config->cols * config->rows || end <= 0 || end > config->cols * config->rows) {
                    logm(INFO, "parse_config_file", "No snake or ladder should reach out of bound of the game field. It will not be included on the board.");
                } else if (parsing_snakes && start < end) {
                    logm(INFO, "parse_config_file", "Snakes have to start with a larger value than it ends with otherwise it would be a ladder. It will not be included on the board.");
                } else if (parsing_ladders && start > end) {
                    logm(INFO, "parse_config_file", "Ladders have to start with a smaller value than it ends with otherwise it would be a snake. It will not be included on the board.");
                } else {
                    // If no error is detected with given ladder/snake positions included them in the board
                    if (parsing_snakes && snake_idx < config->num_snakes && snake_idx < MAX_SNAKES) {                    
                        config->snakes[snake_idx].start = start;
                        config->snakes[snake_idx].end = end;
                        config->snakes[snake_idx].times_used = 0;
                        snake_idx++;
                    } else if (parsing_ladders && ladder_idx < config->num_ladders && ladder_idx < MAX_LADDERS) {
                        config->ladders[ladder_idx].start = start;
                        config->ladders[ladder_idx].end = end;
                        config->ladders[ladder_idx].times_used = 0;
                        ladder_idx++;
                    }
                }                
            }
        }
    }
    free(original_ptr);
    fclose(file);
    return 0;
}

void print_config(Config* config) {
    if (!config) {
        logm(ERROR, "print_config", "Invalid Config (NULL pointer).");
        return;
    }
    
    printf("===========================\n");
    printf("Simulation Configuration:\n");
    printf("  Iterations      : %d\n", config->iterations);
    printf("  Max Sim Steps   : %d\n", config->max_simulation_steps);
    printf("Board Configuration:\n");
    printf("  Grid Size       : %d x %d\n", config->rows, config->cols);
    printf("  Dice Sides      : %d\n", config->dice_sides);
    printf("  Allow Overshoot : %s\n", config->allow_overshoot ? "Yes" : "No");

    printf("\n--- Snakes (%d) ---\n", config->num_snakes);
    if (config->num_snakes == 0) {
        printf("(None)\n");
    } else {
        for (int i = 0; i < config->num_snakes; ++i) {
            printf("  Snake %2d: %3d -> %3d\n", i + 1,
                   config->snakes[i].start, config->snakes[i].end);
        }
    }

    printf("\n--- Ladders (%d) ---\n", config->num_ladders);
    if (config->num_ladders == 0) {
        printf("(None)\n");
    } else {
        for (int i = 0; i < config->num_ladders; ++i) {
            printf("  Ladder %2d: %3d -> %3d\n", i + 1,
                   config->ladders[i].start, config->ladders[i].end);
        }
    }

    printf("===========================\n");
}