#include "config_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void parse_config_file(const char* filename, BoardConfig* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        logm(ERROR, "parse_config_file", "Encounterd error when trying to open given file, it might not exists!");
        free(config);
        exit(EXIT_FAILURE);
    }

    char* original_ptr = malloc(sizeof(char) * MAX_LINE_LENGTH) ;
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

        if (strncmp(line, "ROWS=", 5) == 0) {
            config->rows = atoi(line + 5);
        } else if (strncmp(line, "COLS=", 5) == 0) {
            config->cols = atoi(line + 5);
        } else if (strncmp(line, "DICE=", 5) == 0) {
            config->dice_sides = atoi(line + 5);
        } else if (strncmp(line, "ALLOW_OVERSHOOT=", 16) == 0) {
            config->allow_overshoot = (strncmp(line + 16, "true", 4) == 0);
        } else if (strncmp(line, "SNAKES=", 7) == 0) {
            config->num_snakes = atoi(line + 7);
            parsing_snakes = 1;
            parsing_ladders = 0;
        } else if (strncmp(line, "LADDERS=", 8) == 0) {
            config->num_ladders = atoi(line + 8);
            parsing_snakes = 0;
            parsing_ladders = 1;
        } else {
            int start, end;
            if (sscanf(line, "%d:%d", &start, &end) == 2) {
                if (parsing_snakes && snake_idx < config->num_snakes && snake_idx < MAX_SNAKES) {                    
                    config->snakes[snake_idx].start = start;
                    config->snakes[snake_idx].end = end;
                    snake_idx++;
                } else if (parsing_ladders && ladder_idx < config->num_ladders && ladder_idx < MAX_LADDERS) {
                    config->ladders[ladder_idx].start = start;
                    config->ladders[ladder_idx].end = end;
                    ladder_idx++;
                }
            }
        }
    }
    free(original_ptr);
    fclose(file);
}

void print_board_config(const BoardConfig* config) {
    if (!config) {
        printf("Invalid BoardConfig (NULL pointer).\n");
        return;
    }

    printf("=== Board Configuration ===\n");
    printf("Grid Size       : %d x %d\n", config->rows, config->cols);
    printf("Dice Sides      : %d\n", config->dice_sides);
    printf("Allow Overshoot : %s\n", config->allow_overshoot ? "Yes" : "No");

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