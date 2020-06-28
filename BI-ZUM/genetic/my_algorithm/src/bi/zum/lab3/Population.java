package bi.zum.lab3;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.api.ga.AbstractPopulation;
import cz.cvut.fit.zum.data.StateSpace;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

/**
 * @author Jaroslav Urban
 */
public class Population extends AbstractPopulation {

    private Random random;
    
    public Population(AbstractEvolution evolution, int size) {
        individuals = new Individual[size];
        for (int i = 0; i < individuals.length; i++) {
            individuals[i] = new Individual(evolution, true);
        }
        this.random = new Random();
    }

    /**
     * Method to select individuals from population
     *
     * @param tSize Tournament size
     * @param tWinners The number of individuals to be selected
     * @return List of selected individuals
     */
    public AbstractIndividual[] selectIndividuals(int tSize, int tWinners) {
        AbstractIndividual[] tournament = new AbstractIndividual[tSize];
        // fill tournament with random individuals
        for (int i = 0; i < tSize; ++i){
            tournament[i] = this.individuals[random.nextInt(this.individuals.length)];
        }
        // select and return the best few
        Arrays.sort(tournament, (a, b) -> Double.valueOf(a.getFitness()).compareTo(b.getFitness()));
        return Arrays.copyOfRange(tournament, tSize - tWinners, tSize);
    }
}
