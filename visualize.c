#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"

const char _v_free = ' ', _v_tmp = '.', _v_tile = '#', 
    _v_first_agent = 'a', _v_first_destination = 'A', _v_finished_agent = '0', _v_max_agents = 'Z' - 'A';

int8_t dy[] = {-1, 0, 1, 0},  //* Delta rows for up, right, down, left
       dx[] = {0, 1, 0, -1};  //* Delta cols for up, right, down, left


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
};


void v_free_grid(Visualize_grid* grid) {
    if(grid == NULL) return;
    
    for (uint8_t x = 0; x < grid->size_x; x++) {
        free(grid->data[x]);
    }
    free(grid->data);
    free(grid);
};


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
};



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
};



int8_t v_solve_grid_one_step(Visualize_grid* grid) {
    int8_t n_agents_left = grid->n_agents;

    Agent agents[grid->n_agents];

    //* setup agents' positions and destinations
    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            if (grid->data[x][y] >= _v_first_agent && grid->data[x][y] <= _v_first_agent + _v_max_agents) {

                agents[grid->data[x][y] - _v_first_agent].x = x;
                agents[grid->data[x][y] - _v_first_agent].y = y;
            } else if (grid->data[x][y] >= _v_first_destination && grid->data[x][y] <= _v_first_destination + _v_max_agents) {

                agents[grid->data[x][y] - _v_first_destination].dest_x = x;
                agents[grid->data[x][y] - _v_first_destination].dest_y = y;
            }
        }
    }


    //* perform one step for each agent
    for (uint8_t i = 0; i < grid->n_agents; i++) {

        _v_init_agent(grid, &agents[i]);
                                                        //* pop_back
        uint8_t new_x = agents[i].x + dx[agents[i].path_directions[agents[i].n_cells_left - 1]];
        uint8_t new_y = agents[i].y + dy[agents[i].path_directions[agents[i].n_cells_left - 1 ]];;

        printf("next expected move : %d\n", agents[i].path_directions[agents[i].n_cells_left - 1]);
        printf("new_x,y -> %d %d\n", new_x, new_y);
        (agents[i].n_cells_left)--;


        //* check if the new position is valid and not occupied by an obstacle or another agent
        if (_v_is_cell_in_grid(new_x, new_y, grid) && grid->data[new_x][new_y] != _v_tile) {
            if (grid->data[new_x][new_y] == _v_free) {

                //* move the agent to the new position
                grid->data[agents[i].x][agents[i].y] = _v_free;
                grid->data[new_x][new_y] = _v_first_agent + i;
                agents[i].x = new_x;
                agents[i].y = new_y;
            } else if (new_x == agents[i].dest_x && new_y == agents[i].dest_y) {
                    grid->data[new_x][new_y] = _v_finished_agent;
                    n_agents_left--;
            } else {
                //* check if the agent can wait for the other agent to move
                uint8_t otherAgentIndex = grid->data[new_x][new_y] - _v_first_agent;
                if (agents[otherAgentIndex].path_directions[agents[i].n_cells_left  - 1] == (agents[i].path_directions[agents[i].n_cells_left - 1] + 2) % 4) {
                    agents[otherAgentIndex].path_directions[agents[i].n_cells_left  - 1] = 0; //* to stop him from making 1 future move 
                    //* move the agent to the new position
                    grid->data[agents[i].x][agents[i].y] = _v_free;
                    grid->data[new_x][new_y] = _v_first_agent + i;
                    agents[i].x = new_x;
                    agents[i].y = new_y;
                }
            }
        } else {
            printf("\nInvalid new position (%d;%d) for agent %c.\n\n", new_x, new_y, _v_first_agent + i);
        }
    }

    printf("\n\n%d\n\n", agents[0].n_cells_left);

    for(int i = 0; i < grid->n_agents; i++) {
        printf("\nAGENT NAME: %d\n", i);           
        for(int i_c = 0; i_c < agents[i].n_cells_left; i_c++) {
            printf("%d ", agents[i].path_directions[i_c]);
        }   
        
        free(agents[i].path_directions);
    }
    puts("");


    return n_agents_left;
};


void _v_init_agent(Visualize_grid* grid, Agent* agent) {

    Cell path_cells[grid->size_x][grid->size_y];
    uint8_t manhattan[grid->size_x][grid->size_y];

    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            //* setup manhattan grid
            manhattan[x][y] = _v_heuristic(x, y, agent->dest_x, agent->dest_y);

            //* setup is_visited
            path_cells[x][y].is_visited = 0;
        }
    }
    printf("1\n");   

    //* setup the path_cells
    _v_setup_cells(grid, path_cells, manhattan, agent, agent->x, agent->y);
    printf("2\n");   


    //* get number of cells left --from the Destination to Agent--
    agent->n_cells_left = 0;
    for (uint8_t y = 0; y < grid->size_y; y++) {
        for (uint8_t x = 0; x < grid->size_x; x++) {
            printf("%d ", path_cells[x][y].best_next_direction);
        }
        puts("");
    }
    puts("");

    for (int16_t x = agent->x, y = agent->y; x != agent->dest_x || y != agent->dest_y; (agent->n_cells_left)++) {
        if(x < 0 || y < 0) printf("WENT OUTSIDE OF GRID!!\n");
        x += dx[path_cells[x][y].best_next_direction];
        y += dy[path_cells[x][y].best_next_direction];
        printf("x, y -> %d;%d\n", x, y);   
        printf("dx, dy -> %d;%d & direction - %d\n", dx[path_cells[x][y].best_next_direction], dy[path_cells[x][y].best_next_direction], path_cells[x][y].best_next_direction);   
    }

    printf("n_cells_left = %d\n", agent->n_cells_left);


    //* malloc the needed memory to save the best path
    agent->path_directions = malloc(agent->n_cells_left * sizeof(uint8_t));

    //* setup path_directions
    for (
        int16_t x = agent->x, y = agent->y, i = agent->n_cells_left - 1; 
        x != agent->dest_x || y != agent->dest_y; 
        i--
    ) {
        if(x < 0 || y < 0) printf("WENT OUTSIDE OF GRID!!\n");
        
        x += dx[path_cells[x][y].best_next_direction];
        y += dy[path_cells[x][y].best_next_direction];

        //* the array is reversed so it will be easier to pop_back for a move
        agent->path_directions[i] = path_cells[x][y].best_next_direction;
    }

};


/*
    * up - 0;
    * right - 1;
    * down - 2;
    * left - 3;
*/
uint8_t _v_setup_cells(
    Visualize_grid* grid, 
    Cell path_weights[grid->size_x][grid->size_y], 
    uint8_t manhattan[grid->size_x][grid->size_y], 
    Agent* agent, 
    int8_t x, int8_t y
) {    
 
    if(!_v_is_cell_in_grid(x, y, grid) || path_weights[x][y].is_visited == 1) return UINT8_MAX - manhattan[x][y]; //* later it will be added the manhattan

    if(agent->dest_x == x && agent->dest_y == y) {
        path_weights[x][y].best_next_direction = 5; //* in the end
        return 0; //* goal
    }

    if(grid->data[x][y] == _v_tile) return UINT8_MAX - manhattan[x][y];


    path_weights[x][y].is_visited = 1;
    Cell best_cell = {UINT8_MAX, 6, 1};
    int curr_weight;

    if(_v_is_cell_in_grid(x + dx[0], y + dy[0], grid) &&  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x + dx[0], y + dy[0]) + manhattan[x + dx[0]][y + dy[0]])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight + manhattan[x + 1][y];
        best_cell.best_next_direction = 0;
    }
    if(_v_is_cell_in_grid(x + dx[1], y + dy[1], grid) &&  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x + dx[1], y + dy[1]) + manhattan[x + dx[1]][y + dy[1]])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight + manhattan[x + 1][y];
        best_cell.best_next_direction = 1;
    }
    if(_v_is_cell_in_grid(x + dx[2], y + dy[2], grid) &&  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x + dx[2], y + dy[2]) + manhattan[x + dx[2]][y + dy[2]])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight + manhattan[x][y - 1];
        best_cell.best_next_direction = 2;
    }
    if(_v_is_cell_in_grid(x + dx[3], y + dy[3], grid) &&  (curr_weight = _v_setup_cells(grid, path_weights, manhattan, agent, x + dx[3], y + dy[3]) + manhattan[x + dx[3]][y + dy[3]])  <  best_cell.weight  ) {
        best_cell.weight = curr_weight + manhattan[x - 1][y];
        best_cell.best_next_direction = 3;
    }

    path_weights[x][y] = best_cell;
    return best_cell.weight;
};


uint8_t _v_is_cell_occupied(Visualize_grid* grid, uint8_t x, uint8_t y) {
    return grid->data[x][y] == _v_tile || 
        (grid->data[x][y] >= _v_first_destination && grid->data[x][y] <= _v_first_destination+ _v_max_agents) || 
        (grid->data[x][y] >= _v_first_agent && grid->data[x][y] <= _v_first_agent + _v_max_agents);
};

uint8_t _v_heuristic(int8_t start_row, int8_t start_col, int8_t dest_row, int8_t dest_col) {
    return abs(start_row - dest_row) + abs(dest_col - start_col);
};

int _v_is_cell_in_grid(int8_t x, int8_t y, Visualize_grid* grid) {
    return x >= 0 && x < grid->size_x && y >= 0 && y < grid->size_y;
};


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
};


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
