#include "lga_base.h"
#include "lga_seq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[]) {
    byte* grid_1;
    byte* grid_2;
    int grid_size;

    parse_arguments(argc, argv, &grid_size);

    grid_1 = allocate_grid(grid_size);
    grid_2 = allocate_grid(grid_size);

    simulate_seq(grid_1, grid_2, grid_size);

    print_grid(grid_1, grid_size);

    free(grid_1);
    free(grid_2);

    return 0;
}
