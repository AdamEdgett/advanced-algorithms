/**
 * Adam Edgett, Matt McDonald
 * Project 5b: 	Solving graph coloring using 
 		1)	Greedy search paired with Local search
 		2) 	Random solution paired with Local search (repetative)
 */

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

void greedyColoring(graph* g, int numColors);
void greedyColorNode(graph* g, int node, int numColors);
void checkTimeLimit();
void printSolution();
graph localOptimum(graph* g, int numColors, int opt);
graph randomSolution(graph* g, int numColors);

int main(int argc, char** argv)
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;
	int numColors;
	
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
		graph original(g);
		minConflicts = g.numEdges();
		timeLimit = 600;
		start = time(0);
		cout << "Num colors: " << numColors << endl;

		//Find an initial greedy Solution
		cout << "Solving with Greedy"<< endl;
		greedyColoring(&g, numColors);
		//Next, find local optimums
		cout << "Finding local optimium (2 opt)" << endl;
		g = localOptimum(&g, numColors, 2);
		printSolution();
		cout << "Finding local optimium (3 opt)" << endl;
		g = localOptimum(&g, numColors, 3);
		printSolution();

		cout << "Finding random solution(s) with local optimum (2 opt)" << endl;
		//Reset graph
		g = graph(original);
		bestFound = g;
		minConflicts = g.numEdges();
		start = time(0);
		//Find initial random Solution
		//Next, find a local optimum
		while(1){
			randomSolution(&g, numColors);
			localOptimum(&g, numColors, 2);
			checkTimeLimit();
		}
		
		cout << "Finding random solution(s) with local optimum (3 opt)" << endl;
		//Reset graph
		g = graph(original);
		bestFound = g;
		minConflicts = g.numEdges();
		start = time(0);
		//Find initial random Solution
		//Next, find a local optimum
		while(1){
			randomSolution(&g, numColors);
			localOptimum(&g, numColors, 3);
			checkTimeLimit();
		}	
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
} // end main

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
	for (int i = 0; i < g->numNodes(); i++)
	{
		for (int j = 0; j < g->numNodes(); j++)
		{
			if (i == j)
			{
				continue;
			}
			if (g->isEdge(i, j))
			{
				if (g->getNode(i).getWeight() == g->getNode(j).getWeight())
				{
					conflicts++;
				}
			}
		}
	}

	if (conflicts <= minConflicts)
	{
		bestFound = graph(*g);
		minConflicts = conflicts;
	}

	return minConflicts == 0;
} // end checkColored

/**
 * Color a node, picking the smallest color that is not present within node's neighbors
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
void greedyColorNode(graph* g, int current, int numColors)
{
	node* n = &g->getNode(current);
	int neighborWeight = 0;

	for (int i = 0; i < g->numNodes(); i++)
	{
		if (current == i)
		{
			continue;
		}

		if (g->isEdge(current, i) || g->isEdge(i, current))
		{
			neighborWeight |= (1 << g->getNode(i).getWeight());
		}
	}

	//Pick lowest color not used in neighbors
	for (int c = 0; c < numColors; c++)
	{
		if ((neighborWeight & (1 << c)) == 0)
		{
			n->setWeight(c);
			return;
		}
	}

	//0 Otherwise
	n->setWeight(0);
} // end greedyColorNode

/**
 * Greedy coloring algorithm where iterate 0->n, for x picking the smallest color that is not present within x's neighbors
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
void greedyColoring(graph* g, int numColors)
{
	for (int c = 0; c < g->numNodes(); c++)
	{
		g->getNode(c).setWeight(numColors + 1);
	}

	for (int c = 0; c < g->numNodes(); c++)
	{
		greedyColorNode(g, c, numColors);
		checkTimeLimit();
	}
	checkColored(g);
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
 * @param graph g Original graph to find local optimum for
 * @param int numColors Max # colors
 * @param int 2-opt or 3-opt 
 * @param NEW graph with a local opt solution
 */
graph localOptimum(graph* g, int numColors, int opt)
{
	//TODO
	return graph(*g);
}

/**
 *
 * @param graph g Original graph to find a solution for
 * @param int numColors Max # colors
 * @return NEW graph with a random solution
 */
graph randomSolution(graph* g, int numColors)
{
	//TODO
	return graph(*g);
}
