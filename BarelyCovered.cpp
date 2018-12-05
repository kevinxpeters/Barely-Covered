//*****************************************************************************
// BarelyCovered.cpp
// Author: Kevin Peters
// Date: October 02, 2017
// Class: CIS 350, Bruce Elenbogen
// Description/Purpose: Considering 1 to many graphs ( V, E ). This program will find the ordering of a graph that minimizes the maximum cover.
// It will start by taking your input of vertices, edges and connected vertices, and creating an IDtable as a "primary key" of your adjacency matrix.
// It will then make a copy of the IDtable used to sort and run every permutation. During each permutation it will calculate the maximum cover for that order.
// The order with the smallest maximum cover is saved until the next smallest overwrites, until reaching the end of the permutations.
// The program will finally output the "best" order with the smallest maximum cover; along with the int minimum cover for that graph.
// The input uses a combination of integers and chars to read in your graph. It will take your edges and vertices and create an adjacency matrix
// To properly input into the program provide an int number of graphs, int number of vertices in graph(n).
// Then, you will input a char vertex, int number of edges connected to that vertex, then char connected vertexes respectively until you have input each vertex in the graph.
// Multiple graphs will be input after the first completes its calculation.
// The output will give you a vector of chars with a space between each relating to the smallest cover lexigraphically
// and then a space and then an int of the max cover of that order.
// Input: int Number of graphs, 
//		  int Number of vertices
//		  char vertex
//		  int how many edges the vertex connects to
//		  char associated vertices respective vertex is connected to
// Output: The ordering of the first lexigraphical order of the vertices with the smallest max cover of all permutations (all chars with spaces in between)
//		   int max cover of the the first lexigraphical order with the smallest max cover of all permutations

//*****************************************************************************

/********************INCLUDES**************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

/********************DATA TYPES************************************************/

// from Data Structures and Algorithm Analysis in C++ (Third Edition), by Mark Allen Weiss
template <class Object>
class Matrix
{
public:
	Matrix(int rows = 0, int cols = 0) : Array(rows)	//constructor with two parameters
	{
		for (int i = 0; i < rows; i++)
			Array[i].resize(cols);
	}
	void resize(int rows, int cols)
	{
		Array.resize(rows);
		for (int i = 0; i < rows; i++)
			Array[i].resize(cols);
	}
	const std::vector<Object> & operator[](int row) const
	{
		return Array[row];
	}
	std::vector<Object> & operator[](int row)
	{
		return Array[row];
	}
	//getter
	int numrows() const
	{
		return Array.size();
	}
	//getter
	int numcols() const
	{
		return numrows() ? Array[0].size() : 0;	//return the size, else return 0
	}
private:
	std::vector< std::vector<Object> > Array;	//two dimensional vector called Array
};
class Graph
{
public:
	Graph(int size);	
	void fillGraph(); 
	void printGraph();
	int maxCover(std::vector<char> & order);
	int cover(std::vector<char> order);
	int findLetter(char L);
	void printIDtable();
private:
	Matrix<int> adj;
	int vertices;
	std::vector<char> IDtable;
	std::vector<char> permID;
};

//******************************************************************
// Graph(int size)
// Purpose: Graph class contructor, 
// Output: None
// Pre: Size > 0
// Post: Graph object will be created to size
// and the adj 2D vector will be set to the size of the graph
//******************************************************************
Graph::Graph(int size) {
	adj.resize(size, size);
}

//******************************************************************
// findLetter(char L)
// Purpose: Finds the letter in the IDtable. If not found, inserts it
// Output: Outputs the position of the letter in the IDtable
// Pre: Must pass in a valid letter a-z or A-Z
// Post: The function will return the position of the letter L or else it will return -1 as a sentinel
//******************************************************************
int Graph::findLetter(char L) {
	//used for 1 vertex graph case
	if (adj.numrows() == 1) {
		IDtable[0] = L;
		return 0;
	}
	else {
		for (int i = 0; i < IDtable.size(); i++) {
			if (L == IDtable[i]) {
				return i;
			}
			else if (IDtable[i] == 0) {
				IDtable[i] = L;
				return i;
			}
		}
	}
	return -1;
}//findLetter()

 //******************************************************************
 // printIDtable()
 // Purpose: Prints the IDtable (debugging purposes only)
 // this is the ID key table that does not change
 // Output: None
 // Pre: None
 // Post: Loops through IDtable and prints out each element
 //******************************************************************
void Graph::printIDtable()
{
	for (int i = 0; i < IDtable.size(); i++) {
		std::cout << IDtable[i] << std::endl;
	}
}//printIDtable()

 //******************************************************************
 // fillGraph()
 // Purpose: Uses the 2D vector adj to fill the graph as an adjaceny matrix
 // Output: None
 // Pre: adj has rows > 0, thisVert and connectedVert = a-z || A-Z, numEdges >=0
 // Post: Finds the elements in the IDtable and marks a 1 
 // for each connected vertex in the adjacency matrix to fill the graph
 //******************************************************************
void Graph::fillGraph() {
	//set the IDtable to the number of vertices
	IDtable.resize(adj.numrows(), 0);
	for (int i = 0; i < adj.numrows(); i++) {
		char thisVert;
		int numEdges;
		std::cin >> thisVert;
		std::cin >> numEdges;
		//for 1 vertex graph
		if (adj.numrows() == 1 && numEdges == 0) {
			findLetter(thisVert);
		}
		else {
			for (int j = 0; j < numEdges; j++) {
				char connectedVert;
				std::cin >> connectedVert;
				adj[findLetter(thisVert)][findLetter(connectedVert)] = 1;
				adj[findLetter(connectedVert)][findLetter(thisVert)] = 1;
			}
		}
	}
	return;
}//fillGraph()

 //******************************************************************
 // printGraph()
 // Purpose: Loop through the 2D vector adj to cout the graph, prints the graph (for debugging only)
 // Output: None
 // Pre:  Adj is not null
 // Post: Prints out an adjacency matrix of 1's and 0's in relation to the IDtable generated
 //******************************************************************
void Graph::printGraph() {
	for (int i = 0; i < adj.numcols(); i++) {
		for (int j = 0; j < adj.numrows(); j++) {
			std::cout << adj[i][j] << " ";
		}
		std::cout << std::endl;
	}
}//printGraph()

 //******************************************************************
 // maxCover(std::vector<char> &minPerm)
 // Purpose: Sorts and then checks lexigraphically for the combination with the smallest cover
 // and returns the smallest cover
 // Output: the cover first lexigraphical smallest cover after checking all permutations
 // Pre: minPerm >= 1
 // Post: Function will loop through and check if the permutation is smaller than the last
 // and store it when it is smaller than the last looping through to check each permutation
 //******************************************************************
int Graph::maxCover(std::vector<char> &minPerm) {
	int minCover = minPerm.size(), temp;
	std::vector<char> permID = IDtable;
	std::vector<char> firstMinCover;
	std::sort(permID.begin(), permID.end());
	do {
		temp = cover(permID);
		if (permID.size() == 1) {
			minCover = 0;
			for (int j = 0; j < permID.size(); j++) {
				minPerm[j] = permID[j];
			}
		}else if (temp<minCover) {
			minCover = temp;
			for (int j = 0; j < permID.size(); j++) {
				minPerm[j] = permID[j];
			}
		}
	} while (std::next_permutation(permID.begin(), permID.end()));
	for (int i = 0; i < minPerm.size(); i++) {
		std::cout << minPerm[i] << " ";
	}
	return minCover;
}//maxCover()

//******************************************************************
// cover(std::vector<char> order)
// Purpose: Loops through a given set of vectices to find the max cover of that single combination
// Output: Int of the max cover of that combination of vertices related to the given adjacency matrix
// Pre: order >= 1
// Post: Loop through to find connected vertices and calculate cover at each
// storing the max to be output
//******************************************************************
int Graph::cover(std::vector<char> order) {
	int intCover = 0;
	int max = 0;
	for (int i = 0; i < order.size(); i++) {
		for (int j = 0; j < order.size(); j++) {
			if (adj[findLetter(order[i])][findLetter(order[j])] == 1) {
				intCover = abs( j - i);
				if (intCover > max)
				{
					max = intCover;
				} //if
			} //if
		} //for
	} //for
	return max;
}//cover()

int main() {
	int numGraphs;
	std::cin >> numGraphs;
	Graph * myGraph;
	for (int i = 0; i < numGraphs; i++) {
		int numVertices, minmax;
		std::cin >> numVertices;
		std::vector<char> answer(numVertices);
		myGraph = new Graph(numVertices);
		myGraph->fillGraph();
		minmax=myGraph->maxCover(answer);
		std::cout << minmax;
		delete myGraph;
		std::cout << std::endl;
	}//for
	return 0;
}// main()
