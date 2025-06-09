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

    BoardConfig* config = malloc(sizeof(BoardConfig));
    parse_config_file(args[1], config);
    print_board_config(config);
    // Free Memory at end of progam
    free(config);
}