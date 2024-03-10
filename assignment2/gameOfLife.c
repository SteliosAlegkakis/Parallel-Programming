#include "gameOfLife.h"

int main(int argc,char** argv){

    omp_set_num_threads(NUM_THREADS);

    int rows,columns;
    char** array = read_file(argv[1], &rows, &columns);
    FILE* output = fopen(argv[2],"w");

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            fprintf(output,"|%c%d",array[i][j],count_living_neighbors(i, j, array, rows, columns));
        }
        fprintf(output,"|\n");
    }

}

int count_living_neighbors(int row, int col, char** array, int numOfRows, int numOfCols) {
    int count = 0;
    if(col + 1 < numOfCols) count += (array[row][col+1] == alive);
    if(col - 1 >= 0) count += (array[row][col-1] == alive);
    if(row - 1 >= 0) {
        count += (array[row-1][col] == alive);
        if(col + 1 < numOfCols) count += (array[row-1][col+1] == alive);
        if(col - 1 >= 0) count += (array[row-1][col-1] == alive);
    }
    if(row + 1 < numOfRows) {
        count += (array[row+1][col] == alive);
        if(col + 1 < numOfCols) count+= (array[row+1][col+1] == alive);
        if(col - 1 >= 0) count += (array[row+1][col-1] == alive);
    }
    return count;
}


char** read_file(const char* fileName, int* _rows, int* _columns){

    FILE* file = fopen(fileName,"r");
    check_file_ptr(file,fileName);

    int rows, columns;
    fscanf(file, "%d %d", &columns, &rows);

    char** array = (char**) malloc(rows * sizeof(char*));
    check_memory_ptr(array);
    for(int i = 0; i < rows; i++){
        array[i] = (char*) malloc(columns * sizeof(char));
        check_memory_ptr(array[i]);
    }

    char c = fgetc(file);
    int i = 0, j = 0;
    do{
        c = fgetc(file);

        switch(c) {
            case bar:
                continue;
            case dead:
                array[i][j++] = dead;
                break;
            case alive:
                array[i][j++] = alive;
                break;
            case new_line:
                i++;
                j = 0;
                break;
            default:
                break;
        }

    }while(c != EOF);

    *_rows = rows;
    *_columns = columns;
    return array;
}

void check_file_ptr(FILE* filePtr, const char* fileName){
    if(filePtr == NULL) { 
        printf("Could not open file %s",fileName); 
        exit(EXIT_FAILURE); 
    }
}

void check_memory_ptr(void* ptr){
    if(ptr == NULL) {
        printf("Could not allocate memory");
        exit(EXIT_FAILURE);
    }
}