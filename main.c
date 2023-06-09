#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"


int main() {
    //* Seed the random number generator
    srand(time(NULL));
    
    Visualize_grid* grid;
    // // visualize_grid* grid = v_read_grid("./maps/map_4");
    
    // if(grid == NULL) return 1;

    // v_add_agents(grid, 5);

    // v_draw(grid);
    // while(getc(stdin) == '\n') {
    //     v_solve_grid_one_step(grid);
    //     v_draw(grid);
    // }

    // // printf("size_x = %d\n", grid->size_x);
    // // printf("size_y = %d\n", grid->size_y);

    int choice;
    while (1) {
        printf("\nMenu\n");
        printf("1. Generate a map\n");
        printf("2. Use map from a file\n");
        printf("3. Try solving the map\n");
        printf("4. Add a tile\n");
        printf("5. Add an agent\n");
        printf("6. Exit\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                grid = v_generate_grid(17, 17, 10);
                break;
            case 2:
                grid = v_read_grid("./maps/map_4");
                break;
            case 3:
                v_solve_grid_one_step(grid);
                break;
            case 4:
                v_add_tile(grid);
                break;
            case 5:
                v_add_agent(grid);
                break;
            case 6:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
        v_draw(grid);
    }

    v_free_grid(grid);
    return 0;
}
