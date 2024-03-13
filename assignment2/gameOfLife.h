#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define alive '*'
#define dead  ' '
#define bar '|'
#define new_line '\n'

int numOfThreads;

char** read_file(const char* filename, int* rows, int* columns);
void game_of_life_parallel(char** array, int columns, int rows, int generations);
void game_of_life_serial(char** array, int columns, int rows, int generations);
int count_living_neighbors(int row, int col, char** array, int numOfRows, int numOfCols);
char** make_array(int columns, int rows);
void copy_array_parallel(char** dst, char** src, int size);
void check_file_ptr(FILE* filePtr, const char* fileName);
void check_memory_ptr(void* ptr);