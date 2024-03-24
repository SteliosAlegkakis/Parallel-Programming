<h1>Parallel implementation of <a href="https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life">Conway's game of life</a></h1>

<h2>Description</h2>

<p> The program reads a vector from a file as shown in the input.txt file, where the two numbers in the first line specify the number of rows and columns and the '*' and ' ' symbolize a living or a dead cell. The algorithm runs in parallel for a given number of threads and generations and stores the result in a given txt file. It can also be compiled as a serial program since it is built using OpenMP.</p>

<h2>Run the Parallel program</h2>
<h3>1 - make parallel</h3>
<h3>2 - ./a.out input_file_path num_of_generations output_file_path num_of_threads </h3>

<h2>Run the Serial program</h2>
<h3>1 - make serial</h3>
<h3>2 - ./a.out input_file_path num_of_generations output_file_path</h3>
