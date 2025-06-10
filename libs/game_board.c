#include "game_board.h"
#include <stdlib.h>
#include <stdio.h>

GameBoard* create_game_board(Config* config) {
    int num_fields = config->rows * config->cols;
    GameBoard* game_board = malloc(sizeof(GameBoard));
    game_board->start = malloc(num_fields * sizeof(Node*));

    // Initialize game_board with values from config
    game_board->rows = config->rows;
    game_board->cols = config->cols;
    
    Node** gb = game_board -> start;
    logm(DEBUG, "create_game_board", "Initialized game board successfully.");

    // Initialize default board
    for (int i = 0; i < num_fields; i++) {
        gb[i] = malloc(sizeof(Node));
        gb[i]->ft = DEFAULT;
    }
    logm(DEBUG, "create_game_board", "Initialized game board with default Node* successfully.");

    // Include snakes and ladders and possible moves from any current position
    for (int i = 0; i < num_fields; i++) {
        // Check if current field is beginning of snake
        for (int j = 0; j < config->num_snakes; j++) {
            if (config->snakes[j].start - 1 == i) {
                // Current field is start of snake
                gb[i]->ft = SNAKE;
                gb[i]->successors = malloc(sizeof(Node*));
                gb[i]->successors[0] = gb[config->snakes[j].end - 1]; 
            }
        }

        // Check if current field is beginning of ladder
        for (int j = 0; j < config->num_ladders; j++) {
            if (config->ladders[j].start - 1 == i) {
                // Current field is start of ladder
                gb[i]->ft = LADDER;
                gb[i]->successors = malloc(sizeof(Node*));
                gb[i]->successors[0] = gb[config->ladders[j].end - 1]; 
            }
        }

        // Current field is still default
        if (gb[i]->ft == DEFAULT) {
            gb[i]->successors = malloc(sizeof(Node*) * config->dice_sides);
            for (int j = 0; j < config->dice_sides; j++) {
                if (i + j + 1 < num_fields) {
                    gb[i]->successors[j] = gb[i + j + 1];
                } else {
                    gb[i]->successors[j] = gb[i];
                }
            }
        }
    }
    logm(DEBUG, "create_game_board", "Successfully added snakes and ladders to game field.");
    return game_board;
}

void free_game_board(GameBoard* game_board) {
    if (!game_board) return;

    int num_fields = game_board->rows * game_board->cols;

    if (game_board->start) {
        for (int i = 0; i < num_fields; i++) {
            // Free possible destinations 
            free(game_board->start[i]->successors);
            // Free node itself
            free(game_board->start[i]);
        }
        // Free the array of Node* 
        free(game_board->start);
    }
    // Free GameBoard itself
    free(game_board);
}

void print_game_board(GameBoard* board) {
    if (!board || !board->start) return;

    puts("");
    int num_fields = board->rows * board->cols;

    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            int index = r * board->cols + c;
            Node* node = board->start[index];

            printf("[%3d] ", index + 1);

            switch (node->ft) {
                case SNAKE:
                    for (int i = 0; i < num_fields; i++) {
                        if (board->start[i] == node->successors[0]) {
                            printf("S -> %3d", i + 1);
                            break;
                        }
                    }
                    break;
                case LADDER:
                    for (int i = 0; i < num_fields; i++) {
                        if (board->start[i] == node->successors[0]) {
                            printf("L -> %3d", i + 1);
                            break;
                        }
                    }
                    break;
                default:
                    printf("N       ");
                    break;
            }
            printf(" | ");
        }
        puts("");
    }
    puts("");
}