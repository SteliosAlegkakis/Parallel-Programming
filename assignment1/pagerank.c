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
//calculates the pagerank of each node in a serial way
void pagerankSerial();
//Normalizes the pagerank values
void normalizePagerank();

int main(int argc, char** argv){

    //---------------initialize global variables-------------------

    edges = malloc(sizeof(Edge));
    checkAllocationStatus(edges);
    readFile(argv[1]);

    pagerank = malloc(num_of_nodes* sizeof(double));
    checkAllocationStatus(pagerank);
    new_pagerank = malloc(num_of_nodes * sizeof(double));
    checkAllocationStatus(new_pagerank);
    nodes = malloc(num_of_nodes * sizeof(Node));
    checkAllocationStatus(nodes);
    for(int i = 0; i < num_of_nodes; i++) {
        new_pagerank[i] = INITIAL_PAGERANK_VALUE;
        pagerank[i] = INITIAL_PAGERANK_VALUE;
        nodes[i].ougoing_edges_count = 0;
        nodes[i].incoming_edges_count = 0;
        nodes[i].neighbors_from_incoming_edges = malloc(nodes[i].incoming_edges_count * sizeof(long));
        checkAllocationStatus(nodes->neighbors_from_incoming_edges);
    }

    findNeighbors();
    
    //----------------create threads--------------------------------

    NUM_OF_THREADS = atoi(argv[2]);
    pthread_t* threads = malloc(NUM_OF_THREADS * sizeof(pthread_t));
    checkAllocationStatus(threads);
    int *ids = malloc(NUM_OF_THREADS * sizeof(int));
    checkAllocationStatus(ids);
    pthread_barrier_init(&barrier, NULL, NUM_OF_THREADS);

    for(int i = 0; i < NUM_OF_THREADS; i++){
        ids[i] = i;
        pthread_create(&threads[i], NULL, findPagerank, (void*)&ids[i]);
    }

    for (int i = 0; i < NUM_OF_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);

    //-----------------write output file-----------------------------

    // pagerankSerial(); uncomment for the serial implementation
    normalizePagerank();

    printf("pagerank finished\n");
    FILE *pagerank_csv = fopen("pagerank.csv", "w");
    checkFileStatus(pagerank, "pagerank.csv");
    for (int i = 0; i < num_of_nodes; i++){
        fprintf(pagerank_csv, "%d, %.6f\n", i, pagerank[i]);
    }
    fclose(pagerank_csv);

    //----------------free memory------------------------------------ 

    free(edges);
    free(pagerank);
    free(new_pagerank);
    for(int i = 0; i < num_of_nodes; i++) free(nodes[i].neighbors_from_incoming_edges);
    free(nodes);
    free(threads);
    
    return 0;
}

void readFile(char* _filename){
    FILE *file_ptr;
    file_ptr = fopen(_filename, "r");
    checkFileStatus(file_ptr,_filename);

    char line[100];
    while (fgets(line, sizeof(line), file_ptr) != NULL) {
        if (line[0] == '#') {
            continue;
        } else {
            fseek(file_ptr, -strlen(line)-1, SEEK_CUR);
            break;
        }
    }

    long src, dst;
    while (fscanf(file_ptr, "%ld %ld,", &src, &dst) == 2){
        edges[num_of_edges].src = src;
        edges[num_of_edges].dst = dst;
        num_of_edges++;

        if (src > num_of_nodes) num_of_nodes = src;
        if (dst > num_of_nodes) num_of_nodes = dst;

        edges = realloc(edges, (num_of_edges + 1) * sizeof(Edge));
        checkAllocationStatus(edges);
    }
    num_of_nodes++; //node id's start from 0
    
    fclose(file_ptr);
}

void findNeighbors(){
    Edge edge;
    for(int i = 0; i < num_of_edges; i++){
        edge = edges[i];
        nodes[edge.src].ougoing_edges_count++;
        nodes[edge.dst].incoming_edges_count++;
        nodes[edge.dst].neighbors_from_incoming_edges = realloc(nodes[edge.dst].neighbors_from_incoming_edges,nodes[edge.dst].incoming_edges_count * sizeof(long));
        checkAllocationStatus(nodes->neighbors_from_incoming_edges);
        nodes[edge.dst].neighbors_from_incoming_edges[nodes[edge.dst].incoming_edges_count - 1] = edge.src;
    }
}

void* findPagerank(void* _id){
    int id = *(int*)_id;

    int nodesPerThread = num_of_nodes / NUM_OF_THREADS;
    int extraNodes = num_of_nodes % NUM_OF_THREADS;

    int start = id * nodesPerThread;
    int end = start + nodesPerThread - 1;

    if (id < extraNodes) {
        start += id;
        end += id + 1;
    } else {
        start += extraNodes;
        end += extraNodes;
    }

    double sum;
    for (int itter = 0; itter < MAX_ITERATIONS; itter++) {
        for(int cur_node = start; cur_node <= end; cur_node++){
            sum = 0.0; 
            for(int i = 0; i < nodes[cur_node].incoming_edges_count; i++){
                sum += pagerank[nodes[cur_node].neighbors_from_incoming_edges[i]] / (double)nodes[nodes[cur_node].neighbors_from_incoming_edges[i]].ougoing_edges_count;
            }
            new_pagerank[cur_node] = 0.15 + DAMPING_FACTOR * sum;
        }

        pthread_barrier_wait(&barrier);
        for(int i = start; i < end; i++){
            pagerank[i] = new_pagerank[i];
        }
    }

    pthread_exit(NULL);
}

void checkAllocationStatus(void *_ptr){
    if(_ptr == NULL){
        printf("Memory allocation failed.");
        exit(2);
    }
}

void checkFileStatus(void *_file_ptr, char *_filename){
    if(_file_ptr == NULL){
        printf("Error opening the file %s.\n",_filename);
        exit(1);
    }
}

void pagerankSerial(){

    double sum;
    for (int itter = 0; itter < MAX_ITERATIONS; itter++) {
        for(int cur_node = 0; cur_node < num_of_nodes; cur_node++){
            sum = 0.0; 
            for(int i = 0; i < nodes[cur_node].incoming_edges_count; i++){
                sum += pagerank[nodes[cur_node].neighbors_from_incoming_edges[i]] / (double)nodes[nodes[cur_node].neighbors_from_incoming_edges[i]].ougoing_edges_count;
            }
            new_pagerank[cur_node] = 0.15 + DAMPING_FACTOR * sum;
        }

        for(int i = 0; i < num_of_nodes; i++){
            pagerank[i] = new_pagerank[i];
        }
    }

}

void normalizePagerank(){
    double min_pagerank = pagerank[0];
    double max_pagerank = pagerank[0];

    for (int i = 0; i < num_of_nodes; i++) {
        if (pagerank[i] < min_pagerank) {
            min_pagerank = pagerank[i];
        }
        if (pagerank[i] > max_pagerank) {
            max_pagerank = pagerank[i];
        }
    }

    if(min_pagerank != max_pagerank){
        for (int i = 0; i < num_of_nodes; i++) {
            pagerank[i] = (pagerank[i] - min_pagerank) / (max_pagerank - min_pagerank);
        }
    }
}