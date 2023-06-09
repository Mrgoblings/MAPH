#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"


int main() {
    //* Seed the random number generator
    srand(time(NULL));
    
    visualize_grid* grid = v_generate_grid(12, 12, 10);
    // visualize_grid* grid = v_read_grid("./maps/map_4");
    
    if(grid == NULL) return 1;

    v_add_agents(grid, 5);

    v_draw(grid);

    printf("size_x = %d\n", grid->size_x);
    printf("size_y = %d\n", grid->size_y);

    v_free_grid(grid);
    return 0;
}
