#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"


int main() {
    visualize_grid* grid = v_generate_grid(7, 7, 100);
    
    if(grid == NULL) return 1;
    
    
    v_draw(grid);

    v_free_grid(grid);
    return 0;
}
