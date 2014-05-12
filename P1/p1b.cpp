// Project 1b: Solving graph coloring using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "../common/d_except.h"
#include "../common/d_matrix.h"
#include "../common/graph.h"

using namespace std;

void exhaustiveColoring(graph* g, int numColors, int limit);
bool recursiveColor(graph* g, int node, int numColors);
void exhaustiveColoring(graph* g, int numColors, int limit);

int main()
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;
	int numColors;
	vector<int> solution;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "../instances/color/color12-4.input";

	//cout << "Enter filename" << endl;
	//cin >> fileName;

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

		cout << "Num colors: " << numColors << endl;

		exhaustiveColoring(&g, numColors, 60);
		cout << g;
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
}

/**
 * Check if a graph contains a valid coloring
 * @param g Graph instance to check
 */
bool isColored(graph* g)
{
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
					return false;
				}
			}
		}
	}

	return true;
}

/**
 * Recursively color the graph, brute force approach
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
bool recursiveColor(graph* g, int node, int numColors)
{
	for(int c = 0; c < numColors; c++)
	{
		g->getNode(node).setWeight(c);
		if(isColored(g)){
			return true;
		}
		else if((node + 1) < g->numNodes())
		{
			if(recursiveColor(g, node + 1, numColors))
			{
				return true;
			}
		}
	}

	return false;
}

/**
 * Brute force color the graph
 * @param g Graph instance to color
 * @param node 0 based index of the node to color
 * @param numColors Total number of colors to be used in the coloring scheme
 */
void exhaustiveColoring(graph* g, int numColors, int limit)
{
	recursiveColor(g, 0, numColors);
}

