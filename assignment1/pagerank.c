#include "pagerank.h"

int main(int argc, char** argv){

    //--------------- initialize global variables -------------------

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
    
    //---------------- create threads --------------------------------

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

    //----------------- write output file -----------------------------

    FILE *pagerank_csv = fopen("pagerank.csv", "w");
    checkFileStatus(pagerank, "pagerank.csv");
    for (int i = 0; i < num_of_nodes; i++){
        fprintf(pagerank_csv, "%d, %f\n", i, pagerank[i]);
    }
    fclose(pagerank_csv);

    //---------------- free memory ------------------------------------ 

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

        for(int i = start; i <= end; i++){
            pagerank[i] = new_pagerank[i];
        }

        pthread_barrier_wait(&barrier);
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