#include <iostream>
#include <random>
#include <vector>

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
    int numberOfItems = numberOfItemsDistr(gen);

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

int main()
{
    vector<Item> items = createRandomItems();

    for (Item& item : items)
    {
        cout << item.id << endl;
    }
    
    return 0;
}