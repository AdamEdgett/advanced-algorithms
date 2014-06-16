/**
 * Adam Edgett, Matt McDonald
 * Project 5b: Solving knapsack using
		1) Greedy search paired with Local search
		2) Random solution paired with Local search (repetative)
 */

#include <iostream>
#include <cstdlib>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

using namespace std;

static time_t t;
static time_t start;
static int timeLimit;

vector<double> valueCostRatio;

void greedySolve(knapsack &k);
void calculateValueCostRatios(knapsack &k);
int getBestObject(knapsack &k);
void calculateValueCostRatios(knapsack &k);
bool checkValid(knapsack &k);
void checkTimeLimit();
knapsack randomSolution(knapsack &k);
knapsack localOptimum(knapsack &k, int opt);

int main(int argc, char** argv)
{
	char x;
	ifstream fin;
	string fileName;

	if (argc == 2)
	{
		fileName = argv[1];
	}
	else
	{
		cout << "Enter filename" << endl;
		cin >> fileName;
	}
	fin.open(fileName.c_str());
	if (!fin)
	{
		cout << "Cannot open " << fileName << endl;
		return 1;
	}

	cout << "Reading knapsack instance" << endl;
	knapsack k(fin);
	knapsack original(k);
	knapsack bestFound(k);
	timeLimit = 600;
	start = time(0);
	cout << "Cost limit: " << k.getCostLimit() << endl;

	//Find an initial greedy Solution
	cout << "Solving with Greedy" << endl;
	greedySolve(k);
	//Next, find local optimums
	cout << "Finding local optimium (2 opt)" << endl;
	k = localOptimum(k, 2);
	k.printSolution();
	cout << "Finding local optimium (3 opt)" << endl;
	k = localOptimum(k, 3);
	k.printSolution();

	cout << "Finding random solution(s) with local optimum (2 opt)" << endl;
	//Reset knapsack
	k = knapsack(original);
	start = time(0);
	try {
		//Find initial random solution
		//Next, find a local optimum
		while(1) {
			k = randomSolution(k);
			k = localOptimum(k, 2);
			if (k.getValue() > bestFound.getValue())
			{
				bestFound = k;
			}
			checkTimeLimit();
		}
	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl;
		bestFound.printSolution();
	}

	cout << "Finding random solution(s) with local optimum (3 opt)" << endl;
	//Reset knapsack
	k = knapsack(original);
	start = time(0);
	//Find initial random solution
	//Next, find a local optimum
	try {
		while(1) {
			k = randomSolution(k);
			k = localOptimum(k, 3);
			if (k.getValue() > bestFound.getValue())
			{
				bestFound = k;
			}
			checkTimeLimit();
		}
	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl;
		bestFound.printSolution();
	}
} // end main

/**
 * Check if a knapsack contains a valid selection
 * @param k the knapsack instance to check
 */
bool checkValid(knapsack &k)
{
	return (k.getCost() < k.getCostLimit());
}

/**
 * Uses a greedy algorithm to find the best solution
 * @param k the knapsack to use
 * @param t the time limit
 */
void greedySolve(knapsack &k)
{
	calculateValueCostRatios(k);
	while (k.getCost() < k.getCostLimit()) {
		checkTimeLimit();
		int best = getBestObject(k);
		if (best == -1)
			break;
		k.select(best);
		valueCostRatio[best] = 0;
	}
}

/**
 * Calculates the value/cost ratio for each object
 * And stores the ratios in a vector
 * TODO sort values to improve runtime
 * @param k the knapsack to use
 */
void calculateValueCostRatios(knapsack &k) {
	for (int i = 0; i < k.getNumObjects(); i++)
	{
		valueCostRatio.insert(valueCostRatio.begin() + i,
				(k.getValue(i) / (double)k.getCost(i)));
	}
}

/**
 * Selects the current best object to select
 * @param k the knapsack to use
 */
int getBestObject(knapsack &k) {
	int best = -1;
	for (int i = 0; i < k.getNumObjects(); i++)
	{
		if (best == -1 || valueCostRatio[i] > valueCostRatio[best])
		{
			if (valueCostRatio[i] > 0 && k.getCost(i) <= (k.getCostLimit() - k.getCost()))
			{
				best = i;
			}
		}
	}
	return best;
}

void checkTimeLimit()
{
	t = time(0);
	if (t - start > timeLimit)
	{
		throw baseException("========== Time expired ==========");
	}
}

/**
 * Finds a local optimum and returns the result
 *
 * @param k knapsack to find local optimum for
 * @param int 2-opt or 3-opt
 * @return new knapsack  with a local opt solution
 */
knapsack localOptimum(knapsack &k, int opt)
{
	knapsack original(k);
	deque<knapsack> problem;
	knapsack bestFound = knapsack(k);
	try
	{
		for (int l = 0; l < k.getNumObjects() - 1; l++)
		{
			k = knapsack(original);
			if (!k.isSelected(l))
				continue;
			k.unSelect(l);
			for (int m = l; m < k.getNumObjects() - 1; m++)
			{
				if (!k.isSelected(m))
					continue;
				k.unSelect(m);
				if (opt == 3)
				{
					for (int n = m; n < k.getNumObjects() - 1; n++)
					{
						if (!k.isSelected(n))
							continue;
						k.unSelect(n);
						problem.push_front(knapsack(k));
					}
				}
				else
					problem.push_front(knapsack(k));
			}
		}
		while(problem.size() > 0)
		{
			checkTimeLimit();
			knapsack current = problem.front();
			problem.pop_front();
			deque<knapsack> solution;
			solution.push_front(knapsack(current));
			int currentIndex = 0;
			while(solution.size() > 0)
			{
				knapsack currentSolution = solution.front();
				solution.pop_front();
				if (currentSolution.getValue() > bestFound.getValue())
				{
					bestFound = knapsack(currentSolution);
				}
				if (currentIndex < currentSolution.getNumObjects() - 1)
				{
					if (!currentSolution.isSelected(currentIndex) &&
							(currentSolution.getCost() + currentSolution.getCost(currentIndex)) < currentSolution.getCostLimit())
					{
						currentSolution.select(currentIndex);
						solution.push_front(knapsack(currentSolution));
					}
					currentIndex++;
				}
			}
		}
		return bestFound;

	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl;
		return bestFound;
	}
} //end localOptimum

/**
 * @param k knapsack to find a solution for
 * @return new knapsack with a random solution
 */
knapsack randomSolution(knapsack &k)
{
	int i = 0;
	while (checkValid(k) && i < k.getNumObjects())
	{
		int random = rand() % 2;
		if (random == 1 && (k.getCost() + k.getCost(i)) < k.getCostLimit())
			k.select(i);
		else
			k.unSelect(i);
		i++;
	}
	return k;
}
