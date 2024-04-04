#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <utility>

const int MAX_NUMBER_OF_ITEMS = 50;
const int MAX_ITEM_VALUE = 100;
const int MAX_ITEM_WEIGHT = 15;

struct Item
{
    int id;
    int value;
    int weight;

    Item(int id, int value, int weight) : id(id), value(value), weight(weight) {}
};


std::vector<Item> createRandomItems()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> numberOfItemsDistr(1, MAX_NUMBER_OF_ITEMS);
    const int numberOfItems = numberOfItemsDistr(gen);

    int value;
    int weight;

    std::vector<Item> items = {};

    for (int id = 0; id < numberOfItems; ++id)
    {
        std::uniform_int_distribution<> valueDistr(1, MAX_ITEM_VALUE);
        std::uniform_int_distribution<> weightDistr(1, MAX_ITEM_WEIGHT);

        value = valueDistr(gen);
        weight = weightDistr(gen);

        items.push_back(Item(id, value, weight));
    }

    return items;
}

class GeneticKnapsack
{
    private:
    int capacity;
    int numberOfSolutionsPerPopulation;
    int populationSize;
    std::vector<int> fitnessMeasures;
    std::vector<Item> items;
    std::vector<std::vector<int>> parents,
                                  population,
                                  offsprings,
                                  mutants,
                                  fitnessMeasuresHistory;

    void calculateFitness()
    {
        fitnessMeasures.clear();
        for (int i = 0; i < populationSize; ++i)
        {
            int value = items[i].value;
            int weight = items[i].weight;
            int sumOfSelectedValues = 0;
            int sumOfSelectedWeights = 0;
            for (int j = 0; j < numberOfSolutionsPerPopulation; ++j)
            {
                int solutionItemMask = population[i][j];
                sumOfSelectedValues += solutionItemMask * value;
                sumOfSelectedWeights += solutionItemMask * weight;
            }
            fitnessMeasures.push_back((sumOfSelectedWeights <= capacity) ? sumOfSelectedValues : 0);
        }
    }

    void selectParentsFromPopulation(int numberOfParents)
    {
        parents.clear();
        for (int i = 0; i < numberOfParents; ++i)
        {
            int indexOfMaxFitness = distance(fitnessMeasures.begin(), max_element(fitnessMeasures.begin(), fitnessMeasures.end()));
            parents.push_back(population[indexOfMaxFitness]);
            fitnessMeasures[indexOfMaxFitness] = std::numeric_limits<int>::min();
        }
    }

    std::vector<int> createOffspring(int parent1Index, int parent2Index)
    {
        std::vector<int> offspring;
        for (int i = 0; i < numberOfSolutionsPerPopulation/2; ++i)
        {   
            int parentOffspringValue = (i < numberOfSolutionsPerPopulation/2) ? parents[parent1Index][i] : parents[parent2Index][i];
            offspring.push_back(parentOffspringValue);
        }

        return offspring;
    }

    void crossOverParents(int numberOfOffsprings)
    {
        offsprings.clear();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> crossOverDistr(0.0, 1.0);
        double crossingOverRate = 0.2;

        int parentIndexReference = 0;
        int parent1Index;
        int parent2Index;

        int offspringsCount = 0;
        while (offspringsCount < numberOfOffsprings)
        {
            parent1Index = parentIndexReference % parents.size();
            parent2Index = (parentIndexReference + 1) % parents.size();

            if (crossOverDistr(gen) > crossingOverRate)
            {
                offsprings.push_back(createOffspring(parent1Index, parent2Index));
                offspringsCount++;
            }
            parentIndexReference++;
        }
    }

    void mutateOffsprings()
    {
        double mutationRate = 0.15;
        mutants = offsprings;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> mutationDistr(0.0, 1.0);
        std::uniform_int_distribution<> mutantsDistr(0, numberOfSolutionsPerPopulation-1);
        int mutantSolutionIndex;
        for (int i = 0; i < offsprings.size(); ++i)
        {
            if (mutationDistr(gen) <= mutationRate) continue;
            mutantSolutionIndex = mutantsDistr(gen);
            mutants[i][mutantSolutionIndex] = (mutants[i][mutantSolutionIndex] == 0) ? 1 : 0;
        }
    }

    void updatePopulation()
    {
        merge(parents.begin(), parents.end(),
              mutants.begin(), mutants.end(),
              population.begin());
    }

    public:
    GeneticKnapsack(
        int capacity,
        std::vector<Item> items
    ) : capacity(capacity), items(items) {}

    void createInitialPopulation(int newPopulationSize, int newNumberOfSolutionsPerPopulation)
    {
        populationSize = newPopulationSize;
        numberOfSolutionsPerPopulation = newNumberOfSolutionsPerPopulation;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> solutionDistr(0, 1);

        population.clear();

        std::vector<int> individual;
        for (int i = 0; i < populationSize; ++i) {
            for (int j = 0; j < numberOfSolutionsPerPopulation; ++j) {
                individual.push_back(solutionDistr(gen));
            }
            population.push_back(individual);
            individual.clear();
        }
    }

    void optimizePopulation(int numberOfGenerations)
    {
        int halfPopulationSize = populationSize / 2;

        for (int i = 0; i<numberOfGenerations; ++i)
        {
            calculateFitness();
            fitnessMeasuresHistory.push_back(fitnessMeasures);

            selectParentsFromPopulation(halfPopulationSize);
            crossOverParents(halfPopulationSize);
            mutateOffsprings();

            updatePopulation();
        }
    }

    void showResults()
    {
        std::cout << "Last generation fitness measure: ";
        for (int& fitnessMeasure : fitnessMeasures) std::cout << fitnessMeasure << ", ";
        std::cout << std::endl;

        int indexOfMaxFitness = distance(fitnessMeasures.begin(), max_element(fitnessMeasures.begin(), fitnessMeasures.end()));
        std::cout << "Last generation parameters: ";
        for (int parameter : population[indexOfMaxFitness]) std::cout << parameter << ", ";
    }
};


int main()
{
    std::vector<Item> items = createRandomItems();

    for (Item& item : items)
    {
        std::cout << "ID: " << item.id << " VALUE: " << item.value << " WEIGHT: " << item.weight << std::endl;
    }
    std::cout << std::endl;

    const int knapsackCapacity = 100;
    GeneticKnapsack geneticKnapsack(knapsackCapacity, items);

    const int populationSize = items.size();
    const int numberOfSolutionsPerPopulation = 16;
    geneticKnapsack.createInitialPopulation(populationSize, numberOfSolutionsPerPopulation);

    const int numberOfGenerations = 1000;
    geneticKnapsack.optimizePopulation(numberOfGenerations);

    geneticKnapsack.showResults();

    return 0;
}