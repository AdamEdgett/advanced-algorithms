// Project 2a: Solving knapsack using a greedy algorithm
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
vector<double> valueCostRatio;

void exhaustiveKnapsack(knapsack &k, int t);
void calculateValueCostRatios(knapsack &k);
int getBestObject(knapsack &k);
void checkTimeLimit(knapsack &k);

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
 * Uses a greedy algorithm to find the best solution
 * @param k the knapsack to use
 * @param t the time limit
 */
void exhaustiveKnapsack(knapsack &k, int t)
{
	start = time(0);
	timeLimit = t;
	cout << "Cost limit: " << k.getCostLimit() << endl;
	calculateValueCostRatios(k);
	while(k.getCost() < k.getCostLimit()) {
		checkTimeLimit(k);
		int best = getBestObject(k);
		if(best == -1)
			break;
		k.select(best);
		valueCostRatio[best] = 0;
	}
}

/**
 * Calculates the value/cost ratio for each object
 * And stores the ratios in a vector
 * @param k the knapsack to use
 */
void calculateValueCostRatios(knapsack &k) {
	for(int i = 0; i < k.getNumObjects(); i++)
	{
		valueCostRatio[i] = (k.getValue(i) / (double)k.getCost(i));
	}
}

/**
 * Selects the current best object to select
 * @param k the knapsack to use
 */
int getBestObject(knapsack &k) {
	int best = -1;
	for(int i = 0; i < k.getNumObjects(); i++)
	{
		if(best == -1 || valueCostRatio[i] > valueCostRatio[best])
		{
			if(valueCostRatio[i] > 0 && k.getCost(i) <= (k.getCostLimit() - k.getCost()))
			{
				best = i;
			}
		}
	}
	return best;
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
		k.printSolution();
		throw baseException("========== Time expired ==========");
	}
}

