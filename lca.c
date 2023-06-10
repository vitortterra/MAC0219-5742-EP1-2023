#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 10
#define EMPTY 0x00
#define WALL 0x40
#define NUM_DIRECTIONS 6
#define ITERATIONS 10

typedef unsigned char byte;

byte grid[GRID_SIZE][GRID_SIZE];
byte aux_grid[GRID_SIZE][GRID_SIZE];

int directions[2][6][2] = {
    {{0, 1}, {-1, 1}, {-1, 0}, {0, -1}, {1, 0}, {1, 1}},
    {{0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}}
};

void initialize_grid() {
    int i, j;

    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            if (i == 0 || i == GRID_SIZE - 1 || j == 0 || j == GRID_SIZE - 1) {
                grid[i][j] = WALL;  // Set walls on the boundary
                aux_grid[i][j] = WALL;
            }
            else {
                // if (drand48() < 0.05) {
                //     int dir = (int) (drand48() * 6);
                //     grid[i][j] = 0x01 << dir;
                // } else {
                //     grid[i][j] = EMPTY;  // Empty grid point
                // }

                grid[i][j] = EMPTY;  // Empty grid point
                aux_grid[i][j] = EMPTY;
            }
        }
    }

    grid[2][1] = 0x01; // To the right
    grid[2][3] = 0x08; // To the left
    grid[3][1] = 0x20; // Down and right

}

void print_grid() {
    int i, j;
    for (i = 0; i < GRID_SIZE; i++) {
        if (i % 2 == 0)
            printf(" ");

        for (j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == EMPTY)
                printf("  ");
            else if (grid[i][j] == WALL)
                printf("# ");
            else
                printf("* ");
        }
        printf("\n");
    }
}

void print_grid_animation() {
    int i, j;
    for (i = 0; i < GRID_SIZE; i++) {
        printf("\"");
        if (i % 2 == 0)
            printf(" ");
        for (j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == EMPTY)
                printf("  ");
            else if (grid[i][j] & WALL)
                printf("# ");
            else 
                printf("* ");
        }
        // TODO: remove trailing comma after last frame
        if (i == GRID_SIZE - 1)
            printf("\"+a,\n");
        else
            printf("\"+a+\n");
    }
}

void print_grid_numbers() {
    int i, j;
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            printf("%-3d", grid[i][j]);
        }
        printf("\n");
    }
}

int inbounds(int i, int j) {
    return i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE;
}

void handle_collisions(int i, int j) {
    switch (aux_grid[i][j]) {
        // 2 particle collision
        case 0x09:
            grid[i][j] = 0x12;
            break;
        case 0x12:
            grid[i][j] = 0x28;
            break;
        case 0x28:
            grid[i][j] = 0x09;
            break;

        // 3 particle collision
        case 0x15:
            grid[i][j] = 0x2A;
            break;
        case 0x2A:
            grid[i][j] = 0x15;
            break;

        // 4 particle collision
        case 0x36:
            grid[i][j] = 0x2D;
            break;
        case 0x2D:
            grid[i][j] = 0x1B;
            break;
        case 0x1B:
            grid[i][j] = 0x36;
            break;
        default:
            grid[i][j] = aux_grid[i][j];
    }
}

void update() {
    int i, j;

    // Update positions
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            int dir;
            for (dir = 0; dir < NUM_DIRECTIONS; dir++) {
                byte dir_mask = 0x01 << dir;

                if (grid[i][j] & dir_mask) {
                    int di = directions[i%2][dir][0];
                    int dj = directions[i%2][dir][1];
                    int n_i = i + di;
                    int n_j = j + dj;

                    if (inbounds(n_i, n_j)) {
                        if (grid[n_i][n_j] == WALL)
                            aux_grid[i][j] = ((dir_mask << 3) | (dir_mask >> 3)) % WALL;
                        else
                            aux_grid[n_i][n_j] |= dir_mask;
                    }
                }
            }
        }
    }

    // Handle collisions
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {

            handle_collisions(i, j);

            if (aux_grid[i][j] & WALL)
                aux_grid[i][j] = WALL;
            else
                aux_grid[i][j] = EMPTY;
        }
    }
}

int main() {
    initialize_grid();
    print_grid();

    int i;
    for (i = 0; i < ITERATIONS; i++) {
        update();
        print_grid();
    }

    return 0;
}