#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

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
	void print();
};

int main() {
	
	int choice = 0;

	cout << "Welcome to Matthew Lee\'s 8-puzzle solver. \n Type\"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
	cin >> choice;

	vector<vector<int>> deflt {	{1,2,3},
								{4,8,0},
								{7,6,5} };

	Node test(deflt, 3);


	cout << "-------------before node print------------------" << endl;

	test.print();

	cout << "-------------after node print------------------" << endl;
	

}

Node::Node(vector<vector<int>> state, int size) {
	this->state = state;
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
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
void Node::print() {
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			cout << this->state[i][j] << " ";
		}
		cout << endl;
	}
}
