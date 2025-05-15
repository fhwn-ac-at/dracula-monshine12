#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 65; i < 65 + SIZE; i++) {
        printf("%4c", i);
    }
    puts("");

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (j == 0) 
                printf("%c", 65 + i);
            printf("%3d ", matrix[i][j]);
        }
        puts("");
    }
} 

void add_edge(int matrix[SIZE][SIZE], Edge edge) {
    if ((edge.from < 0 || edge.from > SIZE - 1) || (edge.to < 0 || edge.to > SIZE - 1)) {
        puts("ERROR - add_edge(): From/To attribute of edge out of bounds! Exiting programm!");
        exit(EXIT_FAILURE);
    }
    if (edge.weight <= 0) {
        puts("WARNING - add_edge(): Weigth of edge needs to be > 0. Edge won't be added to matrix, programm will continue!");
    }
    if (matrix[edge.from][edge.to] != 0) {
        printf("WARNING - add_edge(): Vertice (%d) already has an existing edge to the vertice (%d). Edge won't be added to matrix, programm will continue!\n", edge.from, edge.to);
    }

    matrix[edge.from][edge.to] = edge.weight;
    matrix[edge.to][edge.from] = -edge.weight;
}

int main(void) {
    int matrix[SIZE][SIZE] = {0};

    Edge edges[] = {
        {0, 3, 9},
        {2, 0, 4},
        {1, 2, 1},
        {4, 2, 5},
        {4, 3, 4},
        {0, 4, 2},
        {3, 1, 6}
    };

    for (int i = 0; i < (int) (sizeof(edges)/sizeof(Edge)); i++) {
        add_edge(matrix, edges[i]);
    }

    print_matrix(matrix);
    return 0;
}