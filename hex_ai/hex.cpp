//Cody Childers
//cchilder
//Kyle Sullivan
//kyrsulli

//Hex game

#include <vector>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "Graph.h"
#include "HexAI.h"

static int size;

using namespace std;

typedef enum playerColor{
	neutral, p1, p2
}playerColor;

typedef vector<int> intvec;

//prints underscores for formatted output
inline void printUnderscores(){
	cout << "  ";
	for(int i = 0; i < size * 3 + 2; i++){
		cout << '_';
	}
	cout << endl;
}

//prints spaces for formatted output
inline void printSpaces(int num){
	for(int i = 0; i < num; i++){
		cout << ' ';
	}
}

//prints the 1-size numbers across the top and bottom
inline void printColumnNumbers(int num){
	printSpaces(num);
	for(int i = 0; i < size; i++){
		cout << i << "  ";
	}
	cout << endl;
}

//takes in the game board 2d vector, and prints it out staggered
inline void printBoard(vector<intvec> b){
	//take care of the top and bottom
	printColumnNumbers(3);
	printUnderscores();
	//indexes through vector, printing whats there
	for(int offset = 0; offset < size; offset++){//also row
		printSpaces(offset);
		if(offset < 10){
			cout << offset << ' ';
		}else{
			cout << offset;
		}
		// "\\" prints a backslash char
		cout << "\\";
		for(int hex = 0; hex < size; hex++){//also column
			//cout << '(' << (b[offset][hex] == 0?' ':b[offset][hex]) << ')' << ' ';
			cout << '(';
			if(b[offset][hex] == 0){
				cout << ' ';
			}else{
				cout << b[offset][hex];
			}
			cout << ')';
		}
		cout << "\\ ";
		cout << offset << endl;
	}
	printSpaces(size);
	printUnderscores();
	printSpaces(size + 2);
	printColumnNumbers(2);
}

//this function returns a unique 1 index conversion, which
//corresponds to where in the graph this node goes
inline int coordinateConversion(int c1, int c2){
	return size * c1 + c2 + 1;
}

//makes sure the check is in an acceptable range
//used to help index out of range problems by saying
//its ok before indexing
inline bool inBounds(int x){
	if(x < 0 or x >= size){
		return false;
	}else{
		return true;
	}
}

//these next 6 return true if the cell in that position is the same as the current one
inline bool upLeft(vector<intvec> v, int i, int j){
	int checkY = i - 1;
	int checkX = j;
	if ( not inBounds(checkX)){
		return false;
	}
	if ( not inBounds(checkY)){
		return false;
	}
	//if current coordinate is the same type as next, return true
	if (v[i][j] == v[i-1][j]){
		return true;
	}
	return false;
}

inline bool upRight(vector<intvec> v, int i, int j){
	int checkY = i - 1;
	int checkX = j + 1;
	if ( not inBounds(checkX)){
		return false;
	}
	if ( not inBounds(checkY)){
		return false;
	}
	//if current coordinate is of same type as next, return true
	if(v[i][j] == v[i-1][j+1]){
		return true;
	}
	return false;
}

inline bool left(vector<intvec> v, int i, int j){
	int checkX = j - 1;
	if( not inBounds(checkX)){
		return false;
	}
	if(v[i][j] == v[i][j-1]){
		return true;
	}
	return false;
}

inline bool right(vector<intvec> v, int i, int j){
	int checkX = j + 1;
	if( not inBounds(checkX)){
		return false;
	}
	if(v[i][j] == v[i][j+1]){
		return true;
	}
	return false;
}

inline bool downLeft(vector<intvec> v, int i, int j){
	int checkX = j - 1;
	int checkY = i + 1;
	if(not inBounds(checkX)){
		return false;
	}
	if(not inBounds(checkY)){
		return false;
	}
	if(v[i][j] == v[i+1][j-1]){
		return true;
	}
	return false;
}

inline bool downRight(vector<intvec> v, int i, int j){
	int checkX = j;
	int checkY = i + 1;
	if(not inBounds(checkX)){
		return false;
	}
	if(not inBounds(checkY)){
		return false;
	}
	if(v[i][j] == v[i+1][j]){
		return true;
	}
	return false;
}

inline bool playerOneCheck(Graph* graph){
	//for each node across the top
	for(int i = 0; i < (size + 1); ++i){
		//run BFS(i)
		graph->Prim(i, p1);
		//for each node across the bottom
		for(int j = (size * (size - 1) + 1); j < ((size * size) + 1); ++j){
			//does there exist a path from i to j?
			//cout << i << " -> " << j << ": " << graph->getDist(j) << endl;
			if(graph->getDist(j) != INF)
				return true;
				//if so return true
		}
	}
	//return false
	return false;
}

inline bool playerTwoCheck(Graph* graph){
	//for each node across the left
	for(int i = 1; i < ((size * (size - 1)) + 2); i = i + size){
		//run BFS(i)
		graph->Prim(i, p2);
		//for each node across the right
		for(int j = size; j < ((size * size) + 1); j = j + size){
			//does there exist a path from i to j?
			//cout << i << " -> " << j << ": " << graph->getDist(j) << endl;
			if(graph->getDist(j) != INF)
				return true;
				//if so return true
		}
	}
	//return false
	return false;
}

int main(void){

	//get size from the user
	while(true){
		cout << "Would you like an 11x11 or 13x13 board? Enter an 11 or a 13: ";
		cin >> size;
		cout << endl;
		if(cin.fail()){
			cout << "Please enter an integer, 11 or 13" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if(size == 11 or size == 13){
			break;
		}
		cout << "Please enter an 11 or a 13: " << endl << endl;
	}
	//we have size, setup the AI
	HexAI* ai = new HexAI(size);
	//it needs this too
	bool pieRuleInvoked = false;
	
	//setup game
	Graph* graphBoard = new Graph(size * size + 1);
	//http://www.cplusplus.com/reference/vector/vector/?kw=vector
	//vector < vector < int > > pieceBoard(size);
	vector<intvec> pieceBoard;
	
	//initialize board, 0 it out
	//creates a 2d vector, all holding 0
	for(int i = 0; i < size; i++){
		intvec temp;
		pieceBoard.push_back(temp);
		for(int j = 0; j < size; j++){
			pieceBoard[i].push_back(neutral);
		}
	}
	
	//actual game
	cout << "Player 1 (computer) is going top to bottom, player 2 (you) are going left to right" << endl << endl;
	
	//printBoard(pieceBoard);
	cout << endl;
	
	for(int turn = 0; /*no exit condition*/; turn++){
		int player = (turn % 2) + 1;
		if(player == 2){
			//cout << "Player " << player << "'s turn" << endl;
			cout << "Your turn." << endl;
			cout << "Enter the row and column coordinate with only a space inbetween, such as: 5 8" << endl;
			if(turn == 1){
				cout << endl << "Keep in mind that the pie rule is in effect.  Simply enter the coordinates for my hex if you would like to take it." << endl << endl;
			}
		}else{//player is 1 (AI)
			cout << endl <<"Computer's turn.  Please give it a few moments to think." << endl;
		}
		//get their move
		int i, j;
		while(true){
			//grab i and j, make sure they are ints
			if(player == p2){
				bool inputOkay = true;
				cin >> i;
				if(cin.fail() ){ //if type wasn't right
					cin.clear(); //clear stream
					cin.ignore(); //ignore left over data
					cout << "First value is invalid input! Valid coordinates are in range 0 to " << size - 1 << endl;
					inputOkay = false;
				}
				cin >> j;
				if(cin.fail() ){ //if type wasn't right
					cin.clear(); //clear stream
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignore left over data
					cout << "Second value is invalid input! Valid coordinates are in range 0 to " << size - 1 << endl;
					inputOkay = false;
				}
				if(not(inputOkay)){
					continue;
				}
				//we have ints, make sure they are in range
				if(i < 0 or i >= size or j < 0 or j >= size){
					//prompt that input is wrong
					cout << "Invalid input! Valid coordinates are in range 0 to " << size - 1 << endl;
					continue;
				}
				//pie rule:
				//one time swap of the first piece
				if(pieceBoard[i][j] != 0 and turn == 1){
					pieRuleInvoked = true;
					break;
				}
				if(pieceBoard[i][j] != 0){//ignores pie rule
					cout << "Space is already taken, try again." << endl;
					continue;
				}
			}else{ //player is AI
				//set i and j with computer move
				int* move = ai->getMove(pieceBoard, turn, pieRuleInvoked, new Graph(graphBoard), i, j);
				i = move[0];
				j = move[1];
				delete move;
				cout << endl << "My move is " << i << " " << j << endl << endl;
				//i = j = 0;
			}
			break;
			
		}//close while(true) check loop
		
		pieceBoard[i][j] = player;
		
		//checks for adjacent colors
		
		//if i is rows and j is columns, adjacent ones are
		// (i, j+1), (i, j-1), (i-1, j), (i-1, j+1), (i+1, j-1), (i+1, j)
		//if any of those are detected, graphBoard.addEdge( (i, j) to (whatever))
		//we have to figure out a way to assign each thing a master coordinate to use in the graph node calculation
		if( upLeft(pieceBoard, i, j)){ //(i-1, j)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i - 1, j);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added upleft" << endl;
		}
		if( upRight(pieceBoard, i, j)){//(i-1, j+1)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i - 1, j + 1);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added upright" << endl;
		}
		if( left(pieceBoard, i, j)){//(i, j-1)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i, j - 1);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added left" << endl;
		}
		if(right(pieceBoard, i, j)){//(i, j+1)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i, j + 1);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added right" << endl;
		}
		if(downLeft(pieceBoard, i, j)){//(i+1, j-1)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i + 1, j - 1);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added downleft" << endl;
		}
		if(downRight(pieceBoard, i, j)){//(i+1, j)
			int a = coordinateConversion(i, j);
			int b = coordinateConversion(i + 1, j);
			graphBoard->addEdge(a, b, 1, player);
			//cout << "edge added downright" << endl;
		}
		
		//check if {Dijkstra or Prim} can run across, using the int player variable as color
		//and running through all combinations of the top row to all combinations of the
		//bottom row, or left to right depending on turn
		
		//Start at turn 11
		//if(turn >= 11){
		
		//Check for player's turn
			if(player == 1){
				if(playerOneCheck(graphBoard)){
					cout << "Player 1 wins!!" << endl;
					printBoard(pieceBoard);
					break;
				}
					
			}
			else{//player == 2
				if(playerTwoCheck(graphBoard)){
					cout << "Player 2 wins!!" << endl;
					printBoard(pieceBoard);
					break;
				}
			}
		//pretty self explanatory
		printBoard(pieceBoard);
		//if(turn == 2)
		//	break;
	}
	delete graphBoard;
	vector<intvec>().swap(pieceBoard);
	
	return 0;
}
	
