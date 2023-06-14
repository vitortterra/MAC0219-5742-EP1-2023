#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define ITERATIONS 10

/*
   2   1
    \ /
  3 - - 0
    / \
   4   5
*/
int directions[2][6][2] = {
    {{0, 1}, {-1, 1}, {-1, 0}, {0, -1}, {1, 0}, {1, 1}},
    {{0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}}
};

void read_grid_from_file(byte* grid, int grid_size) {
    FILE* file_ptr;
    char filename[8];

    snprintf(filename, sizeof(filename), "%d.in", grid_size);

    file_ptr = fopen(filename, "rb");
    if (!file_ptr) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int total_bytes = grid_size * grid_size;
    size_t total_read = fread(grid, sizeof(byte), total_bytes, file_ptr);
    if (total_read != (size_t) total_bytes) {
        fprintf(stderr, "Error reading file, read %lu bytes out of %d\n", total_read, total_bytes);
        exit(EXIT_FAILURE);
    }

    fclose(file_ptr);
}

void print_grid(byte* grid, int grid_size) {
    for (int i = 0; i < grid_size; i++) {

        // Para ter aparencia hexagonal
        if (i % 2 == 0)
            printf(" ");

        for (int j = 0; j < grid_size; j++) {
            byte c = grid[ind2d(i,j)];

            if (c == EMPTY)
                printf("  ");
            else if (c == WALL)
                printf("# ");
            else
                printf("* ");
        }
        printf("\n");
    }
}

// void print_grid_animation() {
//     int i, j;
//     for (i = 0; i < GRID_SIZE; i++) {
//         printf("\"");
//         if (i % 2 == 0)
//             printf(" ");
//         for (j = 0; j < GRID_SIZE; j++) {
//             if (grid[i][j] == EMPTY)
//                 printf("  ");
//             else if (grid[i][j] & WALL)
//                 printf("# ");
//             else 
//                 printf("* ");
//         }
//         // TODO: remove trailing comma after last frame
//         if (i == GRID_SIZE - 1)
//             printf("\"+a,\n");
//         else
//             printf("\"+a+\n");
//     }
// }

// void print_grid_numbers() {
//     int i, j;
//     for (i = 0; i < GRID_SIZE; i++) {
//         for (j = 0; j < GRID_SIZE; j++) {
//             printf("%-3d", grid[i][j]);
//         }
//         printf("\n");
//     }
// }

// int inbounds(int i, int j) {
//     return i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE;
// }

// void handle_collisions(int i, int j) {
//     switch (aux_grid[i][j]) {
//         // 2 particle collision
//         case 0x09:
//             grid[i][j] = 0x12;
//             break;
//         case 0x12:
//             grid[i][j] = 0x28;
//             break;
//         case 0x28:
//             grid[i][j] = 0x09;
//             break;

//         // 3 particle collision
//         case 0x15:
//             grid[i][j] = 0x2A;
//             break;
//         case 0x2A:
//             grid[i][j] = 0x15;
//             break;

//         // 4 particle collision
//         case 0x36:
//             grid[i][j] = 0x2D;
//             break;
//         case 0x2D:
//             grid[i][j] = 0x1B;
//             break;
//         case 0x1B:
//             grid[i][j] = 0x36;
//             break;
//         default:
//             grid[i][j] = aux_grid[i][j];
//     }
// }

// void update() {
//     int i, j;

//     // Update positions
//     for (i = 0; i < GRID_SIZE; i++) {
//         for (j = 0; j < GRID_SIZE; j++) {
//             int dir;
//             for (dir = 0; dir < NUM_DIRECTIONS; dir++) {
//                 byte dir_mask = 0x01 << dir;

//                 if (grid[i][j] & dir_mask) {
//                     int di = directions[i%2][dir][0];
//                     int dj = directions[i%2][dir][1];
//                     int n_i = i + di;
//                     int n_j = j + dj;

//                     if (inbounds(n_i, n_j)) {
//                         if (grid[n_i][n_j] == WALL)
//                             aux_grid[i][j] = ((dir_mask << 3) | (dir_mask >> 3)) % WALL;
//                         else
//                             aux_grid[n_i][n_j] |= dir_mask;
//                     }
//                 }
//             }
//         }
//     }

//     // Handle collisions
//     for (i = 0; i < GRID_SIZE; i++) {
//         for (j = 0; j < GRID_SIZE; j++) {

//             handle_collisions(i, j);

//             if (aux_grid[i][j] & WALL)
//                 aux_grid[i][j] = WALL;
//             else
//                 aux_grid[i][j] = EMPTY;
//         }
//     }
// }

int main(int argc, char* argv[]) {
    byte* grid_1;
    byte* grid_2;
    int grid_size;

    if (argc != 2){
        printf("usage: ./lca grid_size\n");
        exit(EXIT_FAILURE);
    }

    int ret = sscanf(argv[1], "%d", &grid_size);
    if (ret == 0 || ret == EOF) {
        fprintf(stderr, "Error reading grid_size\n");
        exit(EXIT_FAILURE);
    }
    if (grid_size < MIN_GRID_SIZE || grid_size > MAX_GRID_SIZE) {
        printf("Invalid grid_size %d\n", grid_size);
        exit(EXIT_FAILURE);
    }

    grid_1 = (byte*) calloc(grid_size*grid_size, sizeof(byte));
    grid_2 = (byte*) calloc(grid_size*grid_size, sizeof(byte));

    if (!grid_1 || !grid_2) {
        fprintf(stderr, "Error allocating grids\n");
        exit(EXIT_FAILURE);
    }

    read_grid_from_file(grid_1, grid_size);

    // for (int i = 0; i < ITERATIONS; i++) {
    //     update();
    //     print_grid();
    // }

    print_grid(grid_1, grid_size);

    free(grid_1);
    free(grid_2);

    return 0;
}