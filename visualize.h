#include <stdint.h>


/*
    * no direction available - 0;
    * up - 1;
    * right - 2;
    * down - 3;
    * left - 4;
*/


typedef struct {
    uint8_t weight;
    uint8_t came_from;
    uint8_t is_visited;
} Cell;


//* used in _v_get_agent_positions()
typedef struct {
    uint8_t x;
    uint8_t y;

    uint8_t dest_x;
    uint8_t dest_y;


    uint8_t* path_directions;
    uint8_t n_cells;
} Agent;

typedef struct {
    uint8_t size_x;
    uint8_t size_y;
    char** data;

    uint8_t n_agents;
} Visualize_grid;


//* PUBLIC
void v_draw(Visualize_grid* grid);
Visualize_grid* v_generate_grid(uint8_t size_x, uint8_t size_y, uint8_t n_tiles);
void v_free_grid(Visualize_grid* grid);

Visualize_grid* v_read_grid(char* input_file);

//** adding stuff to the grid
void v_add_tile(Visualize_grid* grid);
void v_add_agent(Visualize_grid* grid);
void v_add_agents(Visualize_grid* grid, uint8_t n_agents);

//** solving the maze 
int8_t v_solve_grid_one_step(Visualize_grid* grid); //* returns n_agents not in their destination


//* PRIVATE

//** generating a map
int _v_is_space_available_for_tile(Visualize_grid* grid, uint8_t x, uint8_t y);
uint8_t _v_random(uint8_t max);


//** validate grid
int _v_are_spaces_connected(Visualize_grid* grid);
void _v_fill(Visualize_grid* grid, uint8_t x, uint8_t y);


//** solving the maze
uint8_t _v_setup_cells(Visualize_grid* grid, Cell path_weights[grid->size_x][grid->size_y], uint8_t manhattan[grid->size_x][grid->size_y], Agent* agent, int8_t x, int8_t y);
uint8_t _v_heuristic(int8_t start_row, int8_t start_col, int8_t dest_row, int8_t dest_col);
int _v_is_valid_cell(int8_t x, int8_t y, Visualize_grid* grid);
uint8_t _v_is_cell_occupied(Visualize_grid* grid, uint8_t x, uint8_t y);