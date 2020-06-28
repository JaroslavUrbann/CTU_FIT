package bi.zum.lab3;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.data.Edge;
import cz.cvut.fit.zum.data.StateSpace;
import cz.cvut.fit.zum.util.Pair;
import cz.cvut.fit.zum.api.Node;
import java.util.Arrays;
import java.util.List;
import java.util.Random;


/**
 * @author JaroslavUrban
 */
public class Individual extends AbstractIndividual {

    private double fitness = Double.NaN;
    private AbstractEvolution evolution;
    
    private Random random;
    boolean[] genome;    

    /**
     * Creates a new individual
     * 
     * @param evolution The evolution object
     * @param randomInit <code>true</code> if the individual should be
     * initialized randomly (we do wish to initialize if we copy the individual)
     */
    public Individual(AbstractEvolution evolution, boolean randomInit) {
        this.evolution = evolution;
        
        this.random = new Random();
        this.genome = new boolean[evolution.getNodesCount()];
        
        if(randomInit) {
            
            for(int i=0; i<evolution.getNodesCount(); i++) {

                this.genome[i] = random.nextBoolean();
            }
            this.repair();
        }
    }

    @Override
    public boolean isNodeSelected(int j) {
        
        return this.genome[j];
    }

    /**
     * Evaluate the value of the fitness function for the individual. After
     * the fitness is computed, the <code>getFitness</code> may be called
     * repeatedly, saving computation time.
     */
    @Override
    public void computeFitness() {
        int result = 0;        
        for(Node n: GraphState.getNodes()) {
            if(!this.genome[n.getId()]) result ++;
        }
        
        this.fitness = result;
    }

    /**
     * Only return the computed fitness value
     *
     * @return value of fitness function
     */
    @Override
    public double getFitness() {
        if (Double.isNaN(this.fitness)){
            computeFitness();
        }
        return this.fitness;
    }

    /**
     * Does random changes in the individual's genotype, taking mutation
     * probability into account.
     * 
     * @param mutationRate Probability of a bit being inverted, i.e. a node
     * being added to/removed from the vertex cover.
     */
    @Override
    public void mutate(double mutationRate) {}
    
    public void mutate(double mutationRate, double endProb, int addedNodes) {

        List<Node> nodes = GraphState.getNodes();
        
        // iterates over the last few addedNodes
        for (int i = nodes.size() - addedNodes; i < nodes.size(); i++){
            Node n = nodes.get(i);
            // does a random walk starting at a given node
            while (random.nextDouble() > endProb){
                if (mutationRate > random.nextDouble()){
                    this.genome[i] = !this.genome[i];
                }
                // get next edge
                int j = random.nextInt(n.getEdges().size());
                while (!GraphState.isInState(n.getEdges().get(j).getFromId()) || 
                       !GraphState.isInState(n.getEdges().get(j).getToId())){
                    j = (j + 1) % n.getEdges().size();
                }
                Edge e = n.getEdges().get(j);
                // get next node
                if (e.getFromId() == n.getId()){
                    n = StateSpace.getNode(e.getToId());
                } else{
                    n = StateSpace.getNode(e.getFromId());
                }
            }
        }
        
        this.repair();
        this.computeFitness();
    }
    

    @Override
    public Pair crossover(AbstractIndividual other) {
        Pair<Individual,Individual> result = new Pair();
        return result;
    }

    
    /**
     * When you are changing an individual (eg. at crossover) you probably don't
     * want to affect the old one (you don't want to destruct it). So you have
     * to implement "deep copy" of this object.
     *
     * @return identical individual
     */
    @Override
    public Individual deepCopy() {
        Individual newOne = new Individual(evolution, false);
       
        System.arraycopy(this.genome, 0, newOne.genome, 0, this.genome.length);

        newOne.fitness = this.fitness;
        return newOne;
    }

   /**
    * Repairs the genotype to make it valid, i.e. ensures all the edges
    * are in the vertex cover.
    */
    private void repair() {        
        /* We iterate over all the edges */
        for(Edge e : GraphState.getEdges()) {
 
            if(!this.genome[e.getFromId()] && !this.genome[e.getToId()]) {
                if(random.nextBoolean()) {
                    this.genome[e.getFromId()] = true;
                }
                else {
                    this.genome[e.getToId()] = true;
                }
            }
        }
    }
    
    /**
     * Return a string representation of the individual.
     *
     * @return The string representing this object.
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        
        
        /* TODO: implement own string representation, such as a comma-separated
         * list of indices of nodes in the vertex cover
         */
        
        boolean first = true;
        for(int i=0; i<this.genome.length; i++) {
            if(this.genome[i]) {
                if(first) {
                    first = false;
                }
                else {
                    sb.append(",");
                }
                sb.append(i);
            }
        }

        return sb.toString();
    }
}
