#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

#define ITERATIONS 50

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

void parse_arguments(int argc, char* argv[], int* grid_size_ptr) {
    int ret;

    if (argc != 3){
        printf("usage: ./lca --grid_size <grid_size>\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc-1; i++) {
        if (strcmp(argv[i], "--grid_size") == 0) {
            ret = sscanf(argv[i+1], "%d", grid_size_ptr);
            if (ret == 0 || ret == EOF) {
                fprintf(stderr, "Error reading grid_size\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (*grid_size_ptr < MIN_GRID_SIZE || *grid_size_ptr > MAX_GRID_SIZE) {
        printf("Invalid grid_size %d (min=%d, max=%d)\n", 
            *grid_size_ptr, MIN_GRID_SIZE, MAX_GRID_SIZE);
        exit(EXIT_FAILURE);
    }
}

byte* allocate_grid(int grid_size) {
    byte* grid = (byte*) calloc(grid_size*grid_size, sizeof(byte));

    if (!grid) {
        fprintf(stderr, "Error allocating grid\n");
        exit(EXIT_FAILURE);
    }

    return grid;
}

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

void initialize_grids(byte* grid_1, byte* grid_2, int grid_size) {
    read_grid_from_file(grid_1, grid_size);

    // Como as paredes sao fixas, basta inseri-las no grid auxiliar
    // durante a inicializacao
    for (int idx = 0; idx < grid_size*grid_size; idx++) {
        if (grid_1[idx] == WALL)
            grid_2[idx] = WALL;
        else
            grid_2[idx] = EMPTY;
    }
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

void print_grid_animation(byte* grid, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        printf("\"");
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
        // TODO: remove trailing comma after last frame
        if (i == grid_size - 1)
            printf("\"+a,\n");
        else
            printf("\"+a+\n");
    }
}

void print_grid_numbers(byte* grid, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            printf("%-3d", grid[ind2d(i,j)]);
        }
        printf("\n");
    }
}

static inline bool inbounds(int i, int j, int grid_size) {
    return i >= 0 && i < grid_size && j >= 0 && j < grid_size;
}

void wall_collision(int i, int j, byte* grid_out, int grid_size, int dir) {
    // Reflexao simples:
    // byte dir_mask = 0x01 << dir;
    // grid_out[ind2d(i,j)] |= ((dir_mask << 3) | (dir_mask >> 3)) % WALL;

    if (dir == 0) {
        grid_out[ind2d(i,j)] |= 0x08;
    }
    else if (dir == 1) {
        if (inbounds(i-2, j, grid_size) && grid_out[ind2d(i-2,j)] != WALL)
            grid_out[ind2d(i-2,j)] |= 0x04;
        else if (inbounds(i, j+1, grid_size) && grid_out[ind2d(i,j+1)] != WALL)
            grid_out[ind2d(i,j+1)] |= 0x20;
        else
            grid_out[ind2d(i,j)] |= 0x10;

    } else if (dir == 2) {
        if (inbounds(i-2, j, grid_size) && grid_out[ind2d(i-2,j)] != WALL)
            grid_out[ind2d(i-2,j)] |= 0x02;
        else if (inbounds(i, j-1, grid_size) && grid_out[ind2d(i,j-1)] != WALL)
            grid_out[ind2d(i,j-1)] |= 0x10;
        else
            grid_out[ind2d(i,j)] |= 0x20;

    } else if (dir == 3) {
        grid_out[ind2d(i,j)] |= 0x01;

    } else if (dir == 4) {
        if (inbounds(i+2, j, grid_size) && grid_out[ind2d(i+2,j)] != WALL)
            grid_out[ind2d(i+2,j)] |= 0x20;
        else if (inbounds(i, j-1, grid_size) && grid_out[ind2d(i,j-1)] != WALL)
            grid_out[ind2d(i,j-1)] |= 0x04;
        else
            grid_out[ind2d(i,j)] |= 0x02;

    } else if (dir == 5) {
        if (inbounds(i+2, j, grid_size) && grid_out[ind2d(i+2,j)] != WALL)
            grid_out[ind2d(i+2,j)] |= 0x10;
        else if (inbounds(i, j+1, grid_size) && grid_out[ind2d(i,j+1)] != WALL)
            grid_out[ind2d(i,j+1)] |= 0x02;
        else
            grid_out[ind2d(i,j)] |= 0x04;
    }
}

byte particles_collision(byte cell) {
    switch (cell) {
        // Colisao entre duas particulas
        case 0x09:
            return 0x12;
        case 0x12:
            return 0x28;
        case 0x28:
            return 0x09;

        // Colisao entre tres particulas
        case 0x15:
            return 0x2A;
        case 0x2A:
            return 0x15;

        // Colisao entre quatro particulas
        case 0x36:
            return 0x2D;
        case 0x2D:
            return 0x1B;
        case 0x1B:
            return 0x36;

        default:
            return cell;
    }
}

void update(byte* grid_in, byte* grid_out, int grid_size) {
    // Propagacao
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            // Para cada celula, para cada direcao, verifica se
            // ha particula indo naquela direcao
            for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
                byte dir_mask = 0x01 << dir;

                // Caso haja uma particula indo na direcao dir,
                // atualiza o vizinho colocando nele uma particula
                // na mesma direcao.
                // Caso o vizinho seja uma barreira (WALL), reflete
                // a velocidade da particula.
                if (grid_in[ind2d(i,j)] & dir_mask) {
                    int di = directions[i%2][dir][0];
                    int dj = directions[i%2][dir][1];
                    int n_i = i + di;
                    int n_j = j + dj;

                    if (inbounds(n_i, n_j, grid_size)) {
                        if (grid_in[ind2d(n_i,n_j)] == WALL)
                            wall_collision(i, j, grid_out, grid_size, dir);
                        else
                            grid_out[ind2d(n_i,n_j)] |= dir_mask;

                        grid_in[ind2d(i,j)] &= ~dir_mask;
                    }
                }
            }
        }
    }

    // Colisoes
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid_out[ind2d(i,j)] = particles_collision(grid_out[ind2d(i,j)]);
        }
    }
}

int main(int argc, char* argv[]) {
    byte* grid_1;
    byte* grid_2;
    int grid_size;

    parse_arguments(argc, argv, &grid_size);

    grid_1 = allocate_grid(grid_size);
    grid_2 = allocate_grid(grid_size);

    initialize_grids(grid_1, grid_2, grid_size);

    print_grid_animation(grid_1, grid_size);

    for (int i = 0; i < ITERATIONS; i++) {
        update(grid_1, grid_2, grid_size);
        print_grid_animation(grid_2, grid_size);
        update(grid_2, grid_1, grid_size);
        print_grid_animation(grid_1, grid_size);
    }

    free(grid_1);
    free(grid_2);

    return 0;
}
