#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "visualize.h"
#include "priority_queue/priority_queue.h"


int main() {
    //* Seed the random number generator
    srand(time(NULL));

    Visualize_grid* grid = NULL;

    int choice;
    uint8_t size_x, size_y, n_tiles;
    char input[100];
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
                printf("Enter size_X of the grid: ");
                scanf("%hhu", &size_x);

                printf("Enter size_Y of the grid: ");
                scanf("%hhu", &size_y);

                printf("Enter number of tiles for the grid: ");
                scanf("%hhu", &n_tiles);
                v_free_grid(grid);
                grid = v_generate_grid(size_x, size_y, n_tiles);
                break;
            case 2:
                int map_number = 0;
                do{
                    printf("Enter map number (0-5): ");
                    scanf("%d", &map_number);
                } while(map_number < 0 || map_number > 5);

                char map_file[20];
                sprintf(map_file, "./maps/map_%d", map_number);

                v_free_grid(grid);
                grid = v_read_grid(map_file);
                break;
            case 3:
                if (grid != NULL) {
                    v_solve_grid_one_step(grid);
                } else {
                    printf("No grid available. Please generate or load a map.\n");
                }
                break;
            case 4:
                if (grid != NULL) {
                    v_add_tile(grid);
                } else {
                    printf("No grid available. Please generate or load a map.\n");
                }
                break;
            case 5:
                if (grid != NULL) {
                    v_add_agent(grid);
                } else {
                    printf("No grid available. Please generate or load a map.\n");
                }
                break;
            case 6:
                printf("Exiting...\n");
                v_free_grid(grid);
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

        if (grid != NULL) v_draw(grid);
    }

    return 0;
}

