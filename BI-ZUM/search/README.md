### Search Algorithms
In this project, I implemented 6 search algorithms:
- Random Search
- DFS
- BFS
- Greedy Search
- Dijkstra's Algorithm
- A*

Since these algorithms only differ in what nodes they expand first, I wrote a shared class \
that is used by all of them and then implemented a node selection method for each algorithm.

I wrote these algorithms in python in a Jupyter notebook which can also be found on GoogleColab [here](https://colab.research.google.com/drive/1UCehkx7nPLR_yzcugLxESsXUfncOh25X?pli=1#forceEdit=true&sandboxMode=true). \
It is the preferred way of running this notebook.

The notebook first downloads a collection of mazes to test the algorithm on. \
They can be seen in the file browser panel on the left. \
Mazes can be changed in the third cell in the FILENAME variable along with other parameters of the notebook. \
After running the first few cells, each algorithm can be run by running its cell and then clicking on Step or Run. \
The algorithms cannot be run in parallel so you have to wait for the algorithm to finish before running another cell.