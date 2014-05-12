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

vector<int> bestSolution;
int bestSolutionValue;

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

void iterateKnapsack(knapsack &k, int currentIndex)
{
	if(k.getCost() < k.getCostLimit())
	{
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

void exhaustiveKnapsack(knapsack &k, int t)
{
	iterateKnapsack(k, 0);
	for(int i = 0; i < bestSolution.size(); i++)
	{
		k.select(bestSolution[i]);
	}
	cout << "Cost limit: " << k.getCostLimit() << "\n";
	return;
}

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
}

