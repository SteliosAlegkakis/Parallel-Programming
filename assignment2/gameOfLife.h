#include <stdio.h>
#include <stdlib.h>
#define alive '*'
#define dead  ' '
#define bar '|'
#define new_line '\n'

char** read_file(const char* filename, int* rows, int* columns);
void check_file_ptr(FILE* filePtr, const char* fileName);
void check_memory_ptr(void* ptr);