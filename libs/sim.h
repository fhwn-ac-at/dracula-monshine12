#pragma once
#include "config_manager.h"
#include "game_board.h"

typedef struct {
    float avg_rolls;
    int overshots;
    int shortest_num_rolls;
    
    int num_snakes;
    Transition snakes[MAX_SNAKES];

    int num_ladders;
    Transition ladders[MAX_SNAKES];
} SimResults;

int roll_dice(int dice_sides);
SimResults run_sim(GameBoard* board, Config* config);
void print_sim_results(SimResults sim_results);