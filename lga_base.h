/* Isso aqui embaixo é um header guard. Serve para evitar dupla-inclusão
 * deste cabeçalho
 */
#ifndef _LGA_BASE_H
#define _LGA_BASE_H

#include <stdbool.h>

#define EMPTY 0x00
#define WALL 0x40
#define MIN_GRID_SIZE 8
#define MAX_GRID_SIZE 4096
#define NUM_DIRECTIONS 6

// Conveniente, mas exige grid_size definida localmente
#define ind2d(i,j) ((i)*(grid_size)+(j))

typedef unsigned char byte;

extern int directions[2][6][2];

inline bool inbounds(int i, int j, int grid_size) {
    return i >= 0 && i < grid_size && j >= 0 && j < grid_size;
}

byte* allocate_grid(int grid_size);

void read_grid_from_file(byte* grid, int grid_size);

void initialize_grids(byte* grid_1, byte* grid_2, int grid_size);

inline bool inbounds(int i, int j, int grid_size);

void wall_collision(int i, int j, byte* grid_out, int grid_size, int dir);

byte particles_collision(byte cell);

void print_grid(byte* grid, int grid_size);

void print_grid_animation(byte* grid, int grid_size);

void print_grid_numbers(byte* grid, int grid_size);

#endif