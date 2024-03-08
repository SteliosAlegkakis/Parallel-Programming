#include "gameOfLife.h"

int main(int argc,char** argv){
    int rows,columns;
    char** array = read_file(argv[1], &rows, &columns);
    printf("\n");
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("|%c",array[j][i]);
        }
        printf("|\n");
    }
    return 0;
}

char** read_file(const char* fileName, int* _rows, int* _columns){

    FILE* file = fopen(fileName,"r");
    check_file_ptr(file,fileName);

    int rows, columns;
    fscanf(file, "%d %d", &columns, &rows);
    printf("%d %d", rows, columns);

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
                array[i++][j] = dead;
                break;
            case alive:
                array[i++][j] = alive;
                break;
            case new_line:
                j++;
                i = 0;
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