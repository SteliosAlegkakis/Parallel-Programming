#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define alive '*'
#define dead  ' '
#define bar '|'
#define new_line '\n'
#define NUM_THREADS 4

char** read_file(const char* filename, int* rows, int* columns);
int count_living_neighbors(int row, int col, char** array, int numOfRows, int numOfCols);
void check_file_ptr(FILE* filePtr, const char* fileName);
void check_memory_ptr(void* ptr);