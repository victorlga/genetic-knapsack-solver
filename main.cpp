#include <algorithm>
#include <iostream>
#include <random>
#include <vector>


const int MAX_NUMBER_OF_ITEMS = 50;
const int MAX_ITEM_VALUE = 100;
const int MAX_ITEM_WEIGHT = 30;

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

    std::vector<Item> items;

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
    int numberOfItems;
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
            int sumOfSelectedValues = 0;
            int sumOfSelectedWeights = 0;
            for (int j = 0; j < numberOfItems; ++j)
            {
                sumOfSelectedValues += population[i][j] * items[j].value;
                sumOfSelectedWeights += population[i][j] * items[j].weight;
            }
            fitnessMeasures.push_back((sumOfSelectedWeights <= capacity) ? sumOfSelectedValues : 0);
        }
    }

    void selectParentsFromPopulation(int numberOfParents)
    {
        std::vector<std::vector<int>> newParents;
        for (int i = 0; i < numberOfParents; ++i)
        {
            int indexOfMaxFitness = distance(fitnessMeasures.begin(), max_element(fitnessMeasures.begin(), fitnessMeasures.end()));
            newParents.push_back(population[indexOfMaxFitness]);
            fitnessMeasures[indexOfMaxFitness] = -1;
        }
        parents = newParents;
    }

    std::vector<int> createOffspring(int parent1Index, int parent2Index)
    {
        int halfNumberOfItems = numberOfItems / 2;
        std::vector<int> offspring(numberOfItems);

        std::copy(parents[parent1Index].begin(), parents[parent1Index].begin() + halfNumberOfItems, offspring.begin());
        std::copy(parents[parent2Index].begin() + halfNumberOfItems, parents[parent2Index].end(), offspring.begin() + halfNumberOfItems);

        return offspring;
    }

    void crossOverParents(int numberOfOffsprings)
    {
        std::vector<std::vector<int>> newOffsprings;

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
            if (crossOverDistr(gen) > crossingOverRate)
            {
                parent1Index = parentIndexReference % parents.size();
                parent2Index = (parentIndexReference + 1) % parents.size();
                newOffsprings.push_back(createOffspring(parent1Index, parent2Index));
                offspringsCount++;
            }
            parentIndexReference++;
        }
        offsprings = newOffsprings;
    }

    void mutateOffsprings()
    {
        double mutationRate = 0.15;
        mutants.clear();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> mutationRateDistr(0.0, 1.0);
        std::uniform_int_distribution<> mutantsDistr(0, numberOfItems-1);

        int mutantIndex;
        for (int i = 0; i < static_cast<int>(offsprings.size()); ++i)
        {
            std::vector<int> mutant = offsprings[i];
            if (mutationRateDistr(gen) <= mutationRate) continue;
            mutantIndex = mutantsDistr(gen);
            mutant[mutantIndex] = (offsprings[i][mutantIndex] == 0) ? 1 : 0;
            mutants.push_back(mutant);
        }
    }

    void updatePopulation()
    {
        // Not proper solution, but it works.
        // Merge will just append vectors, alternating between parents and mutants.
        merge(
            parents.begin(), parents.end(),
            mutants.begin(), mutants.end(),
            population.begin()
        );
    }

    public:
    GeneticKnapsack(
        int capacity,
        std::vector<Item> items
    ) : capacity(capacity), items(items) {}

    void createInitialPopulation(int newPopulationSize, int newNumberOfItems)
    {
        populationSize = newPopulationSize;
        numberOfItems = newNumberOfItems;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> solutionDistr(0, 1);

        for (int i = 0; i < populationSize; ++i)
        {
            std::vector<int> individual;
            for (int j = 0; j < numberOfItems; ++j)
            {
                individual.push_back(solutionDistr(gen));
            }
            population.push_back(individual);
        }
    }

    void optimizePopulation(int numberOfGenerations)
    {
        int halfNumberOfItems = numberOfItems / 2;
        int numberOfParents = populationSize / 2;

        for (int i = 0; i<numberOfGenerations; ++i)
        {
            calculateFitness();
            fitnessMeasuresHistory.push_back(fitnessMeasures);

            selectParentsFromPopulation(numberOfParents);
            crossOverParents(halfNumberOfItems);
            mutateOffsprings();

            updatePopulation();
        }
    }

    void showResults()
    {
        calculateFitness();

        std::cout << "Last generation fitness: ";
        for (int& fitnessMeasure : fitnessMeasures) std::cout << fitnessMeasure << ", ";
        std::cout << std::endl;

        int indexOfMaxFitness = distance(fitnessMeasures.begin(), max_element(fitnessMeasures.begin(), fitnessMeasures.end()));
        std::cout << "Index of max fitness: " << indexOfMaxFitness << std::endl << std::endl;
        std::vector<int> solution = population[indexOfMaxFitness];

        std::cout << "Last generation parameters: ";
        for (int& parameter : solution) std::cout << parameter << ", ";
        std::cout << std::endl;

        int value = 0;
        std::cout << "Selected item values: ";
        for (int i = 0; i < numberOfItems; ++i)
        {
            if (population[indexOfMaxFitness][i] == 1) 
            {
                std::cout << items[i].value << ", ";
                value += items[i].value;
            }
        }
        std::cout << std::endl;

        int weight = 0;
        std::cout << "Selected item weights: ";
        for (int i = 0; i < numberOfItems; ++i)
        {
            if (population[indexOfMaxFitness][i] == 1) 
            {
                std::cout << items[i].weight << ", ";
                weight += items[i].weight;
            }
        }
        std::cout << std::endl << std::endl;

        std::cout << "Final value: " << value << std::endl;
        std::cout << "Final weight: " << weight << std::endl;
        std::cout << "Capacity: " << capacity << std::endl;
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

    const int knapsackCapacity = MAX_ITEM_WEIGHT * 2;
    GeneticKnapsack geneticKnapsack(knapsackCapacity, items);

    const int populationSize = MAX_NUMBER_OF_ITEMS;
    const int numberOfItems = items.size();
    geneticKnapsack.createInitialPopulation(populationSize, numberOfItems);

    const int numberOfGenerations = 1000;
    geneticKnapsack.optimizePopulation(numberOfGenerations);
        
    geneticKnapsack.showResults();

    return 0;
}