package bi.zum.lab3;

import cz.cvut.fit.zum.api.ga.AbstractEvolution;
import cz.cvut.fit.zum.api.ga.AbstractIndividual;
import cz.cvut.fit.zum.api.ga.AbstractPopulation;
import cz.cvut.fit.zum.data.StateSpace;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * @author Your name
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
     * @param count The number of individuals to be selected
     * @return List of selected individuals
     */
    public AbstractIndividual[] selectIndividuals(int tSize, int tWinners) {
        AbstractIndividual[] winners = new AbstractIndividual[tWinners];
        for(int i = 0; i < tSize; i++) {
            AbstractIndividual candidate = this.individuals[random.nextInt(this.individuals.length)];
            for (int j = 0; j < tWinners; j++){
                if (winners[j] == null || candidate.getFitness() > winners[j].getFitness()) {
                    for (int k = tWinners - 1; k > j; k--){
                        winners[k] = winners[k-1];
                    }
                    winners[j] = candidate;
                    break;
                }
            }
        }
        
        return winners;
    }
}
