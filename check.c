#include "lga_base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERATIONS 50

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

    print_grid(grid_1, grid_size);

    for (int i = 0; i < ITERATIONS; i++) {
        update(grid_1, grid_2, grid_size);
        print_grid(grid_2, grid_size);
        update(grid_2, grid_1, grid_size);
        print_grid(grid_1, grid_size);
    }

    free(grid_1);
    free(grid_2);

    return 0;
}
