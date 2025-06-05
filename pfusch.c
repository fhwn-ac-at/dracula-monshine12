#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} point_t;

int is_point_in_unit_circle(point_t p) {
    if (p.x * p.x + p.y * p.y <= 1) {
        return 1;
    }
    return 0;
}

point_t get_random_point() {
    point_t p;
    p.x = 1 - 2 * (float)rand()/(float)RAND_MAX;
    p.y = 1 - 2 * (float)rand()/(float)RAND_MAX;
    return p;
}

point_t random_point_in_unit_circle() {
    srand((unsigned int)time(NULL));
    point_t p;
    do {
        p = get_random_point();
    } while (!is_point_in_unit_circle(p));
    return p;
}

float approximate_pi(int const num_samples) {
    int inside = 0;

    for (int i = 0; i < num_samples; ++i) {
        point_t const p = get_random_point();
        if (is_point_in_unit_circle(p)) {
            ++inside;
        }
    }

    return 4.0f * (float) inside / (float) num_samples;
}
int main() {
    // point_t p = random_point_in_unit_circle();
    // printf("Random point in unit cicle: %f, %f\n", p.x, p.y);
    printf("PI is approximately %.6f\n", approximate_pi(1000000000));
}