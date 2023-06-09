typedef struct {
    uint8_t size_x;
    uint8_t size_y;
    char** data;

    uint8_t n_agents;
} visualize_grid;

//* PUBLIC
void v_draw(visualize_grid* grid);
visualize_grid* v_generate_grid(uint8_t size_x, uint8_t size_y, uint8_t n_tiles);
void v_free_grid(visualize_grid* grid);

visualize_grid* v_read_grid(char* input_file);

//** adding stuff to the grid
void v_add_tile(visualize_grid* grid);
void v_add_agent(visualize_grid* grid);
void v_add_agents(visualize_grid* grid, uint8_t n_agents);

//** solving the maze 
//TODO maybe a list of top 10 best paths from A*
int v_solve_grid_one_step(visualize_grid* grid); //* returns n_agents not in their path


//* PRIVATE

//** generating a map
int _v_is_space_available_for_tile(visualize_grid* grid, uint8_t x, uint8_t y);
uint8_t _v_random(uint8_t max);


//** validate grid
int _v_are_spaces_connected(visualize_grid* grid);
void _v_fill(visualize_grid* grid, uint8_t x, uint8_t y);
