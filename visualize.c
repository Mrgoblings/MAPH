#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"

const char _v_free = ' ', _v_tmp = '.', _v_tile = '#', _v_first_agent = 'a', _v_first_destination = 'A', _v_finished_agent = 'x';

void v_draw(visualize_grid* grid) {
    //* top line
    printf("-");
    for(int x = 0; x < grid->size_x; x++) {
        printf("-");
    }
    printf("-\n");

    //* draw grid data
    for(int y = 0; y < grid->size_y; y++) {
        printf("|");
        for(int x = 0; x < grid->size_x; x++) {
            printf("%c", grid->data[x][y]);
        }
        printf("|\n");
    }

    //* bottom line
    printf("-");
    for(int x = 0; x < grid->size_x; x++) {
        printf("-");
    }
    printf("-\n\n");

    printf("(a, b, c) Agents - lowercase letters\n");
    printf("(A, B, C) Destinations - uppercase letters\n");
    printf("(x) an Agent who has reached his Destination\n\n");

    return;
};


visualize_grid* _v_grid_init(uint8_t size_x, uint8_t size_y) {
    visualize_grid* new_grid = (visualize_grid*) calloc(1, sizeof(visualize_grid));
    new_grid->size_x = size_x;
    new_grid->size_y = size_y;
    
    new_grid->data = (char**) malloc(size_x * sizeof(char*));
    for (uint8_t x = 0; x < size_x; x++) {
        new_grid->data[x] = (char*) malloc(size_y * sizeof(char));
        for (uint8_t y = 0; y < size_y; y++) {
            new_grid->data[x][y] = _v_free;
        }
    }

    return new_grid;
}


void v_free_grid(visualize_grid* grid) {
    for (uint8_t x = 0; x < grid->size_x; x++) {
        free(grid->data[x]);
    }
    free(grid->data);
    free(grid);
}


visualize_grid* v_generate_grid(uint8_t size_x, uint8_t size_y, uint8_t n_tiles) {
    if(size_x == 0 || size_y == 0) return NULL;

    if(size_x * size_y <=n_tiles) {
        printf("Error! Number of tiles must be compatable with the space given.\n");
        return NULL;;
    }

    visualize_grid* new_grid = _v_grid_init(size_x, size_y);

    //* Fill the grid with tiles
    for (uint8_t i = 0; i < n_tiles; i++) {
        v_add_tile(new_grid);
    }

    return new_grid;
};



visualize_grid* v_read_grid(char* input_file) {
    FILE* fp = fopen(input_file, "r");

    if (fp == NULL) {
        puts("Error! Can't open the input file.\n");
        return NULL;
    }

    int curr_size_x = 0, max_size_x = 0, size_y = 0;
    int has_content = 0;  

    //* Calculate curr_size_x and size_y
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            if (has_content) {
                size_y++;
                if (max_size_x == 0)
                    max_size_x = curr_size_x;
                else if (curr_size_x != max_size_x) {
                    fclose(fp);
                    printf("Incorrect file format! Caught a symbol outside of the given size. All lines must be the same length. msx - %d, sx - %d\n", max_size_x, curr_size_x);
                    return NULL;
                }
            }
            curr_size_x = 0;
            has_content = 0;  
        } else if (c == '|' || c == '-') {
            continue;
        } else {
            curr_size_x++;
            has_content = 1;  
        }
    }

    //* Handle the last line in the file
    if (has_content) {
        size_y++;
        if (max_size_x == 0)
            max_size_x = curr_size_x;
        else if (curr_size_x != max_size_x) {
            fclose(fp);
            printf("Incorrect file format! Caught a symbol outside of the given size. All lines must be the same length. msx - %d, sx - %d\n", max_size_x, curr_size_x);
            return NULL;
        }
    }

    //* Reset the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    //* Create the grid
    visualize_grid* grid = _v_grid_init(max_size_x, size_y);
    if (grid == NULL) {
        fclose(fp);
        return NULL;
    }

    //* Read the grid data from the file
    int x = 0, y = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            if(x != 0) {
                x = 0;
                y++;
            }
        } else if (c == '|' || c == '-') {
            continue;
        } else if (c == _v_tile || c == _v_free || c == _v_tmp) {
            if (x < grid->size_x && y < grid->size_y) {
                if(c == _v_tmp) grid->data[x][y] = _v_free;
                else grid->data[x][y] = c;
                x++;
            }
        } else {
            fclose(fp);
            v_free_grid(grid);
            printf("Incorrect file format. Caught an invalid symbol '%c'.\n", c);
            return NULL;
        }
    }

    fclose(fp);
    return grid;
}



void v_add_tile(visualize_grid* grid) {
    uint8_t x, y; 

    do {
        x = _v_random(grid->size_x);
        y = _v_random(grid->size_y);
    } while(!_v_is_space_available_for_tile(grid, x, y));

    grid->data[x][y] = _v_tile;
};


void v_add_agent(visualize_grid* grid) {
    uint8_t x, y; 

    //* agent
    do {
        x = _v_random(grid->size_x);
        y = _v_random(grid->size_y);
    } while(grid->data[x][y] != _v_free);

    grid->data[x][y] = _v_first_agent + (grid->n_agents);

    //* destination
    do {
        x = _v_random(grid->size_x);
        y = _v_random(grid->size_y);
    } while(grid->data[x][y] != _v_free);

    grid->data[x][y] = _v_first_destination + (grid->n_agents)++;
};


void v_add_agents(visualize_grid* grid, uint8_t n_agents) {
    for(int i = 0; i < n_agents; i++){
        v_add_agent(grid);
    }
}

int8_t v_solve_grid_one_step(visualize_grid* grid) {
    int8_t n_agents_left = grid->n_agents;

    int8_t dx[] = {-1, 0, 1, 0}; //* Delta rows for up, right, down, left
    int8_t dy[] = {0, 1, 0, -1}; //* Delta cols for up, right, down, left

    uint8_t visited[grid->size_x][grid->size_y];
    uint8_t parent[grid->size_x][grid->size_y];

    agent agents[grid->n_agents];

    //* setup default values
    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            visited[x][y] = 0;
            parent[x][y] = -1;

            //* get agents
            if('a' >= grid->data[x][y] || grid->data[x][y] <= 'z') { 
                agents[grid->data[x][y] - 'a'].x = x; 
                agents[grid->data[x][y] - 'a'].y = y; 
            } else if('A' >= grid->data[x][y] || grid->data[x][y] <= 'Z') { 
                agents[grid->data[x][y] - 'A'].dest_x = x; 
                agents[grid->data[x][y] - 'A'].dest_y = y; 
            }
        }
    }


    uint8_t min_weight, min_direction, curr_weight;
    for(uint8_t i_a = 0; i_a < grid->n_agents; i_a++) {
        min_weight = 0; 
        min_direction = 0;
        for (uint8_t direction = 0; direction < 4; direction++) {
            int8_t new_x = agents[i_a].x + dx[direction];
            int8_t new_y = agents[i_a].y + dy[direction];

            if (new_x >= 0 && new_x < grid->size_x && new_y >= 0 && new_y < grid->size_y) {
                curr_weight = _v_heuristic(new_x, new_y, agents[i_a].dest_x, agents[i_a].dest_y);

                if (min_weight < curr_weight) {
                    min_weight = curr_weight;
                    min_direction = direction;
                }
            }
        }
    }

    
    return n_agents_left;
}


uint8_t _v_heuristic(uint8_t start_row, uint8_t start_col, uint8_t dest_row, uint8_t dest_col) {
    return abs(start_row - dest_row) + abs(dest_col - start_col);
}



int _v_is_space_available_for_tile(visualize_grid* grid, uint8_t x, uint8_t y) {
    if(grid->data[x][y] == _v_tile) return 0;

    grid->data[x][y] = _v_tile;
    if(_v_are_spaces_connected(grid)) return 1;
    grid->data[x][y] = _v_free;

    return 0;
};


//* quality of life function
uint8_t _v_random(uint8_t max) {
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
