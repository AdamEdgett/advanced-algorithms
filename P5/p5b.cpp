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
#include <stdlib.h>

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
bool recolorNode(graph* g, int node, int numColors);
bool recolorNodes(graph* g, int origin, int dest, int tri, int numColors);
void checkTimeLimit();
void printSolution();
bool localOptimum(graph* g, int numColors, int opt);
bool randomSolution(graph* g, int numColors);
bool checkColored(graph* g);
int findMostConflicts(graph* g, int* max);

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
		srand (time(NULL));
		
		cout << "Reading graph" << endl;
		graph g(fin);
		bestFound = g;
		graph original(g);
		minConflicts = g.numEdges();
		timeLimit = 600;
		start = time(0);
		cout << "Num colors: " << numColors << endl;
		
		try
		{
			//Find an initial greedy Solution
			cout << "Solving with Greedy"<< endl;
			greedyColoring(&g, numColors);
			//Next, find local optimums
			cout << "Finding local optimum (2 opt)" << endl;
			localOptimum(&g, numColors, 2);
			printSolution();
			cout << "Finding local optimum (3 opt)" << endl;
			localOptimum(&g, numColors, 3);
		}
		catch (baseException ex)
		{
			cout << ex.what() << endl;
			return 1;
		}
		printSolution();
		try
		{
			cout << "Finding random solution(s) with local optimum (2 opt)" << endl;
			//Reset graph
			g = graph(original);
			bestFound = g;
			minConflicts = g.numEdges();
			start = time(0);
			//Find initial random Solution
			//Next, find a local optimum
			while(1){
				if(randomSolution(&g, numColors)){
					break;
				}
				if(localOptimum(&g, numColors, 2)){
					break;
				}
				checkTimeLimit();
			}
		}
		catch (baseException ex)
		{
			cout << ex.what() << endl;
		}
		printSolution();
		try
		{		
			cout << "Finding random solution(s) with local optimum (3 opt)" << endl;
			//Reset graph
			g = graph(original);
			bestFound = g;
			minConflicts = g.numEdges();
			start = time(0);
			//Find initial random Solution
			//Next, find a local optimum
			while(1){
				if(randomSolution(&g, numColors)){
					break;
				}
				if(localOptimum(&g, numColors, 3)){
					break;
				}
				checkTimeLimit();
			}
		}
		catch (baseException &ex)
		{
			cout << ex.what() << endl;
		}
		printSolution();
	}
	catch (baseException &ex)
	{
		cout << ex.what() << endl;
	}
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
} 

/**
 * Color a node, picking the smallest color that is not present within node's neighbors
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
bool recolorNode(graph* g, int current, int numColors)
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
			return true;
		}
	}

	return false;
}

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
		recolorNode(g, c, numColors);
		checkTimeLimit();
	}
	checkColored(g);
}

/**
 * Check a static timeline metric and see if time expired. Throw exception if so
 *
 */
void checkTimeLimit()
{
	t = time(0);
	if (t - start > timeLimit)
	{
		throw baseException("========== Time expired ==========");
	}
}

void findNextEdge(graph* g, int* origin, int* dest)
{
	//Edges should really be in a hashmap to decrease O(V*V) to O(E)
	for (int i = *origin; i < g->numNodes(); i++)
	{
		for (int j = *dest; j < g->numNodes(); j++)
		{
			if (i == j)
			{
				continue;
			}
			
			if (g->isEdge(i, j))
			{
				*origin = i;
				*dest = j;
			}
		}
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
bool localOptimum(graph* g, int numColors, int opt)
{
	bool switched = false;

	int origin = 0;
	int dest = 1;
	int max = 9999;
	
	while(!switched){
		switched = false;
		origin = findMostConflicts(g, &max);
		
		if(origin == -1)
		{
			break;
		}

		findNextEdge(g, &origin, &dest);
		
		if (g->getNode(origin).getWeight() == g->getNode(dest).getWeight())
		{
			if (opt == 3)
			{
				for(int c = 0; c < g->numNodes(); c++)
				{
					if(	c != dest && c != origin && 
						((g->isEdge(origin, c) || g->isEdge(c, origin)) && 
						(g->getNode(origin).getWeight() == g->getNode(c).getWeight()) ||
						((g->isEdge(dest, c) || g->isEdge(c, dest)) &&
						(g->getNode(dest).getWeight() == g->getNode(c).getWeight()))))
					{
						if (recolorNodes(g, origin, dest, c, numColors))
						{
							switched = true;
						}
					}
				}
			}
			else
			{
				if(recolorNodes(g, origin, dest, -1, numColors)){
					switched = true;
				}
			}
		}
		checkTimeLimit();
	}
	
	return checkColored(g);
}

/**
 * Find the node with the most conflicts in the graph
 * @param graph g Graph to search
 * @return int Index of node with most conflicts
 */
int findMostConflicts(graph* g, int* max)
{
	int conflicts = 0;
	int maxConflicts = 0;
	int result = -1;
	for (int i = 0; i < g->numNodes(); i++)
	{
		conflicts = 0;
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

		//Set the new value to one with the most conflicts
		//  Which ALSO isn't the same as the previous value with the same # of conflicts
		//  Essentially cascade downwards from cycles
		if(conflicts > maxConflicts && (conflicts < *max))
		{
			result = i;
			maxConflicts = conflicts;
		}
	}

	*max = maxConflicts;
	return result;
}

/**
 * Choose a random solution
 * @param graph g Original graph to find a solution for
 * @param int numColors Max # colors
 * @return NEW graph with a random solution
 */
bool randomSolution(graph* g, int numColors)
{
	graph result(*g);
	for(int c = 0; c < g->numNodes(); c++)
	{
		result.getNode(c).setWeight(rand() % numColors);
	}
	
	return checkColored(&result); //When the Angels win the pennant...
}

bool recolorNodes(graph* g, int origin, int dest, int tri, int numColors)
{
	node* o = &g->getNode(origin);
	node* d = &g->getNode(dest);
	node* t = NULL;
	int neighborWeightOrigin = 0;
	int neighborWeightDest = 0;
	int neighborWeightTri = 0;
	
	bool is3opt = tri >= 0;
	if(!is3opt)
	{
		neighborWeightTri = neighborWeightOrigin;
	}
	else
	{
		t = &g->getNode(tri);
	}

	for (int i = 0; i < g->numNodes(); i++)
	{
		if (origin != i && dest != i && (!is3opt || tri != i))
		{
			if (g->isEdge(origin, i) || g->isEdge(i, origin))
			{
				neighborWeightOrigin |= (1 << g->getNode(i).getWeight());
			}
			
			if (g->isEdge(dest, i) || g->isEdge(i, dest))
			{
				neighborWeightDest |= (1 << g->getNode(i).getWeight());
			}

			if (is3opt && g->isEdge(tri, i) || g->isEdge(i, dest))
			{
				neighborWeightTri |= (1 << g->getNode(i).getWeight());
			}
		}	
	}
	
	//If there is a coloring which exists
	if (neighborWeightOrigin ^ neighborWeightDest ^ neighborWeightTri)
	{
		int combined = neighborWeightOrigin ^ neighborWeightDest ^ neighborWeightTri;

		//Pick lowest color not used in neighbors
		for (int c = 0; c < numColors; c++)
		{
			if ((combined & neighborWeightOrigin & (1 << c)) != 0)
			{
				o->setWeight(c);
			}
			if ((combined & neighborWeightDest & (1 << c) != 0))
			{
				d->setWeight(c);
			}
			if (is3opt && ((combined && neighborWeightTri & (1 << c) != 0)))
			{
				t->setWeight(c);
			}
		}
	
		return true;
	}

	return false;
}
