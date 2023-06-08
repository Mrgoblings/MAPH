#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualise.h"


int main() {
    // visualize_grid* grid = v_generate_grid(7, 7, 10);
    visualize_grid* grid = v_read_grid("./maps/map_4");
    
    printf("drawng the map..\n");
    if(grid == NULL) return 1;
    printf("drawng the map..\n");
    
    v_draw(grid);

    v_free_grid(grid);
    return 0;
}
