// Project 2b: Solving graph coloring using greedy search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"

using namespace std;

static time_t t;
static time_t start;
static int timeLimit;

static graph bestFound;
static int minConflicts;

void greedyColoring(graph* g, int numColors, int limit);
void greedyColorNode(graph* g, int node, int numColors);
void checkTimeLimit(graph* g);
void printSolution();

int main(int argc, char** argv)
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;
	int numColors;
	vector<int> solution;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	//fileName = "../instances/color/color12-4.input";

	if(argc == 2)
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
		cerr << "Cannot open " << fileName << endl;
		return 1;
	}

	try
	{
		cout << "Reading number of colors" << endl;
		fin >> numColors;

		cout << "Reading graph" << endl;
		graph g(fin);
		bestFound = g;
		minConflicts = g.numEdges();

		cout << "Num colors: " << numColors << endl;

		greedyColoring(&g, numColors, 60 * 10);
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl;
		return 1;
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl;
		return 1;
	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl;
	}

	printSolution();
}

void printSolution()
{
	cout << "# Conflicts: " << minConflicts << endl;
	cout << "Best found solution: " << endl;
	bestFound.printNodes();
}

/**
 * Check if a graph contains a valid coloring
 * @param g Graph instance to check
 */
bool checkColored(graph* g)
{
	int conflicts = 0;
	for(int i = 0; i < g->numNodes(); i++)
	{
		for(int j = 0; j < g->numNodes(); j++)
		{
			if(i == j)
			{
				continue;
			}
			if(g->isEdge(i, j))
			{
				if(g->getNode(i).getWeight() == g->getNode(j).getWeight())
				{
					conflicts++;
				}
			}
		}
	}

	if(conflicts <= minConflicts)
	{
		 bestFound = graph(*g);
		 minConflicts = conflicts;
	}

	return minConflicts == 0;
}

/**
 * Recursively color the graph, brute force approach
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
void greedyColorNode(graph* g, int current, int numColors)
{
	node* n = &g->getNode(current);
	int neighborWeight = 0;

	for(int i = 0; i < g->numNodes(); i++)
	{
		if(current == i)
		{
			continue;
		}

		if(g->isEdge(current, i) || g->isEdge(i, current))
		{
			neighborWeight |= (1 << g->getNode(i).getWeight());
		}
	}

	//Pick lowest color not used in neighbors
	for(int c = 0; c < numColors; c++)
	{
		if((neighborWeight & (1 << c)) == 0)
		{
			n->setWeight(c);
			return;
		}
	}

	//99 Otherwise
	n->setWeight(99);
}

/**
 * Greedy coloring algorithm where iterate 0->n, for x picking the smallest color that is not present within x's neighbors
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
void greedyColoring(graph* g, int numColors, int limit)
{
	start = time(0);
	timeLimit = limit;
	
	for(int c = 0; c < g->numNodes(); c++)
	{
		g->getNode(c).setWeight(numColors + 1);
	}

	for(int c = 0; c < g->numNodes(); c++)
	{
		greedyColorNode(g, c, numColors);
		checkTimeLimit(g);
	}
	checkColored(g);
}

void checkTimeLimit(graph* g)
{
	t = time(0);
	if(t - start > timeLimit)
	{
		throw baseException("========== Time expired ==========");
	}
}

