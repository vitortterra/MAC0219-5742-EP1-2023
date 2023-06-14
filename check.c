#include "lga_base.h"
#include "lga_seq.h"
#include "lga_omp.h"
#include "lga_pth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void parse_arguments(int argc, char* argv[], int* grid_size_ptr) {
    int ret;

    if (argc != 3){
        printf("usage: ./check --grid_size <grid_size>\n");
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

bool are_grids_equal(byte* grid_a, byte* grid_b, int grid_size) {
    for (int idx = 0; idx < grid_size*grid_size; idx++)
        if (grid_a[idx] != grid_b[idx])
            return false;
    
    return true;
}

int main(int argc, char* argv[]) {
    byte* grid_ref;
    byte* grid_1;
    byte* grid_2;
    int grid_size;

    parse_arguments(argc, argv, &grid_size);

    grid_ref = allocate_grid(grid_size);
    grid_1 = allocate_grid(grid_size);
    grid_2 = allocate_grid(grid_size);

    simulate_seq(grid_ref, grid_2, grid_size);

    simulate_omp(grid_1, grid_2, grid_size);
    if (are_grids_equal(grid_ref, grid_1, grid_size))
        printf("Resultado OpenMP igual ao sequencial\n");
    else
        printf("Resultado OpenMP DIFERENTE do sequencial\n");
    
    simulate_pth(grid_1, grid_2, grid_size);
    if (are_grids_equal(grid_ref, grid_1, grid_size))
        printf("Resultado Pthreads igual ao sequencial\n");
    else
        printf("Resultado Pthreads DIFERENTE do sequencial\n");

    free(grid_ref);
    free(grid_1);
    free(grid_2);

    return 0;
}
