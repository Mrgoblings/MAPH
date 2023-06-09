#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"

const char _v_free = ' ', _v_tmp = '.', _v_tile = '#', _v_first_agent = 'a', _v_first_destination = 'A', _v_finished_agent = 'x';

void v_draw(Visualize_grid* grid) {
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


Visualize_grid* _v_grid_init(uint8_t size_x, uint8_t size_y) {
    Visualize_grid* new_grid = (Visualize_grid*) calloc(1, sizeof(Visualize_grid));
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


void v_free_grid(Visualize_grid* grid) {
    for (uint8_t x = 0; x < grid->size_x; x++) {
        free(grid->data[x]);
    }
    free(grid->data);
    free(grid);
}


Visualize_grid* v_generate_grid(uint8_t size_x, uint8_t size_y, uint8_t n_tiles) {
    if(size_x == 0 || size_y == 0) return NULL;

    if(size_x * size_y <=n_tiles) {
        printf("Error! Number of tiles must be compatable with the space given.\n");
        return NULL;;
    }

    Visualize_grid* new_grid = _v_grid_init(size_x, size_y);

    //* Fill the grid with tiles
    for (uint8_t i = 0; i < n_tiles; i++) {
        v_add_tile(new_grid);
    }

    return new_grid;
};



Visualize_grid* v_read_grid(char* input_file) {
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
    Visualize_grid* grid = _v_grid_init(max_size_x, size_y);
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



void v_add_tile(Visualize_grid* grid) {
    uint8_t x, y; 

    do {
        x = _v_random(grid->size_x);
        y = _v_random(grid->size_y);
    } while(!_v_is_space_available_for_tile(grid, x, y));

    grid->data[x][y] = _v_tile;
};


void v_add_agent(Visualize_grid* grid) {
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


void v_add_agents(Visualize_grid* grid, uint8_t n_agents) {
    for(int i = 0; i < n_agents; i++){
        v_add_agent(grid);
    }
}


void _v_init_agent(Visualize_grid* grid, Agent* agent) {
    //* set up the manhattan layout to destination
    uint8_t manhattan[grid->size_x][grid->size_y];

    //* init the path_weights
    Cell path_weights[grid->size_x][grid->size_y];

    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            manhattan[x][y] = _v_heuristic(x, y, agent->dest_x, agent->dest_y);
            if (grid->data[x][y] == _v_tile || (grid->data[x][y] >= 'A' && grid->data[x][y] <= 'Z'))
                manhattan[x][y] = 255;

            path_weights[x][y].is_visited = 0;
        }
    }

    //* setup the path_weights
    _v_setup_cells(grid, path_weights, manhattan, agent, 0, 0);

    agent->n_cells = 0;
    for (uint8_t x = agent->dest_x, y = agent->dest_y; x != agent->x || y != agent->y; (agent->n_cells)++) {
        switch (path_weights[x][y].came_from) {
            case 1:
                y++;
                break;
            case 2:
                x++;
                break;
            case 3:
                y--;
                break;
            case 4:
                x--;
                break;
        }
        if (grid->data[x][y] == grid->data[agent->dest_x][agent->dest_y])
            break;
    }

    agent->path_directions = malloc(agent->n_cells * sizeof(uint8_t));

    for (uint8_t x = agent->dest_x, y = agent->dest_y, i = 0; x != agent->x || y != agent->y; i++) {
        switch (path_weights[x][y].came_from) {
            case 1:
                y++;
                agent->path_directions[i] = 3;
                break;
            case 2:
                x++;
                agent->path_directions[i] = 4;
                break;
            case 3:
                y--;
                agent->path_directions[i] = 1;
                break;
            case 4:
                x--;
                agent->path_directions[i] = 2;
                break;
        }
        if (grid->data[x][y] == grid->data[agent->dest_x][agent->dest_y])
            break;
    }
}



/*
    * no direction available - 0;
    * up - 1;
    * right - 2;
    * down - 3;
    * left - 4;
*/
uint8_t _v_setup_cells(Visualize_grid* grid, Cell path_weights[grid->size_x][grid->size_y], uint8_t manhattan[grid->size_x][grid->size_y], Agent* agent, int8_t x, int8_t y) {
    if(path_weights[x][y].is_visited == 1 || !_v_is_valid_cell(x, y, grid)) return UINT8_MAX;
    if(agent->dest_x == x && agent->dest_y == y) return 0; //* goal
    if(_v_is_cell_occupied(grid, x, y)) return UINT8_MAX;
    path_weights[x][y].is_visited = 1;
    Cell best_cell = {0, 0, 1};
    uint8_t curr_weight;

    //* prime the cell
    best_cell.weight = _v_setup_cells(grid, path_weights, manhattan, agent, x, y + 1) + manhattan[x][y];
    best_cell.came_from = 1;

    if(  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x + 1, y) + manhattan[x][y])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight;
        best_cell.came_from = 2;
    }
    if(  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x, y - 1) + manhattan[x][y])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight;
        best_cell.came_from = 3;
    }
    if(  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x - 1, y) + manhattan[x][y])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight;
        best_cell.came_from = 4;
    }

    path_weights[x][y] = best_cell;
    return best_cell.weight;
}


int8_t v_solve_grid_one_step(Visualize_grid* grid) {
    int8_t n_agents_left = grid->n_agents;
    uint8_t dx[] = {0, -1, 0, 1, 0};  // Delta rows for up, right, down, left
    uint8_t dy[] = {0, 0, 1, 0, -1};  // Delta cols for up, right, down, left

    Agent agents[grid->n_agents];

    //* setup agents' positions and destinations
    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            if (grid->data[x][y] >= 'a' && grid->data[x][y] <= 'z') {
                agents[grid->data[x][y] - 'a'].x = x;
                agents[grid->data[x][y] - 'a'].y = y;
            } else if (grid->data[x][y] >= 'A' && grid->data[x][y] <= 'Z') {
                agents[grid->data[x][y] - 'A'].dest_x = x;
                agents[grid->data[x][y] - 'A'].dest_y = y;
            }
        }
    }

    //* perform one step for each agent
    for (uint8_t i = 0; i < grid->n_agents; i++) {
        _v_init_agent(grid, &agents[i]);
        uint8_t new_x = agents[i].x + dx[agents[i].path_directions[agents[i].n_cells]];
        uint8_t new_y = agents[i].y + dy[agents[i].path_directions[agents[i].n_cells]];;
        printf("next expected move : %d\n", agents[i].path_directions[agents[i].n_cells]);
        printf("new_x,y -> %d %d\n", new_x, new_y);
        (agents[i].n_cells)--;


        //* check if the new position is valid and not occupied by an obstacle or another agent
        if (_v_is_valid_cell(new_x, new_y, grid) && grid->data[new_x][new_y] != _v_tile) {
            if (grid->data[new_x][new_y] == _v_free) {
                //* move the agent to the new position
                grid->data[agents[i].x][agents[i].y] = _v_free;
                grid->data[new_x][new_y] = 'a' + i;
                agents[i].x = new_x;
                agents[i].y = new_y;
            } else if (new_x == agents[i].dest_x && new_y == agents[i].dest_y) {
                    grid->data[new_x][new_y] = 'x';
                    n_agents_left--;
            } else {
                //* check if the agent can wait for the other agent to move
                uint8_t otherAgentIndex = grid->data[new_x][new_y] - 'a';
                if (agents[otherAgentIndex].path_directions[agents[i].n_cells] == (agents[i].path_directions[agents[i].n_cells] + 2) % 4) {
                    agents[otherAgentIndex].path_directions[agents[i].n_cells] = 0; //* to stop him from making 1 future move 
                    //* move the agent to the new position
                    grid->data[agents[i].x][agents[i].y] = _v_free;
                    grid->data[new_x][new_y] = 'a' + i;
                    agents[i].x = new_x;
                    agents[i].y = new_y;
                }
            }
        }
    }

    return n_agents_left;
}

uint8_t _v_is_cell_occupied(Visualize_grid* grid, uint8_t x, uint8_t y) {
    return grid->data[x][y] == _v_tile || (grid->data[x][y] >= 'A' && grid->data[x][y] <= 'Z') || (grid->data[x][y] >= 'a' && grid->data[x][y] <= 'z');
}
uint8_t _v_heuristic(int8_t start_row, int8_t start_col, int8_t dest_row, int8_t dest_col) {
    return abs(start_row - dest_row) + abs(dest_col - start_col);
}

int _v_is_valid_cell(int8_t x, int8_t y, Visualize_grid* grid) {
    return x >= 0 && x < grid->size_x && y >= 0 && y < grid->size_y;
}


int _v_is_space_available_for_tile(Visualize_grid* grid, uint8_t x, uint8_t y) {
    if(grid->data[x][y] == _v_tile) return 0;

    grid->data[x][y] = _v_tile; //* a bug may occur if later the tile is not placed but in the current implementation that is not a problem
    if(_v_are_spaces_connected(grid)) return 1;
    grid->data[x][y] = _v_free;

    return 0;
};


//* quality of life function
uint8_t _v_random(uint8_t max) {
    return rand() % max;
}


void _v_fill(Visualize_grid* grid, uint8_t x, uint8_t y) {
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

int _v_are_spaces_connected(Visualize_grid* grid) {
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
