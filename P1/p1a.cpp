// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <cstdlib>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"

static time_t t;
static time_t start;
static int timeLimit;
vector<int> bestSolution;
int bestSolutionValue;

void exhaustiveKnapsack(knapsack &k, int t);
void interateKnapsack(knapsack &k, int currentIndex);
void saveSolution(knapsack &k);
void loadSolution(knapsack &k);
void checkTimeLimit(knapsack &k);
void outputSolution(knapsack &k);

int main(int argc, char* argv[])
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = argv[1];

	if(fileName.empty())
	{
		cout << "Enter filename" << endl;
		cin >> fileName;
	}

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		exhaustiveKnapsack(k, 600);

		cout << endl << "Best solution" << endl;
		k.printSolution();
	}

	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
}

/**
 * Recursively test all possible solutions for the knapsack
 * @param k the knapsack to use
 * @param currentIndex the current item
 */
void iterateKnapsack(knapsack &k, int currentIndex)
{
	if(k.getCost() < k.getCostLimit())
	{
		checkTimeLimit(k);
		if(currentIndex < k.getNumObjects())
		{
			k.select(currentIndex);
			iterateKnapsack(k, currentIndex + 1);
			k.unSelect(currentIndex);
			iterateKnapsack(k, currentIndex + 1);
		}
		else if(k.getValue() >= bestSolutionValue)
		{
			saveSolution(k);
		}
	}
}

/**
 * Uses brute force to find the best solution
 * @param k the knapsack to use
 * @param t the time limit
 */
void exhaustiveKnapsack(knapsack &k, int t)
{
	start = time(0);
	timeLimit = t;
	cout << "Cost limit: " << k.getCostLimit() << endl;
	iterateKnapsack(k, 0);
	loadSolution(k);
}

/**
 * Saves the current solution to a solution vector
 * @param k the knapsack to use
 */
void saveSolution(knapsack &k)
{
	bestSolution.clear();
	for(int i = 0; i < k.getNumObjects(); i++)
	{
		if(k.isSelected(i))
			bestSolution.push_back(i);
	}
	bestSolutionValue = k.getValue();
}

/**
 * Loads the solution from the solution vector
 * @param k the knapsack to use
 */
void loadSolution(knapsack &k)
{
	for(int i = 0; i < k.getNumObjects(); i++)
	{
		k.unSelect(i);
	}
	for(int i = 0; i < bestSolution.size(); i++)
	{
		k.select(bestSolution[i]);
	}
}

/**
 * Checks the time limit and ends the program if it is exceeded
 * @param k the knapsack to use
 */
void checkTimeLimit(knapsack &k)
{
	t = time(0);
	if(t - start > timeLimit)
	{
		loadSolution(k);
		k.printSolution();
		throw baseException("========== Time expired ==========");
	}
}

