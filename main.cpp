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
int maxQSize = 0;

vector<int> findInd(vector<vector<int>> state, int num);
int misHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth);
int manHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth);

// ----- UNORIGINAL CODE USED TO TRACK CPUTIME FROM: https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
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
// -----

class Node {
public: 
	Node();
	Node(vector<vector<int>> state, int size);
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

Node customInp();
vector<vector<int>> goalStateGen(int size);
void queuefxn(Node &n, int choice, map<vector<vector<int>>, int> &map);
Node generalSearch(Node &n, int choice);
void sortN();
void assignCost(int choice, vector<vector<int>> goal);
// ----- UNORIGINAL CODE USED TO COMPARE AND SORT THE QUEUE FROM: https://stackoverflow.com/questions/5174115/sorting-a-vector-of-objects-by-a-property-of-the-object
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
	
	Node test;
	vector<vector<int>> deflt{  {0,7,2},
								{4,6,1},
								{3,5,8} };

	if (inp == 2) {
		test = customInp();
	}
	else {
		test.state = deflt;
	}


	int choice = 0;
	cout << "Enter your choice of algorithm\n\t1. Uniform Cost Search\n\t2. A* with the Misplaced Tile heuristic.\n\t3. A* with the Manhattan distance heuristic." << endl;
	cin >> choice;

	double begintimer = get_cpu_time();
	Node sol = generalSearch(test, choice);
	double endtimer = get_cpu_time();
	double elapsedtimer = (endtimer - begintimer);

	cout << "\nGoal State Found!\n";

	printf("Time measured: %3f seconds.\n", elapsedtimer);
	cout << "Nodes Expanded: " << nodesExpanded << endl;
	cout << "Max Queue Size: " << maxQSize << endl;
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
	cost = 0;
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
	cost = 0;
};

Node customInp() {											// Function to get a custom puzzle state
	string str;
	vector<int> blank;
	vector<vector<int>> inpState;
	string temp;
	cin.ignore();
	cout << "Please enter in tiles in rows separated by spaces or tabs. (Ex: \"a b c\" or \t\"a\tb\tc\")" << endl;

	for (int i = 0; i < 3; i++) {
		cout << "Row " << i + 1 << ": ";
		getline(cin, str);
		cout << endl;
		stringstream ss(str);
		inpState.push_back(blank);
		for (int j = 0; j < 3; j++) {
			ss >> temp;
			inpState.at(i).push_back(stoi(temp));
		}
	}
	return Node(inpState, 3);
}

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
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
			queue.push_back(*this->ex1);
			nodesExpanded++;
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
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
			queue.push_back(*this->ex2);
			nodesExpanded++;
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
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
			queue.push_back(*this->ex3);
			nodesExpanded++;
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
			map.insert(pair<vector<vector<int>>, int>(tempState, 1));
			queue.push_back(*this->ex4);
			nodesExpanded++;
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

	return goalState;
}

Node generalSearch(Node &n, int choice) {
	map<vector<vector<int>>, int> map;
	queue.push_back(n);
	int depth = 0;
	vector<vector<int>> goalState = goalStateGen(n.state.size());		// Creating a dynamic goal state based on problem state size
	vector<vector<int>> errState = { {-1} };
	n.d = depth;
	Node temp = n;
	Node error(errState, 1);
	map.insert(pair<vector<vector<int>>,int>(temp.state, temp.d));
	int printOut = 0;
	
	// Pseudocode starts here

	while (1) {
		if (queue.empty()) {
			return error;
		}
		if (printOut < 3) {
			cout << "The best state to expand with a g(n) = " << queue.front().d << " and h(n) = " << queue.front().cost - queue.front().d << " is..." << endl;
			queue.front().print();
			cout << "\t\t\tExpanding this node..." << endl;
			printOut++;
		}

		temp = queue.front();
		queue.pop_front();

		if (temp.state == goalState) {
			
			return temp;
		}
		queuefxn(temp, choice, map);
		assignCost(choice, goalState);
		sortN();
	}
}

void queuefxn(Node &n, int choice, map<vector<vector<int>> , int> &map) {			// proxy function to queue children
	n.expand(n, map);
	
	if (maxQSize < queue.size()) {
		maxQSize = queue.size();
	}

	return;
}

vector<int> findInd(vector<vector<int>> state, int num) {							// Unused Function because it increases run time
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

int misHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth) {		// Misplaced Tiles Heuristic Function to calculate the cost
	int sum = 0;

	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < state.size(); j++) {
			if (state[i][j] != 0) {
				if (state[i][j] != goal[i][j]) {
					sum++;
				}
			}
		}
	}
	sum += depth;

	return sum;
}

int manHeur(vector<vector<int>> state, vector<vector<int>> goal, int depth) {		// Manhattan Distance Heuristic Function to calculate the cost
	int sum = 0;

	int stateRow = 0;
	int stateCol = 0;
	int goalRow = 0;
	int goalCol = 0;

	int size = state.size();

	for (unsigned k = 1; k < size * size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (state[i][j] == k) {
					stateRow = i;
					stateCol = j;
				}
				if (goal[i][j] == k) {

					goalRow = i;
					goalCol = j;
				}
				
			}
		}
		sum += abs(goalRow - stateRow) + abs(goalCol - stateCol);
	}
	
	sum += depth;

	return sum;
}

void assignCost(int choice, vector<vector<int>> goal) {									// Assigns Costs to the Nodes dependent on Heuristic chosen
	if (choice == 1){
		return;
	}
	if (choice == 2) {
		for (int i = 0; i < queue.size(); i++) {
			if (queue.at(i).cost == 0) {
				queue.at(i).cost = misHeur(queue.at(i).state, goal, queue.at(i).d);
			}
			
		}
	}
	else if (choice == 3) {
		for (int i = 0; i < queue.size(); i++) {
			if (queue.at(i).cost == 0) {
				queue.at(i).cost = manHeur(queue.at(i).state, goal, queue.at(i).d);
			}
		}
	}
	return;
}

void sortN() {																			// Sort Function for the queue
	int minInd = 0;
	vector<vector<int>> blank;
	Node temp(blank, blank.size());
	
	sort(queue.begin(), queue.end(), &comparator);

	return;
}

void Node::print() {																	// Print Function for the states
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
}
