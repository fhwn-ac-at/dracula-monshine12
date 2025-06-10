#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libs/game_board.h"
#include "libs/config_manager.h"

int main(int argc, char** args) {
    if (argc != 2) {
        logm(ERROR, "main", "A config file is required when trying to run executable e.g. './main game_configs/default_config.txt'!");
        exit(EXIT_FAILURE);
    }

    Config* config = malloc(sizeof(Config));
    parse_config_file(args[1], config);
    logm(DEBUG, "main", "Parsed configuration file successfully!");
    print_board_config(config);
    GameBoard* game_board = create_game_board(config);
    print_game_board(game_board);

    logm(DEBUG, "main", "About to free resources.");
    free_game_board(game_board);
    free(config);
    logm(DEBUG, "main", "Freed resources successfully!");
}