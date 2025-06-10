#pragma once
#include "config_manager.h"
#include "game_board.h"

typedef struct {
    float avg_rolls;
    int shortest_num_rolls;
} SimResults;

int roll_dice(int dice_sides);
SimResults run_sim(GameBoard* board, Config* config);
void print_sim_results(SimResults sim_results);