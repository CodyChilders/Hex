#include <vector>
#include "Graph.h"
#ifndef HEXAI_H
#define HEXAI_H


typedef std::vector<int> intvec;

class HexAI{

public:
	HexAI(int s);
	~HexAI();
	int* getMove(std::vector<intvec> board, int turn, bool pieRuleInvoked, Graph* edges, int lastPlayerMoveI, int lastPlayerMoveJ);
	
private:
	int* RandomMove(std::vector<intvec> board);
	void AIRandomMove(std::vector<intvec> board);
	void AI (std::vector<intvec> board);
	void MonteCarlo(std::vector<intvec> board, int turn, bool pieRuleInvoked, int lastPlayerMoveI, int lastPlayerMoveJ, Graph* edges);
	void connectingMove(std::vector<intvec> board, int playerLastMoveI, int playerLastMoveJ, Graph* edges);
	void advancingMove(std::vector<intvec> board, int playerLastMoveI, int playerLastMoveJ, Graph* edges);
	void recursiveGame(std::vector<intvec> board, int nextMoveI, int nextMoveJ, int player, Graph* edges, int depth);

	//taken from hex.cpp so the ai can check on its own
	int coordinateConversion(int c1, int c2);
	bool inBounds(int x);
	bool upLeft(std::vector<intvec> v, int i, int j);
	bool upRight(std::vector<intvec> v, int i, int j);
	bool left(std::vector<intvec> v, int i, int j);
	bool right(std::vector<intvec> v, int i, int j);
	bool downLeft(vector<intvec> v, int i, int j);
	bool downRight(vector<intvec> v, int i, int j);
	bool playerOneCheck(Graph* graph);
	bool playerTwoCheck(Graph* graph);

	//
	//variables
	//
	int compMadeMove; //decides if it is the first turn or not
	int compMoveI; //row
	int compMoveJ; //column
	int size; //size of the board
	
	//board data
	int middle;
	int middleIActuallyTaken;
	int middleJActuallyTaken;
	
	//game run data
	int gamesWon;
	int gamesLost;
};

#endif