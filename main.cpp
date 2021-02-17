#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <windows.h>

using namespace std;

int nodesExpanded = 0;

vector<int> findInd(vector<vector<int>> state, int num);
//bool isDupe(Node n, map<Node, int> map);
int misHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth);
int manHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth);

double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
				((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}

class Node {
public: 
	Node();
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
	int cost;										// cost of node g(n) + h(n)
	

	void print();
	void expand(Node& n, map<vector<vector<int>>, int> &map);
	
};

deque<Node> queue;

vector<vector<int>> goalStateGen(int size);
void queuefxn(Node &n, int choice, map<vector<vector<int>>, int> &map);
Node generalSearch(Node &n, int choice);
void sortN();
void assignCost(int choice, vector<vector<int>> goal);
bool comparator(const Node& lhs, const Node& rhs) {
	if (lhs.cost == rhs.cost) {
		return lhs.d < rhs.d;
	}

	return lhs.cost < rhs.cost;
}


int main() {
	
	int inp = 0;

	cout << "Welcome to Matthew Lee\'s 8-puzzle solver. \nType \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
	cin >> inp;
	
	int choice = 0;
	cout << "Enter your choice of algorithm\n\t1. Uniform Cost Search\n\t2. A* with the Misplaced Tile heuristic.\n\t3. A* with the Manhattan distance heuristic." << endl;
	cin >> choice;

	vector<vector<int>> deflt {	{7,1,2},
								{4,8,5},
								{6,3,0} };

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
	

	double begintimer = get_cpu_time();

	
	
	Node sol = generalSearch(test, choice);
	sol.print();
	cout << "depth: " << sol.d << endl;


	double endtimer = get_cpu_time();
	double elapsedtimer = (endtimer - begintimer);

	cout << "begin: " << begintimer << endl;
	cout << "end: " << endtimer << endl;

	printf("Time measured: %3f seconds.\n", elapsedtimer);

	cout << "-------------general search test finished---------------------" << endl;
	
	
	cout << "----------------Testing maps---------------------" << endl;

	map<vector<vector<int>>, int> m;
	if (m.find(deflt) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(deflt, 1));
	}
	else {
		cout << "value: " << m.find(deflt)->second << endl;
	}
	if (m.find(randstate1) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(randstate1, 1));
	}
	else {
		cout << "value: " << m.find(randstate1)->second << endl;
	}
	if (m.find(randstate2) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(randstate2, 1));
	}
	else {
		cout << "value: " << m.find(randstate2)->second << endl;
	}
	if (m.find(deflt) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(deflt, 1));
	}
	else {
		cout << "value: " << m.find(deflt)->second << endl;
	}
	if (m.find(randstate1) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(randstate1, 1));
	}
	else {
		cout << "value: " << m.find(randstate1)->second << endl;
	}
	if (m.find(randstate2) == m.end()) {
		cout << "not found" << endl;
		m.insert(pair<vector<vector<int>>, int>(randstate2, 1));
	}
	else {
		cout << "value: " << m.find(randstate2)->second << endl;
	}

	Node* yup = new Node({ {1} }, 1);
	for (auto it = m.cbegin(); it != m.cend(); ++it) {
		yup = new Node(it->first, 3);
		yup->print();
		cout << endl;
	}


	cout << "-------------Finished testing maps---------------------" << endl;

	/*cout << "-------------Testing Heuristics---------------------" << endl;

	int misheurcost = 0;
	int manheurcost = 0;
	misheurcost = misHeur(test.state, goalNode.state, test.d);
	manheurcost = manHeur(test.state, goalNode.state, test.d);

	cout << "misHeur cost: " << misheurcost << endl;
	cout << "manHeur cost: " << manheurcost << endl;

	cout << "-------------Finished Heuristic test---------------------" << endl;*/
	cout << "-------------Print solution steps---------------------" << endl;
	
	Node waffle = sol;
	
	sol.print();
	cout << "size of head: " << sol.prev->state.size() << endl;
	cout << "depth: " << sol.d << endl;
	cout << "depth of head: " << sol.prev->d << endl;

	/*
	while (waffle.prev != NULL) {
		waffle.print();
		waffle = *waffle.prev;
	}
	*/
	cout << "-------------Finished printing---------------------" << endl;

	cout << "Nodes Expanded: " << nodesExpanded << endl;

	//cout << "test node state: " << test.ex1->d;

	Node* asdf = &test;
	Node qwer(randstate3, randstate3.size());
	asdf->ex1 = &q.front();
	asdf->print();
	cout << endl;
	asdf->ex1->print();

}

Node::Node() {
	prev = NULL;
	ex1 = NULL;
	ex2 = NULL;
	ex3 = NULL;
	ex4 = NULL;
	this->state = { {} };
	sz = 0;
	d = 0;
}

Node::Node(vector<vector<int>> state, int size) {
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

void Node::expand(Node &n, map<vector<vector<int>>, int> &map) {
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
		n.ex1 = new Node(tempState, tempState.size());
		n.ex1->prev = &n;																				// SOLVE THIS: this->ex1->prev is being assigned to garbage value
		n.ex1->d = this->d + 1;

		if (map.find(tempState) == map.end()) {				// not found in dictionary
//			cout << "up: size before: " << map.size() << endl;
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
//			cout << "up: size after: " << map.size() << endl;
			queue.push_back(*this->ex1);
//			cout << "up: new state found!" << endl;
			nodesExpanded++;
		}
		else {
//			cout << "up: this is already inside the dict" << endl;
		}
	}
	if (x + 1 < sz) {										// check if down is valid
		tempState = this->state;
		tempState[x][y] = tempState[x + 1][y];				// down
		tempState[x + 1][y] = 0;
		this->ex2 = new Node(tempState, tempState.size());
		this->ex2->prev = &n;
		this->ex2->d = this->d + 1;

		if (map.find(tempState) == map.end()) {				// not found in dictionary
//			cout << "down: size before: " << map.size() << endl;
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
//			cout << "down: size after: " << map.size() << endl;
			queue.push_back(*this->ex2);
//			cout << "down: new state found!" << endl;
			nodesExpanded++;
		}
		else {
//			cout << "down: this is already inside the dict" << endl;
		}
	}
	if (y - 1 >= 0) {										// check if left is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y - 1];				// left
		tempState[x][y - 1] = 0;
		this->ex3 = new Node(tempState, tempState.size());
		this->ex3->prev = &n;
		this->ex3->d = this->d + 1;

		if (map.find(tempState) == map.end()) {				// not found in dictionary
//			cout << "left: size before: " << map.size() << endl; 
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
//			cout << "left: size after: " << map.size() << endl;
			queue.push_back(*this->ex3);
//			cout << "left: new state found!" << endl;
			nodesExpanded++;
		}
		else {
//			cout << "left: this is already inside the dict" << endl;
		}
	}
	if (y + 1 < sz) {										// check if right is valid
		tempState = this->state;
		tempState[x][y] = tempState[x][y + 1];				// right
		tempState[x][y + 1] = 0;
		this->ex4 = new Node(tempState, tempState.size());
		this->ex4->prev = &n;
		this->ex4->d = this->d + 1;

		if (map.find(tempState) == map.end()) {				// not found in dictionary
//			cout << "right: size before: " << map.size() << endl; 
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
//			cout << "right: size after: " << map.size() << endl;
			queue.push_back(*this->ex4);
//			cout << "right: new state found!" << endl;
			nodesExpanded++;
		}
		else {
//			cout << "right: this is already inside the dict" << endl;
		}
	}

	if (posmove > 4 || posmove < 2) {
		cout << "Error: Invalid number of possible moves. Expansion failed." << endl;
	}
	return;
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
Node generalSearch(Node &n, int choice) {
	map<vector<vector<int>>, int> map;
																		// max size queue; how many nodes were expanded
	queue.push_back(n);
	int depth = 0;

	vector<vector<int>> goalState = goalStateGen(n.state.size());		// Creating a dynamic goal state based on problem state size
	vector<vector<int>> errState = { {-1} };
	n.d = depth;

	Node temp = n;
	Node error(errState, 1);

	map.insert(pair<vector<vector<int>>,int>(temp.state, temp.d));

	while (1) {
		if (queue.empty()) {
			return error;
		}
//		queue.front().print();
//		cout << endl;
		temp = queue.front();
		queue.pop_front();
		cout << temp.cost - temp.d << " " << temp.d << endl;
		if (temp.state == goalState) {
			
			return temp;
		}
		queuefxn(temp, choice, map);
		assignCost(choice, goalState);
		sortN();
		/*
		cout << "head node: " << endl;
		n.print();
		if (n.ex1 != NULL) {
			cout << "child ex1 node: " << endl;
			n.ex1->print();
		}
		if (n.ex2 != NULL) {
			cout << "child ex2 node: " << endl;
			n.ex2->print();
		}
		if (n.ex3 != NULL) {
			cout << "child ex3 node: " << endl;
			n.ex3->print();
		}
		if (n.ex4 != NULL) {
			cout << "child ex4 node: " << endl;
			n.ex4->print();
		}
		
		for (int j = 0; j < queue.size(); j++) {
			cout << j << ": " << queue.at(j).cost << endl;
		}*/


	}
}

void queuefxn(Node &n, int choice, map<vector<vector<int>> , int> &map) {			// proxy function to queue children
	n.expand(n, map);
	
	/*for (int j = 0; j < queue.size(); j++) {
		cout << "...." << endl;
		queue.at(j).print();
		cout << endl;
		queue.at(j).prev->print();
		cout << "...." << endl;
	}
	
	for (int i = 0; i < queue.size(); i++) {
		cout << "====\n";
		cout << i << ": " << endl;
		queue.at(i).print();
		cout << endl;
		cout << "====\n";
	}
	/*Node* temp = new Node({ {1} }, 1);
	for (auto it = map.cbegin(); it != map.cend(); ++it) {
		temp = new Node(it->first, 3);
		temp->print();
		cout << endl;
	}
	
	cout << "0000000000000\n0000000000000\n0000000000000\n" << endl;
	for (int l = 0; l < queue.size(); l++) {
		cout << "`````````" << endl;
		queue.at(l).print();
		cout << "`````````" << endl;
	}*/

	

	return;
}

//bool isDupe(Node n, map<int, Node> map) {
//	map.find(n);
//}

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

int misHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth) {
	int sum = 0;

	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < state.size(); j++) {
			if (state[i][j] != 0) {
				if (state[i][j] != goal[i][j]) {
//					cout << "element at (" << i << ", " << j << "): " << state[i][j] << endl;
					sum++;
				}
			}
		}
	}
	sum += depth;

	return sum;
}

int manHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth) {
	int sum = 0;
	vector<int> ind1;
	vector<int> ind2;

	int r = 0;
	int c = 0;
	int r1 = 0;
	int c1 = 0;

	for (unsigned k = 1; k < 9; k++) {
		for (int i = 0; i < state.size(); i++) {
			for (int j = 0; j < state.size(); j++) {
				if (goal[i][j] == k) {
					//				cout << "element state at (" << i << ", " << j << "): " << state[i][j] << endl;
//					ind1 = findInd(state, goal[i][j]);
					//				cout << "element index at (" << ind1.at(0) << ", " << ind1.at(1) << "): " << state[i][j] << endl;

//					sum = sum + abs(ind1.at(0) - i);
//					sum = sum + abs(ind1.at(1) - j);
					r1 = i;
					c1 = j;
				}
				if (state[i][j] == k) {
					r = i;
					c = j;
				}
			}
		}
		sum += abs(r1 - r) + abs(c1 - c);
	}
	
	sum += depth;

	return sum;
}

void assignCost(int choice, vector<vector<int>> goal) {
	if (choice == 2) {
		for (int i = 0; i < queue.size(); i++) {
			queue.at(i).cost = misHeur(queue.at(i).state, goal, queue.at(i).d);
		}
	}
	else if (choice == 3) {
		for (int i = 0; i < queue.size(); i++) {
			queue.at(i).cost = manHeur(queue.at(i).state, goal, queue.at(i).d);
		}
	}
}

void sortN() {
	int minInd = 0;
	vector<vector<int>> blank;
	Node temp(blank, blank.size());
	
	sort(queue.begin(), queue.end(), &comparator);

	return;
}

void Node::print() {
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
}
