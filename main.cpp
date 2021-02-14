#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
using namespace std;

vector<int> findInd(vector<vector<int>> state, int num);

class Node {
public: 
	Node(vector<vector<int>> state, int size);
	//Node(Node* prev, Node* ex1, Node* ex2, int state[][], int size);
	//Node(Node* prev, Node* ex1, Node* ex2, Node* ex3, int  state[][], int size);
	//Node(Node* prev, Node* ex1, Node* ex2, Node* ex3, Node* ex4, int state[][], int size);
	Node* prev;
	Node* ex1;
	Node* ex2;
	Node* ex3;
	Node* ex4;
	vector<vector<int>> state;
	int sz;
	int depth;
	

	void print();
	void expand();
	
};

vector<Node> queuefxn(Node n, deque<Node> queue);
Node generalSearch(Node n, int choice);

int main() {
	
	int inp = 0;

	cout << "Welcome to Matthew Lee\'s 8-puzzle solver. \nType \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
	cin >> inp;
	
	int choice = 0;
	cout << "Enter your choice of algorithm\n\t1. Uniform Cost Search\n\t2. A* with the Misplaced Tile heuristic.\n\t3.  A* with the Manhattan distance heuristic." << endl;
	cin >> choice;

	vector<vector<int>> deflt {	{1,2,3},
								{4,0,8},
								{7,6,5} };

	Node test(deflt, 3);


	cout << "-------------before node print------------------" << endl;

	test.print();

	cout << "-------------after node print------------------" << endl;
	
	cout << "-------------before index print------------------" << endl;

	vector<int> newtest = findInd(test.state, 2);
	cout << "(" << newtest.at(0) << ", " << newtest.at(1) << ")" << endl;

	cout << "-------------after index print------------------" << endl;

	cout << "-------------before test expand-----------------" << endl;
	test.expand();
	cout << "-------------after test expand------------------" << endl;
	cout << endl << "State move up" << endl;
	test.ex1->print();
	cout << endl << "State move down" << endl;
	test.ex2->print();
	cout << endl << "State move left" << endl;
	test.ex3->print();
	cout << endl << "State move right" << endl;
	test.ex4->print();


	cout << "-------------testing dynamic goal-----------------" << endl;

	int goalNSize = 0;
	
	cout << "Please enter in the size of puzzle" << endl;
	cin >> goalNSize;

	vector<vector<int>> goalState;							// Creating a dynamic goal state based on problem state size
	vector<int> blank;
	for (int i = 0; i < goalNSize; i++) {
		goalState.push_back(blank);
		for (int j = 0; j < goalNSize; j++) {
			goalState.at(i).push_back(j + i * goalNSize + 1);
		}
	}
	goalState[goalNSize - 1][goalNSize - 1] = 0;

	Node goalNode(goalState, goalNSize);
	goalNode.print();
	cout << "-------------dynamic goal test finished------------------" << endl;
}

Node::Node(vector<vector<int>> state, int size) {
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
	prev = NULL;
	ex1 = NULL;
	ex2 = NULL;
	ex3 = NULL;
	ex4 = NULL;
	this->state = state;
	sz = size;
};
/*
Node::Node(Node* prev, Node* ex1, Node* ex2, int state[][], int size) {
	this->prev = prev;
	this->ex1 = ex1;
	this->ex2 = ex2;
	this->ex3 = NULL;
	this->ex4 = NULL;
	this->state = new int[size][size];
	this->state = state;
	sz = size;
}

Node::Node(Node* prev, Node* ex1, Node* ex2, Node* ex3, int state[][], int size) {
	this->prev = prev;
	this->ex1 = ex1;
	this->ex2 = ex2;
	this->ex3 = ex3;
	this->ex4 = NULL;
	this->state = new int[size][size];
	this->state = state;
	sz = size;
}

Node::Node(Node* prev, Node* ex1, Node* ex2, Node* ex3, Node* ex4, int state[][], int size) {
	this->prev = prev;
	this->ex1 = ex1;
	this->ex2 = ex2;
	this->ex3 = ex3;
	this->ex4 = ex4;
	this->state = new int[size][size];
	this->state = state;
	sz = size;
}
*/

void Node::expand() {
	vector<int> indmove = findInd(this->state, 0);				// index of "0"

	int posmove = 4;										// possible moves
	int sz = this->state.size();								// length/width of state. The N if state is N x N size.
	int x = indmove.at(0);
	int y = indmove.at(1);
	vector<vector<int>> tempState;


	if (x == 0 || x == sz - 1) {
		posmove--;
	}
	if (y == 0 || y == sz - 1) {
		posmove--;
	}

	if (x - 1 >= 0) {										// check if up is valid
		tempState = this->state;
		tempState[x][y] = tempState[x - 1][y];				// up
		tempState[x - 1][y] = 0;
		this->ex1 = new Node(tempState, tempState.size());
		this->ex1->prev = this;
	}
	if (x + 1 < sz) {										// check if down is valid
		tempState = this->state;
		tempState[x][y] = tempState[x + 1][y];				// down
		tempState[x + 1][y] = 0;
		this->ex2 = new Node(tempState, tempState.size());
		this->ex2->prev = this;
	}
	if (y - 1 >= 0) {										// check if left is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y - 1];				// left
		tempState[x][y - 1] = 0;
		this->ex3 = new Node(tempState, tempState.size());
		this->ex3->prev = this;
	}
	if (y + 1 < sz) {										// check if right is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y + 1];				// right
		tempState[x][y + 1] = 0;
		this->ex4 = new Node(tempState, tempState.size());
		this->ex4->prev = this;
	}
	

	if (posmove > 4 || posmove < 2) {
		cout << "Error: Invalid number of possible moves. Expansion failed." << endl;
	}

}

Node generalSearch(Node n, int choice) {
	deque<Node> queue;										// max size queue; how many nodes were expanded
	queue.push_back(n);

	vector<vector<int>> goalState;							// Creating a dynamic goal state based on problem state size
	vector<int> blank;
	for (int i = 0; i < n.state.size(); i++) {
		goalState.push_back(blank);
		for (int j = 0; j < n.state.size(); j++) {
			goalState.at(i).push_back(j + i * n.state.size() + 1);
		}
	}
	goalState[n.state.size() - 1][n.state.size() - 1] = 0;

	Node goalNode(goalState, n.state.size());
	goalNode.print();

	vector<vector<int>> errState = { {-1} };
	Node error(errState, 1);

	while (1) {
		if (!(queue.empty())) {
			return error;
		}
		queue.pop_front();
		if (n.state == goalState) {
			return n;
		}
		

	}
}

deque<Node> queuefxn(Node n, deque<Node> queue, int choice) {
	deque<Node> temp;
	return temp;
}

vector<int> findInd(vector<vector<int>> state, int num) {
	vector<int> index;
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < state.size(); j++) {
			if (state[i][j] == num) {
				index.push_back(i);
				index.push_back(j);
			}
		}
	}

	if (index.size() != 2) {							// Error catching
		index.push_back(-1);
		index.push_back(-1);
	}
	return index;
}

void Node::print() {
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
}
