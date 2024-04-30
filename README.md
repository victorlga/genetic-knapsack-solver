## Genetic Algorithm for the Knapsack Problem

#### Description
This project implements a genetic algorithm to solve the knapsack problem, a combinatorial optimization problem. The program creates a set of items, each with a value and weight, and attempts to maximize the total value of items in a knapsack without exceeding its weight capacity.

#### Features
- Random item generation with configurable maximum values and weights.
- Genetic algorithm implementation including selection, crossover, mutation, and fitness evaluation.
- Simulation of multiple generations to evolve optimal or near-optimal solutions.
- Detailed output of the genetic algorithm's performance and results over generations.

#### How It Works
1. **Item Generation**: Items are randomly generated with specified limits for weight and value.
2. **Initial Population**: A population of potential solutions (individuals) is initialized, where each solution is a binary vector representing whether an item is included in the knapsack.
3. **Fitness Calculation**: The fitness of each individual is calculated based on the total value of items included, penalizing those that exceed the weight capacity.
4. **Parent Selection**: A set of the fittest individuals is selected to breed the next generation.
5. **Crossover**: Offsprings are produced by combining genetic material from two parents.
6. **Mutation**: Offsprings undergo random mutations to introduce genetic diversity.
7. **Population Update**: The new generation replaces the old one, and the process repeats.

#### Requirements
- C++ Compiler (e.g., GCC, Clang)
- Standard C++ Library

#### Compilation and Execution
To compile and run the project, use the following command:
```
g++ -o main main.cpp
./main
```

#### Configuration
Modify constants in the code to change the number of items, their maximum value and weight, and the knapsack capacity:
```cpp
const int MAX_NUMBER_OF_ITEMS = 50;
const int MAX_ITEM_VALUE = 100;
const int MAX_ITEM_WEIGHT = 30;
```

#### Output
The program outputs details of each item, the genetic algorithm's progress across generations, and the final solution including the total value and weight of the items in the knapsack.

#### License
This project is licensed under the MIT License - see the LICENSE.md file for details.
