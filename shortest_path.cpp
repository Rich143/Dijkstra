#include <iostream>
#include <iomanip>
#include <climits>
#include <sstream>

const int WIDTH = 5;
const int HEIGHT = 5;

using namespace std;

// Element of linked list of nodes in pathList
struct Node {
	int nodeNumber;
	Node* next;
};

Node* newNode(int nodeNumber0) {
	Node* node = new Node;
	node->nodeNumber = nodeNumber0;
	node->next = NULL;

	return node;
}

void insertNode(Node* head, int nodeNumber0) {
	if (head == NULL) {
		head = newNode(nodeNumber0);
		return;
	} else {
		while (head->next != NULL) {
			head = head->next;
		}
		head->next = newNode(nodeNumber0);
	}
	return;
}

void deleteNodes(Node* node) {
	if (node == NULL) {
		cout << "Deleting Nodes\n";
		return;
	}
	deleteNodes(node->next);
	delete node;
	return;
}

// Element of array of paths in current graph
struct Path {
	int distance; // distance from start to node
	Node* pathNodeList; // linked list of nodes indicating path from start to this node
};



void print_matrix(unsigned int (&matrix)[WIDTH][HEIGHT], int width, int height);


void print_matrix(unsigned int (&matrix)[WIDTH][HEIGHT], int width, int height, string specifier);

void print_paths(Path* paths, int numPaths, string specifier);

void print_list(bool* list, int size, string specifier);

void print_linked_list(Node* node, string specifier, bool printSpecifier);

//void copy_to(int** in, int width, int height, int** out);


void dijkstra(unsigned int (&graph)[WIDTH][HEIGHT], int width, int height, int startNode, Path** paths);

int main() {
	unsigned int graph[WIDTH][HEIGHT] = { 
		{0, 2, INT_MAX, INT_MAX, 1},
		{2, 0, INT_MAX, 8, INT_MAX},
		{INT_MAX, INT_MAX, 0, INT_MAX, 5},
		{INT_MAX, 8, INT_MAX, 0, INT_MAX},
		{1, INT_MAX, 5, INT_MAX, INT_MAX}
	};

	print_matrix(graph, WIDTH, HEIGHT, "Graph: ");

	Path* paths; // array of paths

	dijkstra(graph, WIDTH, HEIGHT, 0, &paths);

	print_paths(paths, WIDTH, "Dijkstra results: ");

	delete[] paths;
	return 0;
}

void print_matrix(unsigned int (&matrix)[WIDTH][HEIGHT], int width, int height) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void print_matrix(unsigned int (&matrix)[WIDTH][HEIGHT], int width, int height, string specifier) {
	cout << specifier << endl;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void print_paths(Path* paths, int numPaths, string specifier) {
	cout << specifier << endl;
	for (int i=0; i<numPaths; i++) {
		cout << "Path to Node " << i << ": Distance = " << paths[i].distance
			 << " path = ";
		Node* path = paths[i].pathNodeList;
		while (path!= NULL) {
			cout << path->nodeNumber << "->";
			path = path->next;
		}
		cout << endl;
	}
	cout << endl;
}

void print_list(bool* list, int size, string specifier) {
	cout << setw(15) << specifier << "  ";

	for (int i=0; i<size; i++)	{
		cout << list[i] << " ";
	}
	cout << endl;
}

void print_linked_list(Node* node, string specifier, bool printSpecifier) {
	if (printSpecifier) {
		cout << specifier << "  ";
	}
	while (node != NULL) {
		cout << node->nodeNumber << "->";
		node = node->next;
	}
	cout << endl;
}

/*void copy_to(int** in, int width, int height, int** out) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			out[i][j] = in[i][j]; 
		}
	}
}*/

//@graph 2d matrix (array) with rows being origin nodes and columns weights to destination nodes
void dijkstra(unsigned int (&graph)[WIDTH][HEIGHT], int width, int height, int startNode, Path** paths0) {
	if (width != height)
		return;

	// number of nodes in graph
	int numNodes = width;

	// array of paths for all the nodes
	Path* paths = new Path[numNodes];

	// Array to store visited nodes, initialized to zero
	bool* visited = new bool[numNodes]();

	// Path to current node
	Node* pathToCurrent = newNode(startNode);

	// Node with closest distance
	int closestNode = startNode;

	// distance to closest node
	int shortestDistance = INT_MAX;

	// number of visited nodes
	int numVisited = 0; 

	// initialize the paths from the start node
	for (int i=0; i<numNodes; i++) {
		// if there is a connection
		if (graph[startNode][i] < INT_MAX) {
			// set the distance
			paths[i].distance = graph[startNode][i];
			// create a path
			paths[i].pathNodeList = newNode(startNode);
			paths[i].pathNodeList->next = newNode(i);

			// Keep updating which node is closest
			if (paths[i].distance < shortestDistance && i != startNode) {
				shortestDistance = paths[i].distance;
				closestNode = i;
			}
		}
		else {
			paths[i].distance = INT_MAX;
			paths[i].pathNodeList = NULL;
		}
	}

	print_paths(paths, numNodes, "After Start Node");

	numVisited++;
	visited[startNode] = true;

	// visit all the nodes
	while (numVisited < numNodes) {
		// node were going to visit now
		int currentNode = closestNode;

		cout << "Checking Node# " << currentNode << endl;

		// distance to current node
		int distanceToCurrent = paths[currentNode].distance;

		// The path to the current node
		Node* pathToCurrent = paths[currentNode].pathNodeList;

		// reset shortestDistance
		shortestDistance = INT_MAX;

		// add to paths the current nodes neighbour
		for (int i=0; i<numNodes; i++) {
			// set the nodes neighbours as new paths if they are shorter than current path
			if (graph[currentNode][i] < INT_MAX && graph[currentNode][i] + distanceToCurrent < paths[i].distance && i != currentNode) {
				// set the new shortest distance
				paths[i].distance = graph[currentNode][i] + distanceToCurrent;

				// add the route to the current node to the path
				Node* appendToPath = pathToCurrent;
				Node* path = paths[i].pathNodeList;
				Node* pathPrevious = NULL;

				cout << "Adding path to node " << i << endl;
				while (appendToPath != NULL) {
					
					// if a path already exists, overwrite it
					if (path != NULL) {
						cout << "Modifying current path element\n";
						path->nodeNumber = appendToPath->nodeNumber;
						cout << "added ->" << path->nodeNumber << endl;

						pathPrevious = path;
						path = path->next;
						appendToPath = appendToPath->next;
					}
					else {
						cout << "Adding new path element\n";
						path = newNode(appendToPath->nodeNumber);

						if (paths[i].pathNodeList == NULL) {
							paths[i].pathNodeList = path; // set pathNodeList to head node
							cout << "Set pathNodeList to " << path << endl;
						}
						if (pathPrevious != NULL) {
							pathPrevious->next = path;
						}

						cout << "added ->" << path->nodeNumber << endl;
						pathPrevious = path;
						path = path->next;
						appendToPath = appendToPath->next;
					}

				}

				// add the connection to node i to i's path
				if (path != NULL) {
					cout << "Modifying current path element\n";
					path->nodeNumber = i;
					cout << "added ->" << path->nodeNumber << endl;
					deleteNodes(path->next);
					path->next = NULL;
				}
				else {
					cout << "Adding new path element\n";
					path = newNode(i);
					pathPrevious->next = path;
					cout << "added ->" << path->nodeNumber << endl;
				}

				print_linked_list(paths[i].pathNodeList, "Resulting Path is: ", true);
			}


			// update closestNode and distance
			if (paths[i].distance < shortestDistance && !(visited[i]) && i != currentNode) {
				shortestDistance = paths[i].distance;
				closestNode = i;
			}
		}

		numVisited++;
		visited[currentNode] = true;

		stringstream ss;
		ss << "After Checking Node#" << currentNode;
		string iterID = ss.str();
		print_paths(paths, numNodes, iterID);
		cout << "Visited " << numVisited << " nodes out of " << numNodes << "nodes\n";
		print_list(visited, numNodes, "Visited for current node");
		cout << "\n\n";
	}

	delete[] visited;

	(*paths0) = paths;
}
