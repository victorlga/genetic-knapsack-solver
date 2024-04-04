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
    std::vector<std::vector<int>> parents;
    std::vector<std::vector<int>> population;
    std::vector<std::vector<int>> offsprings;

    public:
    GeneticKnapsack(
        int capacity
    ) : capacity(capacity) {}

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

    void calculateFitness(int weight, int value)
    {
        for (int i = 0; i < populationSize; ++i)
        {
            int sumOfSelectedValues = 0;
            int sumOfSelectedWeights = 0;
            for (int j = 0; j < numberOfSolutionsPerPopulation; ++j)
            {
                int solutionItemMask = population[i][j];
                sumOfSelectedValues += solutionItemMask * value;
                sumOfSelectedWeights += solutionItemMask * weight;
            }
            fitnessMeasures[i] = (sumOfSelectedWeights <= capacity) ? sumOfSelectedValues : 0;
        }
    }

    void selectSolutionFromPopulation(int numberOfParents)
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
        int chanceOfCrossingOver;
        int crossingOverProbabilityThreshold = 0.8;

        int parentIndexReference = 0;
        int parent1Index;
        int parent2Index;

        int offspringsCount = 0;
        while (offspringsCount < numberOfOffsprings)
        {
            parent1Index = parentIndexReference % parents.size();
            parent2Index = (parentIndexReference + 1) % parents.size();

            chanceOfCrossingOver = crossOverDistr(gen);
            if (chanceOfCrossingOver > crossingOverProbabilityThreshold)
            {
                offsprings[offspringsCount] = createOffspring(parent1Index, parent2Index);
                offspringsCount++;
            }
            parentIndexReference++;
        }
    }
};


int main()
{
    std::vector<Item> items = createRandomItems();

    for (Item& item : items)
    {
        std::cout << "ID: " << item.id << " VALUE: " << item.value << " WEIGHT: " << item.weight << std::endl;
    }

    const int knapsackCapacity = 100;
    GeneticKnapsack geneticKnapsack(knapsackCapacity);

    const int populationSize = items.size();
    const int numberOfSolutionsPerPopulation = 16;
    geneticKnapsack.createInitialPopulation(populationSize, numberOfSolutionsPerPopulation);

    const int numberOfGenerations = 1000;

    

    
    return 0;
}