


void _v_init_agent(Visualize_grid* grid, Agent* agent) {
    //* set up the manhattan layout to destination
    uint8_t manhattan[grid->size_x][grid->size_y];

    //* init the path_cells
    Cell path_cells[grid->size_x][grid->size_y];

    for (uint8_t x = 0; x < grid->size_x; x++) {
        for (uint8_t y = 0; y < grid->size_y; y++) {
            //* setup manhattan grid
            manhattan[x][y] = _v_heuristic(x, y, agent->dest_x, agent->dest_y);

            //* setup is_visited
            path_cells[x][y].is_visited = 0;
        }
    }

    //* setup the path_cells
    _v_setup_cells(grid, path_cells, manhattan, agent, agent->dest_x, agent->dest_y);


    //* get n_cells_left size
    agent->n_cells_left = 0;
    for (uint8_t x = agent->dest_x, y = agent->dest_y; x != agent->x || y != agent->y; (agent->n_cells_left)++) {
        break;

        x += dx[path_cells[x][y].came_from];
        y += dy[path_cells[x][y].came_from];
    }

    printf("n_cells_left = %d\n", agent->n_cells_left);


    //* malloc the needed memory
    agent->path_directions = malloc(agent->n_cells_left * sizeof(uint8_t));

    //* setup path_directions
    for (
        int16_t x = agent->dest_x, y = agent->dest_y, i = 0; 
        x != agent->x || y != agent->y; 
        i++
    ) {
        if(x < 0 || y < 0) printf("WENT OUTSIDE OF GRID!!\n");
        
        x += dx[path_cells[x][y].came_from];
        y += dy[path_cells[x][y].came_from];

        agent->path_directions[i] = (path_cells[x][y].came_from + 2) % 4;
    }
}




/*
    * no direction available - 0;
    * up - 1;
    * right - 2;
    * down - 3;
    * left - 4;
*/

uint8_t _v_setup_cells(
    Visualize_grid* grid, 
    Cell path_weights[grid->size_x][grid->size_y], 
    uint8_t manhattan[grid->size_x][grid->size_y], 
    Agent* agent, 
    int8_t x, int8_t y
) {    

    if(path_weights[x][y].is_visited == 1 || !_v_is_valid_cell(x, y, grid)) return UINT8_MAX;
    if(agent->x == x && agent->y == y) return 0; //* goal
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