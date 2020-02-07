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
    private int debugLimit = 100;
    private Random rand = new Random();
    private boolean hpSearch = false;
    private int earlyStoppingThreshold = 900;
    private int tournamentSize = 10;
    private int tournamentWinners = 2;
    private int nodesToAdd = 1;
    private double endProbability = 0.5;
    
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
        return "My evolution";
    }

    @Override
    public void run(){
        if (hpSearch){
            HPSearch(100);
        } else {
            run(true);
        }
    }
    
    private void run(boolean verbose) {
        GraphState.init(nodesToAdd);

        // only use even population size
        if (populationSize % 2 == 1){
            populationSize++;
        }
        
        // Initialize the population
        population = new Population(this, populationSize);
        
        // Collect initial system time, average fitness, and the best fitness
        time.a = System.currentTimeMillis();
        avgFitness.a = population.getAvgFitness();
        AbstractIndividual best = population.getBestIndividual();
        bestFitness.a = best.getFitness();
        
        // Show on map
        updateMap(best);
        
        double currentBestFitness = bestFitness.a;
        int bestFitnessGeneration = 0;
                
        // Run evolution cycle for the number of generations set in GUI
        for(int g=0; g < generations; g++) {
            // the evolution may be terminate from the outside using GUI button
            // early stopping
            if (isFinished || 
                    bestFitnessGeneration + earlyStoppingThreshold < g ||
                    GraphState.getNodes().size() == StateSpace.nodesCount()) {
                System.out.println("yw " + GraphState.getNodes().size() + " " + StateSpace.nodesCount());
                break;
            }
            
            // initialize the next generation's population
            ArrayList<AbstractIndividual> newInds = new ArrayList();
            
            // elitism: Preserve the best individual
            // (this is quite exploatory and may lead to premature convergence!)
            newInds.add(population.getBestIndividual().deepCopy());

            // keeping track of the generation of the absolute best individual for early stopping purposes
            if (newInds.get(0).getFitness() > currentBestFitness){
                currentBestFitness = newInds.get(0).getFitness();
                bestFitnessGeneration = g;
            }
            
            // select the better half of the population
            while(newInds.size() < populationSize / 2) {
                
                // selection by a tournament format on the current state
                AbstractIndividual[] selected = population.selectIndividuals(tournamentSize, tournamentWinners);

                // copy selected individuals
                for (AbstractIndividual i: selected){
                    if (newInds.size() < populationSize) {
                        newInds.add(i.deepCopy());
                    }
                }
            }

            GraphState.enlargeState(nodesToAdd);

            for (int i = 0; i < populationSize; i++){
                Individual ind = (Individual) newInds.get(i % newInds.size()).deepCopy();
                ind.mutate(mutationProbability, endProbability, nodesToAdd);
                population.setIndividualAt(i, ind);
            }

            if (verbose){
                // print statistic
                System.out.println("gen: " + g + "\t bestFit: " + population.getBestIndividual().getFitness() + "\t avgFit: " + population.getAvgFitness());
            }

            if (verbose && g % debugLimit == 0) {
                best = population.getBestIndividual();
                updateMap(best);
            }
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
            wr.write("Crossover probability,Mutation probability,Best Fitness,Average Fitness,Number of generations\n");
            for (int i = 0; i < n_iterations; i++){
                setCrossoverProbability(rand.nextDouble());
                setMutationProbability(rand.nextDouble());
                setGenerations(rand.nextInt(4000) + 1);
                run(false);
                isFinished = false;
                wr.write(crossoverProbability + "," + mutationProbability + "," + bestFitness.b + "," + avgFitness.b + "," + generations + "\n");
                System.out.printf("%02d Cross: %.2f Mutation: %.2f Best: %.2f Avg: %.2f Gens: %d\n", i, crossoverProbability, mutationProbability, bestFitness.b, avgFitness.b, generations);
            }
            wr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
}
