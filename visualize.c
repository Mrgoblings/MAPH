#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"

const char _v_free = ' ', _v_tile = '#', _v_first_agant = 'a', _v_first_tile = 'A', _v_tmp = 'c';


void v_draw(visualize_grid* grid) {
    //* top line
    printf("-");
    for(int x = 0; x < grid->size_x; x++) {
        printf("--");
    }
    printf("--\n");

    //* draw grid data
    for(int y = 0; y < grid->size_y; y++) {
        printf("| ");
        for(int x = 0; x < grid->size_x; x++) {
            printf("%c ", grid->data[x][y]);
        }
        printf("|\n");
    }

    //* bottom line
    printf("-");
    for(int x = 0; x < grid->size_x; x++) {
        printf("--");
    }
    printf("--\n");

    return;
};


visualize_grid* v_generate_grid(uint8_t size_x, uint8_t size_y, uint8_t n_tiles) {
    if(size_x == 0 || size_y == 0) return NULL;

    if(size_x * size_y <=n_tiles) {
        printf("Error! Number of tiles must be compatable with the space given.\n");
        return NULL;;
    }

    //* Seed the random number generator
    srand(time(NULL));


    visualize_grid* new_grid = (visualize_grid*) calloc(1, sizeof(visualize_grid));
    new_grid->size_x = size_x;
    new_grid->size_y = size_y;
    
    new_grid->data = (char**) malloc(size_x * sizeof(char*));
    for (uint8_t x = 0; x < size_x; x++) {
        new_grid->data[x] = (char*)malloc(size_y * sizeof(char));
        for (uint8_t y = 0; y < size_y; y++) {
            new_grid->data[x][y] = _v_free;
        }
    }

    //* Fill the grid with tiles
    for (uint8_t i = 0; i < n_tiles; i++) {
        _v_add_tile(new_grid);
    }

    return new_grid;
};


void v_free_grid(visualize_grid* grid) {
    for (uint8_t x = 0; x < grid->size_x; x++) {
        free(grid->data[x]);
    }
    free(grid->data);
    free(grid);
}

visualize_grid* v_read_grid(char* input_file) {
    FILE* fp;
    fp = fopen(input_file, "r");
    
    if (fp == NULL) {
        puts("Error! Can't open the input file.");
        return NULL;
    }
    
    visualize_grid* grid = (visualize_grid*) malloc(sizeof(visualize_grid));
    if (grid == NULL) {
        fclose(fp);
        return NULL;
    }
    
    char curr_char;
    int x = 0, y = 0;
    while (fscanf(fp, "%c", &curr_char) != EOF) {
        if (curr_char == '\n') {
            if (x == grid->size_x) {
                x = 0;
                y++;
            } else {
                fclose(fp);
                free(grid);
                printf("Incorrect file format. All lines must be the same length.");
                return NULL;
            }
        } else if (curr_char == _v_tile || curr_char == _v_free) {
            if (x < grid->size_x && y < grid->size_y) {
                grid->data[x][y] = curr_char;
                x++;
            } else {
                fclose(fp);
                free(grid);
                return NULL;
            }
        } else if (curr_char == _v_tmp) {
            if (x < grid->size_x && y < grid->size_y) {
                grid->data[x][y] = _v_free;
                x++;
            } else {
                fclose(fp);
                free(grid);
                return NULL;
            }
        } else {
            fclose(fp);
            free(grid);
            return NULL;
        }
    }
    
    fclose(fp);
    return grid;
}



void _v_add_tile(visualize_grid* grid) {
    uint8_t x, y; 

    do {
        x = _v_random(grid->size_x);
        y = _v_random(grid->size_y);
        // // printf("x: %d (%d), y: %d\n", x, y);
    } while(!_v_is_space_available_for_tile(grid, x, y));

    grid->data[x][y] = _v_tile;
};


int _v_is_space_available_for_tile(visualize_grid* grid, uint8_t x, uint8_t y) {
    if(grid->data[x][y] == _v_tile) return 0;

    grid->data[x][y] = _v_tile;
    if(_v_are_spaces_connected(grid)) return 1;
    grid->data[x][y] = _v_free;

    return 0;
};


uint8_t _v_random(uint8_t max) {
    
    //* Generate a random number within the specified range
    return rand() % max;
}


void _v_fill(visualize_grid* grid, uint8_t x, uint8_t y) {
    if (x < 0 || x >= grid->size_x || y < 0 || y >= grid->size_y) {
        return; // Ignore out-of-bounds cells
    }

    grid->data[x][y] = _v_tmp;
    if(x + 1 < grid->size_x && grid->data[x + 1][y] == _v_free) _v_fill(grid, x + 1, y);
    if(x - 1 >= 0 && grid->data[x - 1][y] == _v_free) _v_fill(grid, x - 1, y);
    if(y + 1 < grid->size_y && grid->data[x][y + 1] == _v_free) _v_fill(grid, x, y + 1);
    if(y - 1 >= 0 && grid->data[x][y - 1] == _v_free) _v_fill(grid, x, y - 1);
    return;
};

int _v_are_spaces_connected(visualize_grid* grid) {
    int answer = 1; //* bool

    //* get the fist empty space
    uint8_t first_x_position = 0, first_y_position = 0;
    for (; first_x_position < grid->size_x; first_x_position++) {
        for (first_y_position = 0; first_y_position < grid->size_y; first_y_position++) {
            if (grid->data[first_x_position][first_y_position] == _v_free) {
                break;
            }
        }
        if (first_y_position < grid->size_y) {
            break;
        }
    } 
    //* fill
    _v_fill(grid, first_x_position, first_y_position);

    //* check if everything is filled and reverse it to free
    for(uint8_t x = 0; x < grid->size_x; x++) {
        for(uint8_t y = 0; y < grid->size_y; y++) {
            if(grid->data[x][y] == _v_free) {
                answer = 0;
            } else if(grid->data[x][y] == _v_tmp) {
                grid->data[x][y] = _v_free;
            };
        }
    }

    return answer;
};
