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

vector<vector<int>> createInitialPopulation(int populationSize)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> solutionDistr(0, 1);

    int numberSolutionsPerPopulation = 16;
    vector<vector<int>> initialPopulation(populationSize, vector<int>(numberSolutionsPerPopulation));

    for (int i = 0; i < populationSize; ++i)
    {
        for (int j = 0; j < numberSolutionsPerPopulation; ++j)
        {
            initialPopulation[i][j] = solutionDistr(gen);
        }
    }

    return initialPopulation;
}


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