#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libs/game_board.h"
#include "libs/config_manager.h"
#include "libs/sim.h"
#include <time.h>

int main(int argc, char** args) {
    if (argc != 2) {
        logm(ERROR, "main", "A config file is required when trying to run executable e.g. './main game_configs/default.txt'!");
        exit(EXIT_FAILURE);
    }

    Config* config = malloc(sizeof(Config));
    parse_config_file(args[1], config);
    logm(DEBUG, "main", "Parsed configuration file successfully!");
    // print_board_config(config);
    GameBoard* board = create_game_board(config);
    print_game_board(board);
    
    logm(DEBUG, "main", "Starting simulation now.");
    SimResults results = run_sim(board, config);
    print_sim_results(results);
    logm(DEBUG, "main", "Successfully ended simulation.");
    
    logm(DEBUG, "main", "About to free resources.");
    free_game_board(board);
    free(config);
    logm(DEBUG, "main", "Freed resources successfully!");
}