#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
using namespace std;

vector<int> findInd(vector<vector<int>> state, int num);
void removeDupes();

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
	int d;											// depth
	

	void print();
	deque<Node> expand(deque<Node> queue);
	
};

vector<vector<int>> goalStateGen(int size);
deque<Node> queuefxn(Node n, deque<Node> queue, int choice);
Node generalSearch(Node n, int choice);

int main() {
	
	int inp = 0;

	cout << "Welcome to Matthew Lee\'s 8-puzzle solver. \nType \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
	cin >> inp;
	
	int choice = 0;
	cout << "Enter your choice of algorithm\n\t1. Uniform Cost Search\n\t2. A* with the Misplaced Tile heuristic.\n\t3.  A* with the Manhattan distance heuristic." << endl;
	cin >> choice;

	vector<vector<int>> deflt {	{1,3,6},
								{5,0,2},
								{4,7,8} };

	Node test(deflt, 3);


	cout << "-------------before node print------------------" << endl;

	test.print();

	cout << "-------------after node print------------------" << endl;
	
	cout << "-------------before index print------------------" << endl;

	vector<int> newtest = findInd(test.state, 2);
	cout << "(" << newtest.at(0) << ", " << newtest.at(1) << ")" << endl;

	cout << "-------------after index print------------------" << endl;

	deque<Node> q;
	q.push_back(test);

	/*
	cout << "-------------before test expand-----------------" << endl;
	q = test.expand(q);
	cout << "-------------after test expand------------------" << endl;
	cout << endl << "State move up" << endl;
	test.ex1->print();
	//cout << endl << "State move down" << endl;
	//test.ex2->print();
	cout << endl << "State move left" << endl;
	test.ex3->print();
	//cout << endl << "State move right" << endl;
	//test.ex4->print();

	cout << "-------------checking queue-----------------" << endl;

	cout << endl << "OG State" << endl;
	q.at(0).print();
	cout << endl << "State move up" << endl;
	q.at(1).print();
	//cout << endl << "State move down" << endl;
	//q.at(2).print();
	cout << endl << "State move left" << endl;
	q.at(2).print();
	//cout << endl << "State move right" << endl;
	//q.at(4).print();

	cout << "-------------end of queue checking-----------------" << endl;
	*/
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

	cout << "-------------testing state comparison---------------------" << endl;

	vector<vector<int>> randstate1 = {	{2,3,4},
										{5,6,7},
										{1,8,0} };
	vector<vector<int>> randstate2 = {	{3,4,1},
										{5,2,6},
										{7,8,0} };
	vector<vector<int>> randstate3 = {  {2,3,4},
										{5,6,7},
										{1,8,0} };

	Node comp1(randstate1, 3);
	Node comp2(randstate2, 3);
	Node comp3(randstate3, 3);

	cout << "comp1 == comp1: ";

	if (comp1.state == comp1.state) {								// should print true
		cout << "true";
	}
	else {
		cout << "false";
	}
	cout << endl;
	cout << "comp1 == comp2: ";

	if (comp1.state == comp2.state) {								// should print false
		cout << "true" << endl;
	}
	else {
		cout << "false";
	}
	cout << endl;
	cout << "comp2 == comp2: ";

	if (comp2.state == comp2.state) {								// should print true
		cout << "true";
	}
	else {
		cout << "false";
	}
	cout << endl;

	cout << "comp1 == comp3: ";										// should print true
	if (comp1.state == comp3.state) {
		cout << "true";
	}
	else {
		cout << "false";
	}
	cout << endl;

	cout << "-------------state comparison test finished---------------------" << endl;

	cout << "-------------testing general search---------------------" << endl;

	Node sol = generalSearch(test, choice);
	sol.print();
	cout << "depth: " << sol.d << endl;
	cout << "-------------general search test finished---------------------" << endl;

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
	d = 0;
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

deque<Node> Node::expand(deque<Node> queue) {
	vector<int> indmove = findInd(this->state, 0);			// finds the blank tile aka index of "0"

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
		this->ex1->d = this->d + 1;
		queue.push_back(*this->ex1);
	}
	if (x + 1 < sz) {										// check if down is valid
		tempState = this->state;
		tempState[x][y] = tempState[x + 1][y];				// down
		tempState[x + 1][y] = 0;
		this->ex2 = new Node(tempState, tempState.size());
		this->ex2->prev = this;
		this->ex2->d = this->d + 1;
		queue.push_back(*this->ex2);
	}
	if (y - 1 >= 0) {										// check if left is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y - 1];				// left
		tempState[x][y - 1] = 0;
		this->ex3 = new Node(tempState, tempState.size());
		this->ex3->prev = this;
		this->ex3->d = this->d + 1;
		queue.push_back(*this->ex3);
	}
	if (y + 1 < sz) {										// check if right is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y + 1];				// right
		tempState[x][y + 1] = 0;
		this->ex4 = new Node(tempState, tempState.size());
		this->ex4->prev = this;
		this->ex4->d = this->d + 1;
		queue.push_back(*this->ex4);
	}

	if (posmove > 4 || posmove < 2) {
		cout << "Error: Invalid number of possible moves. Expansion failed." << endl;
	}
	return queue;
}

vector<vector<int>> goalStateGen(int size) {
	vector<vector<int>> goalState;							
	vector<int> blank;
	for (int i = 0; i < size; i++) {
		goalState.push_back(blank);
		for (int j = 0; j < size; j++) {
			goalState.at(i).push_back(j + i * size + 1);
		}
	}
	goalState[size - 1][size - 1] = 0;

	Node goalNode(goalState, size);
	//goalNode.print();

	return goalState;
}
// (1, 3) (3, 2)
Node generalSearch(Node n, int choice) {
	deque<Node> queue;													// max size queue; how many nodes were expanded
	queue.push_back(n);
	int depth = 0;

	vector<vector<int>> goalState = goalStateGen(n.state.size());		// Creating a dynamic goal state based on problem state size
	vector<vector<int>> errState = { {-1} };
	n.d = depth;

	Node temp = n;
	Node error(errState, 1);

	while (1) {
		if (queue.empty()) {
			return error;
		}
		//queue.front().print();
		//cout << endl;
		temp = queue.front();
		queue.pop_front();

		if (temp.state == goalState) {
			return temp;
		}
		queue = queuefxn(temp, queue, choice);
	}
}

deque<Node> queuefxn(Node n, deque<Node> queue, int choice) {					// proxy function to queue children
	queue = n.expand(queue);
	/*
	for (int j = 0; j < queue.size(); j++) {
		cout << "...." << endl;
		queue.at(j).print();
		cout << endl;
		queue.at(j).prev->print();
		cout << "...." << endl;
	}

	for (int i = 0; i < queue.size(); i++) {
		cout << "====\n";
		cout << i << ": " << queue.at(i).d << endl;
		cout << "====\n";
	}*/

	return queue;
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
