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
    int return_val = parse_config_file(args[1], config);
    if (return_val) {
        free(config);
        logm(ERROR, "main", "An error occured during config parse phase");
        exit(EXIT_FAILURE);
    }

    logm(DEBUG, "main", "Parsed configuration file successfully!");
    // print_board_config(config);
    GameBoard* board = create_game_board(config);
    print_game_board(board);
    
    logm(DEBUG, "main", "Starting simulation now.");
    SimResults* results = run_sim(board, config);
    if (results == NULL) {
        free(config);
        free_board(board);
        logm(ERROR, "main", "An error occured within run_sim and it returned NULL. Terminating program.");
        exit(EXIT_FAILURE);
    }

    print_sim_results(results);
    logm(DEBUG, "main", "Successfully ended simulation.");
    
    logm(DEBUG, "main", "About to free resources.");
    free_board(board);
    free(config);
    free(results->shortest_roll_sequence);
    free(results);
    logm(DEBUG, "main", "Freed resources successfully!");
}