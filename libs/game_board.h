#pragma once
#include "logger.h"
#include "config_manager.h"

typedef enum {
    SNAKE, LADDER, DEFAULT
} FieldType;

typedef struct Node {
    FieldType ft;
    struct Node** possible_destinations;
} Node;

typedef struct {
    int rows;
    int cols;
    Node** start;
} GameBoard;

GameBoard* create_game_board(GameConfig* config);
void free_game_board(GameBoard* board);
void print_game_board(GameBoard* board);