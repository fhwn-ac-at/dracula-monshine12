#pragma once
#include "logger.h"
#define MAX_SNAKES 100
#define MAX_LADDERS 100
#define MAX_LINE_LENGTH 256

typedef struct {
    int start;
    int end;
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
} BoardConfig;

/**
 * @brief Will initialize given BoardConfig variable with information from config file
 * 
 * @param filename Name and location of config file
 * @param config Ptr to BoardConfig object which will be initialized with this function
 */
void parse_config_file(const char* filename, BoardConfig* config);

/**
 * @brief Prints all information stored within BoardConfig variable
 * 
 * @param config Configuration variable holding onto all configs
 */
void print_board_config(const BoardConfig* config);