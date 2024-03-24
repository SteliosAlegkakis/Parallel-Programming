<h1>Parallel implementation of Pagerank</h1>

<h2>Description</h2>

<p>In this assignment, I developed a parallel program for graph processing. The program reads a graph as a list of edges stored in a text file, where each line describes an edge in the form "src dst," meaning the number of the node where the edge starts and the number of the node where the edge ends. The numbers are 64-bit integers. The program concurrently calculates the PageRank of each node by executing the PageRank algorithm for 50 iterations. All nodes are initialized with a PageRank value of 1.0. I used C language and C's pthread API. The number of threads that will execute the algorithm and the file name are given to the program as command line arguments. The results are stored in the pagerank.csv file</p>

<h2>Run the program</h2>
<h3>1 - make all</h3>
<h3>2 - ./pagerank file_path num_of_threads</h3>
