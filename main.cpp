// C++ program to create target string, starting from
// random string using Genetic Algorithm
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;

// Target string to be generated
const int N = 100;
// Number of individuals in each generation
const int POPULATION_SIZE = min(3 * N, 100);
// Probability of mutation
const int MUTATION_PROB = 0.3;

// Function to generate random numbers in given range
int random_num(int start, int end)
{
	int range = (end - start) + 1;
	int random_int = start + (rand() % range);
	return random_int;
}

// create chromosome of genes
vector<int> create_gnome()
{
	vector<int> gnome(N, 0);
	for (int i = 0; i < N; i++)
		gnome[i] = random_num(1, N);
	return gnome;
}

// Class representing individual in population
class Individual
{
public:
	vector<int> chromosome;
	int fitness;
	Individual(vector<int> chromosome);
	Individual mate(Individual parent2);
	int cal_fitness();
};

Individual::Individual(vector<int> chromosome)
{
	this->chromosome = chromosome;
	fitness = cal_fitness();
};

// Perform mating and produce new offspring
Individual Individual::mate(Individual par2)
{
	// chromosome for offspring
	vector<int> child_chromosome(N, 0);

	for (int i = 0; i < N; i++)
	{
		// random probability
		float p = random_num(0, 100) / 100;

		// if prob is less than 0.35, insert gene
		// from parent 1
		if (p < (1 - MUTATION_PROB) / 2)
			child_chromosome[i] = chromosome[i];

		// if prob is between 0.35 and 0.7, insert
		// gene from parent 2
		else if (p < 1 - MUTATION_PROB)
			child_chromosome[i] = par2.chromosome[i];

		// otherwise insert random gene(mutate),
		// for maintaining diversity
		else
			child_chromosome[i] = random_num(1, N);
	}

	// create new Individual(offspring) using
	// generated chromosome for offspring
	return Individual(child_chromosome);
};


// Calculate fittness score, it is the number of
// characters in string which differ from target
// string.
int Individual::cal_fitness()
{
	int fitness = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < i; ++j) {
			if (chromosome[i] == chromosome[j] || abs(chromosome[i] - chromosome[j]) == abs(i - j)) {
				++fitness;
				break;
			}
		}
	}
	return fitness;
};

// Overloading < operator
bool operator<(const Individual &ind1, const Individual &ind2)
{
	return ind1.fitness < ind2.fitness;
}

// Overloading << operator
ostream& operator<< (ostream &out, const vector<int> &queens) {
	for (auto &queen : queens) {
		out << queen << ' ';
	}
	return out;
}

// Driver code
int main()
{
	srand((unsigned)(time(0)));

	// current generation
	int generation = 0;

	vector<Individual> population;
	bool found = false;

	// create initial population
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		vector<int> gnome = create_gnome();
		population.push_back(Individual(gnome));
	}

	while (!found)
	{
		// sort the population in increasing order of fitness score
		sort(population.begin(), population.end());

		// if the individual having lowest fitness score ie.
		// 0 then we know that we have reached to the target
		// and break the loop
		if (population[0].fitness <= 0)
		{
			found = true;
			break;
		}

		// Otherwise generate new offsprings for new generation
		vector<Individual> new_generation;
		// From 20% of fittest population, Individuals
		// will mate to produce offspring
		for (int i = 0; i < POPULATION_SIZE; i++)
		{
			int len = population.size();
			int r = random_num(0, (20 * POPULATION_SIZE) / 100);
			Individual parent1 = population[r];
			r = random_num(0, (20 * POPULATION_SIZE) / 100);
			Individual parent2 = population[r];
			Individual offspring = parent1.mate(parent2);
			new_generation.push_back(offspring);
		}
		population = new_generation;
		/*cout << "Generation: " << generation << "\t";
		cout << "Queens: " << population[0].chromosome << "\t";
		cout << "Fitness: " << population[0].fitness << "\n";*/

		generation++;
	}
	/*cout << "Generation: " << generation << "\t";
	cout << "Queens: " << population[0].chromosome << "\t";
	cout << "Fitness: " << population[0].fitness << "\n";*/

	ofstream outfile;
	outfile.open("output.txt", ios::out);
	outfile << "Number of Queens: " << N << "\n";
	outfile << "Generations: " << generation << "\n";
	outfile << "Queens result: " << population[0].chromosome << "\n";

	return 0;
}