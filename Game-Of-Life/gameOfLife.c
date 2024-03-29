#include "gameOfLife.h"

int main(int argc,char** argv) {

    int rows, columns, generations = atoi(argv[2]);
    char** array = read_file(argv[1], &rows, &columns);
    FILE* output = fopen(argv[3],"w");
    numOfThreads = atoi(argv[4]);

    game_of_life_parallel(array, columns, rows, generations);
    
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            fprintf(output, "|%c", array[i][j]);
        }
        fprintf(output, "|\n");
    }

}

void game_of_life_parallel(char **array, int columns, int rows, int generations) {

    char** newArray = make_array(columns, rows);
    int numNeighbors,currentGen = 0, i = 0 , j = 0;

    #ifdef _OPENMP
    omp_set_dynamic(0);
    omp_set_num_threads(numOfThreads);
    #endif
    for(currentGen = 0; currentGen < generations; currentGen++){

        #pragma omp parallel for private(i, j, numNeighbors)
        for(i = 0; i < rows; i++){
            for(j = 0; j < columns; j++){
                numNeighbors = count_living_neighbors(i, j, array, rows, columns);
                if( numNeighbors == 3 ) newArray[i][j] = alive;
                else if( numNeighbors == 2 && array[i][j] == alive) newArray[i][j] = alive;
                else newArray[i][j] = dead;
            }
        }
        
        copy_array_parallel(array, newArray, rows, columns);
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

char** make_array(int columns, int rows) {

    char** newArray = (char**)malloc(rows * sizeof(char*));
    check_memory_ptr(newArray);

    for(int i = 0; i < rows; i++){
        newArray[i] = (char*) malloc(columns * sizeof(char));
        check_memory_ptr(newArray[i]);
    }

    return newArray;

}

void copy_array_parallel(char** dst, char** src, int rows, int columns) {

    int i = 0;

    #pragma omp parallel for private (i)
    for(i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            dst[i][j] = src[i][j];
        }
    }
    
}

char** read_file(const char* fileName, int* _rows, int* _columns) {

    FILE* file = fopen(fileName,"r");
    check_file_ptr(file,fileName);

    int rows, columns;
    fscanf(file, "%d %d", &columns, &rows);

    char** array = make_array(columns, rows);

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

void check_file_ptr(FILE* filePtr, const char* fileName) {

    if(filePtr == NULL) { 
        printf("Could not open file %s",fileName); 
        exit(EXIT_FAILURE); 
    }

}

void check_memory_ptr(void* ptr) {

    if(ptr == NULL) {
        printf("Could not allocate memory");
        exit(EXIT_FAILURE);
    }

}