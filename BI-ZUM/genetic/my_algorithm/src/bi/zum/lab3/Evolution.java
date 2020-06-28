package bi.zum.lab3;

import cz.cvut.fit.zum.util.Pair;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.data.StateSpace;
import java.util.Random;
import org.openide.util.lookup.ServiceProvider;

import java.io.FileWriter;
import java.io.IOException;

/**
 * @author Jaroslav Urban
 */
@ServiceProvider(service = AbstractEvolution.class)
public class Evolution extends AbstractEvolution<Individual> implements Runnable {

    /**
     * start and final average fitness
     */
    private Pair<Double, Double> avgFitness;
    /**
     * start and final best fitness in whole population
     */
    private Pair<Double, Double> bestFitness;
    /**
     * start and final time
     */
    private Pair<Long, Long> time;
    /**
     * How often to print status of evolution
     */
    
    private Random rand = new Random();
    private boolean hpSearch = false;
    
    // HYPERPAREMETERS:
    private int tournamentSize = 75;
    private int tournamentWinners = 10;
    // how many nodes to add to the graph each generation
    private int nodesToAdd = 50;
    // probability of ending the mutating random walk at a given timestep
    private double endProbability = 0.9;
    // + populationSize
    // + generations
    // + mutationProbability


    /**
     * The population to be used in the evolution
     */
    Population population;

    public Evolution() {
        isFinished = false;
        avgFitness = new Pair<Double, Double>();
        bestFitness = new Pair<Double, Double>();
        time = new Pair<Long, Long>();        
    }

    @Override
    public String getName() {
        return "My algorithm";
    }

    // runs hyper paremeter search instead, if hpSearch is true
    @Override
    public void run(){
        if (hpSearch){
            HPSearch(200);
        } else {
            run(true);
        }
    }
    
    private void run(boolean verbose) {
        GraphState.init(nodesToAdd);

        // updates the number of generations if it is too low
        int minGens = StateSpace.nodesCount() / nodesToAdd;
        if (minGens > generations){
            setGenerations(minGens);
        }
        
        // only use even population size
        setPopulationSize(getPopulationSize() + getPopulationSize() % 2);
        
        // Initialize the population
        population = new Population(this, getPopulationSize());
        
        // Collect initial system time, average fitness, and the best fitness
        time.a = System.currentTimeMillis();
        avgFitness.a = population.getAvgFitness();
        AbstractIndividual best = population.getBestIndividual();
        bestFitness.a = best.getFitness();
        
        // Show on map
        updateMap(best);
        
        // decides how often to add new nodes
        int enlargeAfter = generations / (minGens + 2);
        if (enlargeAfter == 0){
            enlargeAfter = 1;
        }

        // Run evolution cycle for the number of generations set in GUI
        for(int g=1; g < generations; g++) {
            // the evolution may terminate from the outside using GUI button
            if (isFinished) {
                break;
            }

            // initialize the next generation's population
            ArrayList<AbstractIndividual> newInds = new ArrayList();
            
            // elitism: Preserve the best individual
            // (this is quite exploatory and may lead to premature convergence!)
            newInds.add(population.getBestIndividual().deepCopy());
            
            // selects half of the population through tournament selection
            while(newInds.size() < getPopulationSize() / 2) {
                
                // selection by a tournament format on the current state
                AbstractIndividual[] selected = population.selectIndividuals(tournamentSize, tournamentWinners);

                // copy selected individuals
                for (AbstractIndividual i: selected){
                    if (newInds.size() < getPopulationSize()) {
                        newInds.add(i.deepCopy());
                    }
                }
            }

            // adds new nodes to the state
            if (g % enlargeAfter == 0){
                GraphState.enlargeState(nodesToAdd);
            }

            // adds 2 mutations of each selected individual to the population
            for (int i = 0; i < getPopulationSize(); i++){
                Individual ind = (Individual) newInds.get(i % newInds.size()).deepCopy();
                ind.mutate(mutationProbability, endProbability, nodesToAdd);
                population.setIndividualAt(i, ind);
            }

            if (verbose){
                // print statistic
                System.out.println("gen: " + g + "\t bestFit: " + population.getBestIndividual().getFitness() + "\t avgFit: " + population.getAvgFitness());
                best = population.getBestIndividual();
                updateMap(best);
            }
            
            // updates map
            updateGenerationNumber(g);
        }

        // === END ===
        time.b = System.currentTimeMillis();
        population.sortByFitness();
        avgFitness.b = population.getAvgFitness();
        best = population.getBestIndividual();
        bestFitness.b = best.getFitness();
        updateMap(best);
        isFinished = true;
        if (verbose){
            System.out.println("Evolution has finished after " + ((time.b - time.a) / 1000.0) + " s...");
            System.out.println("avgFit(G:0)= " + avgFitness.a + " avgFit(G:" + (generations - 1) + ")= " + avgFitness.b + " -> " + ((avgFitness.b / avgFitness.a) * 100) + " %");
            System.out.println("bstFit(G:0)= " + bestFitness.a + " bstFit(G:" + (generations - 1) + ")= " + bestFitness.b + " -> " + ((bestFitness.b / bestFitness.a) * 100) + " %");
            System.out.println("bestIndividual= " + population.getBestIndividual());
            System.out.println("========== Evolution finished =============");
        }
    }
    
    private void HPSearch(int n_iterations){
        try {
            long unixTime = System.currentTimeMillis() / 1000L;
            FileWriter wr = new FileWriter(unixTime + ".csv");
            wr.write("End probability,"
                    + "Mutation probability,"
                    + "Best fitness,"
                    + "Average fitness,"
                    + "Number of generations,"
                    + "Tournament size,"
                    + "Tournament winners,"
                    + "Nodes to add,"
                    + "Population size,"
                    + "Time\n");
            for (int i = 0; i < n_iterations; i++){
                isFinished = false;
                setMutationProbability(rand.nextDouble());
                setGenerations(rand.nextInt(2000) + 10);
                setPopulationSize(rand.nextInt(800) + 10);
                tournamentSize = rand.nextInt(getPopulationSize() / 2) + 2;
                tournamentWinners = rand.nextInt(tournamentSize / 2) + 1;
                nodesToAdd = rand.nextInt(StateSpace.nodesCount() / 2) + 1;
                endProbability = 0.15 + 0.85 * rand.nextDouble();
                run(false);
                wr.write(endProbability + "," + 
                        mutationProbability + "," + 
                        bestFitness.b + "," + 
                        avgFitness.b + "," + 
                        generations + "," + 
                        tournamentSize + "," + 
                        tournamentWinners + "," + 
                        nodesToAdd + "," + 
                        getPopulationSize() + "," + 
                        ((time.b - time.a) / 1000.0) + "\n");
                System.out.printf("%02d "
                        + "End: %.2f "
                        + "Mutation: %.2f "
                        + "Best: %.2f "
                        + "Avg: %.2f "
                        + "Gens: %3d "
                        + "Siz: %3d "
                        + "Winners: %2d "
                        + "AddNodes: %2d "
                        + "popSize: %d "
                        + "time: %.2f\n", 
                        i, 
                        endProbability, 
                        mutationProbability, 
                        bestFitness.b, 
                        avgFitness.b, 
                        generations, 
                        tournamentSize, 
                        tournamentWinners, 
                        nodesToAdd, 
                        getPopulationSize(), 
                        ((time.b - time.a) / 1000.0));
            }
            wr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
}
