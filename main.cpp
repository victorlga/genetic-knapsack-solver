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


vector<Item> createRandomItems()
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
    std::vector<int> fitness;
    std::vector<vector<int>> parents;
    std::vector<vector<int>> population;

    public:
    GeneticKnapsack(
        int populationSize,
        int numberOfSolutionsPerPopulation,
        int capacity
    ) : populationSize(populationSize), numberOfSolutionsPerPopulation(numberOfSolutionsPerPopulation), capacity(capacity) {}

    void createInitialPopulation()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> solutionDistr(0, 1);

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
            fitness[i] = (sumOfSelectedWeights <= capacity) ? sumOfSelectedValues : 0;
        }
    }

    void selectSolutionFromPopulation(int numberOfParents)
    {
        for (int i = 0; i < numberOfParents; ++i)
        {
            int indexOfMaxFitness = distance(fitness.begin(), max_element(fitness.begin(), fitness.end()));
            parents.push_back(population[indexOfMaxFitness]);
            fitness[indexOfMaxFitness] = std::numeric_limits<int>::min();
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

    int populationSize = items.size();

    auto initialPopulation = createInitialPopulation(populationSize);

    int numberOfGenerations = 1000;

    

    
    return 0;
}