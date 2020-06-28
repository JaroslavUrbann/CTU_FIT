### Genetic algorithm for the vertex cover problem

#### Assignment description
The task is to develop a genetic algorithm that is able to solve the vertex cover problem.  
I started working on this project before the coronavirus shutdown and before the assignment specification got changed,  
which is why I approached this project in a looser way, rather than implementing a standard out-of-the-box algorithm, which is what I would have done now.  

#### Methodology & Algorithms
Firstly, I ran a hyper-parameter search on the default algorithm that is provided with the template. I then analyzed the results to get a sense of where the algorithm is lacking and developed my own algorithm based on that. I then ran a second hyper-parameter search on my own algorithm and finally, I tested both of them on a different graph and compared their performances.  

#### Analysing the default algorithm
Note that all the following text takes into account that there is a bug on the line 98 of the default algorithm, where the '<' sign should be '>'.  
![Image of the possible bug](images/possible_bug.png?raw=true "Possible bug")  

The default algorithm has 2 main hyperparameters - mutation probability and crossover probability.  
After performing 100 runs of the algorithm with these set randomly, I've found out that the absolute best performance is achieved when the mutation probability is set to 1 and the crossover probability is set to 0.  
![Image of the HPS results](images/HPS_results.png?raw=true "The algorithm does significantly better with mutation probability = 1 and crossover probability = 0")  
With respect to the algorithm itself, these parameters make no sense at all.
Crossover probability being set to 0 means that all individuals are being evolved separately without any information transfer and mutation being set to 1 means that at each step, all nodes of each individual are mutated with 100% probability, which effectively just performs a bit negation of the genotype.  

I didn't understand how the algorithm was performing so well without actually doing anything when set to these values.  
Then I realized that there is an another "silent" part of the algorithm that repairs the genotype after each mutation.  
The repair mechanism goes through all the edges after each mutation and if there is an edge that isn't covered by either of its vertices, it randomly chooses one of them and covers it.  
With this mechanism at play, the algorithm was in fact randomly generating new vertex covers at each step in a smart way.  
To confirm this theory, I implemented an algorithm that would do exactly that. It would only use the repair mechanism to generate new individuals without any evolutionary techniques. I'll come back to that later.  

#### Designing my algorithm
When looking through the default algorithm I felt like it wasn't properly using the fact that the genotype is a graph. I focused on the fact that the mutation is just done randomly and without any concern for what mutating a gene might mean for the adjacent edges or nodes. If a node is mutated (flipped), then the nodes nearby should be the next candidates for mutation, rather than some unrelated nodes far away.  
I made a new genotype representation which is a graph that is expanded every couple of generations by adding a few new vertices using bfs.  
Mutation is done by going through the last few added vertices and starting a random walk at each given vertex.  
The random walk just randomly selects one of the neighbor nodes at each step and has some probability of mutating the current node and some probability of terminating.  
At each step of the evolution process, I use tournament selection to pick half of the population and make 2 mutated copies of each selected individual.
#### Results
I also ran a hyperparameter search for my algorithm. The results weren't as interesting but they helped me pick the best parameters. So far, I've only been using the island map for all my experiments, but towards the end of the project I also found out that there are other maps available and decided to use the earth one for a comparison between the algorithms I've discussed.  
I ran each of the algorithms for 2000 generations 5 times, where each run of each algorithm took approximately 60 seconds. Here are the results:  

| Algorithm     | Avg. Fittness   |  
| :------------ | :-------------- |  
| random        | 2162 ± 4.1      |  
| default       | 2455 ± 8.3      |  
| only repair   | 2575 ± 3.4      |  
| mine          | **2886 ± 12.1** |  
  
The parameters that were used were:  
2000 generations, 130 population size for all algorithms.  
50% mutation probability for my algorithm with some other parameters that are set in the code by default (in Population.java)  
(I don't know how to make them appear in the menu).  
100% mutation probability and 0% crossover probability for the default algorithm.  
The random and only repair algorithms don't have any additional parameters.  

#### Conclusions  
Although my algorithm achieves higher fitness than the default algorithm, it is only by a relatively small margin. I realized that this improvement likely comes from the fact that it uses domain knowledge which essentially goes against the spirit of evolutionary algorithms, which are supposed to be as general as possible so that they can be used almost anywhere.

### Repository overview

**folders ending with "_algorithm"** -  algorithms that I used in this project  
**HPS_default.csv, HPS_mine.csv** - data gathered in the hyperparameter searches  
**HPS_results_default.ipynb, HPS_results_mine.ipynb** - jupyter notebooks with graphs analysing the data  
**results.ods** - fitness of the 5 runs of each algorithm  