#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <utility>

using namespace std;

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
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> numberOfItemsDistr(1, MAX_NUMBER_OF_ITEMS);
    const int numberOfItems = numberOfItemsDistr(gen);

    int value;
    int weight;

    vector<Item> items = {};

    for (int id = 0; id < numberOfItems; ++id)
    {
        uniform_int_distribution<> valueDistr(1, MAX_ITEM_VALUE);
        uniform_int_distribution<> weightDistr(1, MAX_ITEM_WEIGHT);

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
    int fitnessMaxValueIndex;
    int numberOfSolutionsPerPopulation;
    int populationSize;
    vector<int> fitness;
    vector<vector<int>> parents;
    vector<vector<int>> population;

    void findFitnessMaxValueIndex()
    {
        int maxValueFitness = *max_element(fitness.begin(), fitness.end());
        int i = 0;
        while (fitness[i] != maxValueFitness) i++;
        fitnessMaxValueIndex = i;
    }

    public:
    GeneticKnapsack(
        int populationSize,
        int numberOfSolutionsPerPopulation,
        int capacity
    ) : populationSize(populationSize), numberOfSolutionsPerPopulation(numberOfSolutionsPerPopulation), capacity(capacity) {}

    void createInitialPopulation()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> solutionDistr(0, 1);

        for (int i = 0; i < populationSize; ++i) {
            vector<int> individual;
            for (int j = 0; j < numberOfSolutionsPerPopulation; ++j) {
                individual.push_back(solutionDistr(gen));
            }
            population.push_back(individual);
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
        vector<int> parent;
        for (int i = 0; i < numberOfParents; ++i)
        {
            findFitnessMaxValueIndices();
            parent = population[];
            parents.push_back(parent);
        }
    }
};


int main()
{
    vector<Item> items = createRandomItems();

    for (Item& item : items)
    {
        cout << "ID: " << item.id << " VALUE: " << item.value << " WEIGHT: " << item.weight << endl;
    }

    const int knapsackCapacity = 100;

    int populationSize = items.size();

    auto initialPopulation = createInitialPopulation(populationSize);

    int numberOfGenerations = 1000;

    

    
    return 0;
}