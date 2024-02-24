#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define DAMPING_FACTOR 0.85
#define INITIAL_PAGERANK_VALUE 1.0
#define MAX_ITERATIONS 50

typedef struct {
    long src;
    long dst;
} Edge;

typedef struct {
    long id;
    int incoming_edges_count;
    long *neighbors_from_incoming_edges;
    int ougoing_edges_count;
} Node;

Node *nodes = NULL;
Edge *edges = NULL;
int num_of_edges = 0;
int num_of_nodes = 0;
double *pagerank = NULL;
double *new_pagerank = NULL;
int NUM_OF_THREADS = 0;
pthread_barrier_t barrier;

//reads the file with the given filename and finds the number of nodes, 
//the number of edges and stores the edges in the edges array.
void readFile(char *_filename);
//finds the neighbors from incoming edges and from outcoming edges for each node
void findNeighbors();
//calculates the page rank for a subset of nodes depending on the number of threads
void* findPagerank(void *_id);
//checks if the given pointer is NULL and terminates the execution if so.
void checkAllocationStatus(void *_ptr);
//checks if the given file_ptr is NULL and terminates the execution if so.
void checkFileStatus(void *_file_ptr, char *_filename);