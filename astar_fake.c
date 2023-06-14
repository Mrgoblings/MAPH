#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int check_valid(uint8_t cell_val){
    if(cell_val == ' ')
        return 1;
    return 0;
}


void find_path(uint8_t matrix[3][3]){
    if(found && found2)
        return;

    int destination_x = -1;
    int destination_y = -1;
    int start_x = -1;
    int start_y = -1;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(matrix[j][i] == 'a'){
                start_x = j;
                start_y = i;
            }
            if(matrix[j][i] == 'A'){
                destination_x = j;
                destination_y = i;
            }
        }
    }

    if(start_x + 1 == destination_x && start_y == destination_y){
        
    }

    if(start_x < destination_x){
        
        if(check_valid(matrix[start_y][start_x + 1])){
            matrix[start_y][start_x++] = ' ';
            matrix[start_y][start_x] = 'a';
        }
    }



    // printf("Coords start: [%d,%d]\n", start_x, start_y);
    // printf("Coords destination: [%d,%d]\n", destination_x, destination_y);


}  


int main(){
    uint8_t matrix[3][3];

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matrix[j][i] = ' ';
        }
    }

    matrix[1][1] = '#'; // #
    matrix[0][2] = 'a'; // a
    matrix[2][0] = 'A'; // A

    find_path(matrix, 0, 0);

}