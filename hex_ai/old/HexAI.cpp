#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Graph.h"
#include "HexAI.h"

const int sampleGamesRunPerCheck = 100;

using namespace std;

typedef enum playerColor{
	neutral, p1, p2
}playerColor;

typedef std::vector<int> intvec;

HexAI::HexAI(int s){
	compMoveI = 0;
	compMoveJ = 0;
	compMadeMove = false;
	size = s;
	srand(time(NULL));
	
	//find the middle of the board for first move
	if(s == 11){
		middle = 5; 
	}else{ //s == 13
		middle = 6;
	}
}

HexAI::~HexAI(){
	//nothing needs to be deleted
}
/*******************************************************
coordinateConversaion() through playerTwoCheck() are 
copied from hex.cpp exactly
*******************************************************/

inline void printUnderscores(){
	int size = 11;
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
	int size = 11;
	for(int i = 0; i < size; i++){
		cout << i << "  ";
	}
	cout << endl;
}

//takes in the game board 2d vector, and prints it out staggered
inline void printBoard(vector<intvec> b){
	//take care of the top and bottom
	int size = 11;
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
inline int HexAI::coordinateConversion(int c1, int c2){
	return size * c1 + c2 + 1;
}

//makes sure the check is in an acceptable range
//used to help index out of range problems by saying
//its ok before indexing
inline bool HexAI::inBounds(int x){
	if(x < 0 or x >= size){
		return false;
	}else{
		return true;
	}
}

//these next 6 return true if the cell in that position is the same as the current one
inline bool HexAI::upLeft(vector<intvec> v, int i, int j){
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

inline bool HexAI::upRight(vector<intvec> v, int i, int j){
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

inline bool HexAI::left(vector<intvec> v, int i, int j){
	int checkX = j - 1;
	if( not inBounds(checkX)){
		return false;
	}
	if(v[i][j] == v[i][j-1]){
		return true;
	}
	return false;
}

inline bool HexAI::right(vector<intvec> v, int i, int j){
	int checkX = j + 1;
	if( not inBounds(checkX)){
		return false;
	}
	if(v[i][j] == v[i][j+1]){
		return true;
	}
	return false;
}

inline bool HexAI::downLeft(vector<intvec> v, int i, int j){
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

inline bool HexAI::downRight(vector<intvec> v, int i, int j){
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

inline bool HexAI::playerOneCheck(Graph* graph){
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

inline bool HexAI::playerTwoCheck(Graph* graph){
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


//when it throws its hands in the air
int* HexAI::RandomMove(vector<intvec> board){
	bool acceptableMove = false;
	int* move = new int[2];
	//cout << "Got into RandomMove at depth: " << depth << endl;
	while( not acceptableMove){
		int a = rand() % (size);// - 1);//it has trouble when its randomly on the bottom
		int b = rand() % (size);
	//	cout << "loopin in RandomMove at depth: " << depth << " with move (" << a << ", " << b << "), owned by player " << board[a][b] << endl;
		if(board[a][b] == neutral){
			acceptableMove = true;
			move[0] = a;
			move[1] = b;
		}
	}
	return move;
}

//this is antiquated, do not use
void HexAI::AIRandomMove(vector<intvec> board){
        bool acceptableMove = false;
        while( not acceptableMove){
                int a = rand() % (size - 1);//it has trouble when its randomly on the bottom
                int b = rand() % size;
                if(board[a][b] == neutral){
                        acceptableMove = true;
                        compMoveI = a;
                        compMoveJ = b;
                }
        }
}

//makes a decision based on the board, sets 2 variables
//that are read in as the move later on
//tries to go straight top to bottom, move left or right if that spot is taken.
//goes for a random move if it can't decide how to move
void HexAI::AI (vector<intvec> board){
	//catch seg faults that show up 1 turn after random indexes put it on the bottom row
	if(compMadeMove and compMoveI == (size - 1) ){ //if it randomly ends up on the bottom, move it to the top and start over, here to fix seg faults
		compMadeMove = false;
	}
	//determine starting position
	if( not compMadeMove ){//first move, pick a spot on the top
		bool goodguess = false;
		int startPos = rand() % size;
		while(!goodguess){
			if(board[0][startPos] != 0){
				startPos = rand() % size;
			}else{
				goodguess = true;
			}
		}
		compMoveI = 0;
		compMoveJ = startPos;
		compMadeMove = true;
	}else{//later moves
		int potentialMove = board[compMoveI + 1][compMoveJ];
		if(potentialMove == neutral){//try to just move down one
			compMoveI += 1;
		}else{//try downleft or just left or right
			potentialMove = board[compMoveI + 1][compMoveJ - 1];
			if(potentialMove == neutral){//downleft
				compMoveI += 1;
				compMoveJ -= 1;
			}else{//downLeft was taken, go left or right
				potentialMove = board[compMoveI][compMoveJ + 1];
				if(potentialMove == neutral){//go right
					compMoveJ += 1;
				}else{//hopefully left works?
					potentialMove = board[compMoveI][compMoveJ - 1];
					if(potentialMove == neutral){
						compMoveJ -= 1;
					}else{//ARG I give up, random
						/*AI*/RandomMove(board);
					}
				}
			}
		}
	}
	if(compMoveI < 0 or compMoveI >= size or compMoveJ < 0 or compMoveJ >= size){
		/*AI*/RandomMove(board);
	}
	//cout << endl << "My move is " << compMoveI << " " << compMoveJ << endl << endl;
}

//recursive call to check moves
//top level call should enter '1' as player, because the cpu is p1
//set up to take a move, rather than decide its own, so the top level can force constraints on it
void HexAI::recursiveGame(vector<intvec> board, int nextMoveI, int nextMoveJ, int player, Graph* edges, int depth){

//cout << "recursive game called, depth: " << depth << endl;
	//make the move
	board[nextMoveI][nextMoveJ] = player;
	//add the edges to the graph
	//note: this is taken exactly from hex.cpp's edge adding algorithm
	if( upLeft(board, nextMoveI, nextMoveJ)){ //(i-1, j)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI - 1, nextMoveJ);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added upleft" << endl;
	}
	if( upRight(board, nextMoveI, nextMoveJ)){//(i-1, j+1)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI - 1, nextMoveJ + 1);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added upright" << endl;
	}
	if( left(board, nextMoveI, nextMoveJ)){//(i, j-1)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI, nextMoveJ - 1);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added left" << endl;
	}
	if(right(board, nextMoveI, nextMoveJ)){//(i, j+1)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI, nextMoveJ + 1);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added right" << endl;
	}
	if(downLeft(board, nextMoveI, nextMoveJ)){//(i+1, j-1)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI + 1, nextMoveJ - 1);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added downleft" << endl;
	}
	if(downRight(board, nextMoveI, nextMoveJ)){//(i+1, j)
		int a = coordinateConversion(nextMoveI, nextMoveJ);
		int b = coordinateConversion(nextMoveI + 1, nextMoveJ);
		edges->addEdge(a, b, 1, player);
		//cout << "edge added downright" << endl;
	}
	//figure out who the next player is
	int nextPlayer;
	if(player == 1){
		nextPlayer = 2;
	}else{ //player == 2
		nextPlayer = 1;
	}
	//cout << "reached here with depth: " << depth << endl;
	//check to see if anyone won, or to keep going
	if(playerOneCheck(edges)){
	//	cout << "playeronewins" << endl;
		//printBoard(board);
		gamesWon++;
		delete edges;
		return;
	}else if(playerTwoCheck(edges)){
	//	cout << "playertwowins" << endl;
		//printBoard(board);
		gamesLost++;
		delete edges;
		return;
	}else{//no one won, so i'm gonna keep playing
		//find the next i/j
	//	cout << "reached even further with depth: " << depth << endl;
	//	if(depth==107) printBoard(board);
		int* move = RandomMove(board);
	//	cout << "final test at depth: " << depth << endl;
		int i = move[0];
		int j = move[1];
		delete move;
	//	cout << "about to call recursion at depth: " << depth << endl;
		recursiveGame(board, i, j, nextPlayer, new Graph(edges), depth + 1);
	//	cout << "past recursion at depth: " << depth << endl;
		delete edges;
		return;
	}
}

inline void HexAI::advancingMove(vector<intvec> board, int playerLastMoveI, int playerLastMoveJ, Graph* edges){
	//j increases by 1 for every 2 i decreases
	//j decreases by 1 for every 2 i increases
	int iOffset = 0;
	int jOffset = 0;
	
	//this is used to make sure that sample games were actually run
	int gamesPlayed = 0;
	
	//testst start hurr
	gamesWon = 0;
	gamesLost = 0;
	//holds values for determining next move
	int wins = 0;
	int winningI = -1;
	int winningJ = -1;
	//while the offsets are still on the board
	while(middleIActuallyTaken + iOffset <  size and
	      middleIActuallyTaken - iOffset >= 0    and
		  middleJActuallyTaken + jOffset <  size and
		  middleJActuallyTaken - jOffset >= 0)
	{
//cout << "entered advancing move main loop" << endl;
		//if I can play it, go ahead and try
		if(board[middleIActuallyTaken + iOffset][middleJActuallyTaken - jOffset] == 0){
			//count that we tried to play a game
			gamesPlayed++;
			//play the game a bunch of times
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
//cout << "playing a recursisve game" << endl;
				recursiveGame(board, middleIActuallyTaken + iOffset, middleJActuallyTaken - jOffset, p1, new Graph(edges), 0);
			}
cout << "Curr highest wins: " << wins << " with (" << winningI << ", " << winningJ << "), with challenging value of " << gamesWon << " with (" << (middleIActuallyTaken + iOffset) << ", " << (middleJActuallyTaken - jOffset) << ")" << endl;
			//if I was more successful, store this value
			if(wins /*>*/< gamesWon){
cout << "Debug?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken + iOffset;
				winningJ = middleJActuallyTaken - jOffset;
				//reset for next pass
				gamesWon = 0;
			}
		}
		//if I can play it, go ahead and try
		if(board[middleIActuallyTaken - iOffset][middleJActuallyTaken + jOffset] == 0){
			//count that we tried to play a game
			gamesPlayed++;
			//play the game a bunch of times
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
//cout << "playing the second recursive game" << endl;
				recursiveGame(board, middleIActuallyTaken - iOffset, middleJActuallyTaken + jOffset, p1, new Graph(edges), 0);
			}
			//if I was more successful, store this value
cout << "Curr highest wins: " << wins << " with (" << winningI << ", " << winningJ << "), with challenging value of " << gamesWon << " with (" << (middleIActuallyTaken - iOffset) << ", " << (middleJActuallyTaken + jOffset) << ")" << endl;
			if(wins < gamesWon){//*wins*/gamesLost /*>*/< gamesWon){
cout << "Debug 2 ?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken - iOffset;//+ iOffset;
				winningJ = middleJActuallyTaken + jOffset;//- jOffset;
				//reset for next pass
				gamesWon = 0;
			}
			
		}
		iOffset += 2;
		jOffset += 1;
	}
	//check if all those cells were taken
	//respond with a different move if so
	if(gamesPlayed == 0){
		connectingMove(board, playerLastMoveI, playerLastMoveJ, new Graph(edges));
		return;
	}
	else{
		compMoveI = winningI;
		compMoveJ = winningJ;
	}
}

//connecting moves are the 1 of the 2 hexes that connect the advancing moves
//they are inbetween each advancing move in the stack
inline void HexAI::connectingMove(vector<intvec> board, int playerLastMoveI, int playerLastMoveJ, Graph* edges){
	//i decreases by 2 for j increases of 1
	//i increases by 2 for j decreases of 1
	int iOffset = 1;//this starts at 1, because connecting moves start 1 off of center
	int jOffset = 0;
	
	//this is used to make sure that sample games were actually run
	int gamesPlayed = 0;
	
	//testst start hurr
	gamesWon = 0;
	gamesLost = 0;
	//holds values for determining next move
	int wins = 0;
	int winningI = -1;
	int winningJ = -1;
	//tries to favor going down the left connecting moves, making
	//the right (direction, not correct) one if left is taken.
	//moves also come through this section if advancing move failed to
	//find an acceptable advancing move, so this one throws up its hands and
	//calls random if it was unable to find one.
	while(middleIActuallyTaken + iOffset <  size and
	          middleIActuallyTaken - iOffset >= 0    and
		      middleJActuallyTaken + jOffset <  size and
		      middleJActuallyTaken - jOffset >= 0)
	{
		if(board[middleIActuallyTaken - iOffset][middleJActuallyTaken + jOffset] == 0){
			gamesPlayed++;
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
				recursiveGame(board, middleIActuallyTaken - iOffset, middleJActuallyTaken + jOffset, p1, new Graph(edges), 0);
			}
			//if this is statistically better
			if(wins < gamesWon){
cout << "Debug 2 ?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken - iOffset;//+ iOffset;
				winningJ = middleJActuallyTaken + jOffset;//- jOffset;
				//reset for next pass
				gamesWon = 0;
			}
		}//the left hand option seems to be taken, so try the right
		else if(board[middleIActuallyTaken - iOffset][middleJActuallyTaken + jOffset + 1] == 0){
			gamesPlayed++;
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
				recursiveGame(board, middleIActuallyTaken - iOffset, middleJActuallyTaken + jOffset + 1, p1, new Graph(edges), 0);
			}
			//if this is statistically better
			if(wins < gamesWon){
cout << "Debug 2 ?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken - iOffset;//+ iOffset;
				winningJ = middleJActuallyTaken + jOffset;//- jOffset;
				//reset for next pass
				gamesWon = 0;
			}
		}
		
		
		if(board[middleIActuallyTaken + iOffset][middleJActuallyTaken - jOffset] == 0){
			gamesPlayed++;
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
				recursiveGame(board, middleIActuallyTaken - iOffset, middleJActuallyTaken + jOffset, p1, new Graph(edges), 0);
			}
			//if this is statistically better
			if(wins < gamesWon){
cout << "Debug 2 ?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken - iOffset;//+ iOffset;
				winningJ = middleJActuallyTaken + jOffset;//- jOffset;
				//reset for next pass
				gamesWon = 0;
			}
		}else if(board[middleIActuallyTaken + iOffset][middleJActuallyTaken - jOffset + 1] == 0){
			gamesPlayed++;
			for(int i = 0; i < sampleGamesRunPerCheck; i++){
				recursiveGame(board, middleIActuallyTaken - iOffset, middleJActuallyTaken + jOffset + 1, p1, new Graph(edges), 0);
			}
			//if this is statistically better
			if(wins < gamesWon){
cout << "Debug 2 ?" << endl;
				//gamesWon = wins;
				wins = gamesWon;
				winningI = middleIActuallyTaken - iOffset;//+ iOffset;
				winningJ = middleJActuallyTaken + jOffset;//- jOffset;
				//reset for next pass
				gamesWon = 0;
			}
		}
		iOffset += 2;
		jOffset += 1;
	}
	//if all those options were taken
	if(gamesPlayed == 0){
		int* x = RandomMove(board);
		compMoveI = x[0];
		compMoveJ = x[1];
		delete x;
	}else{
		compMoveI = winningI;
		compMoveJ = winningJ;
	}
}

//top level access to the recursive algorithm that runs board simulations
//this has this data available:
//the player's last turn is in playerMove[I/J]
//the comp's last move is still in compMove[I/J]
//also, board is always a copy, so you can modify
//it without regret! :D
inline void HexAI::MonteCarlo(vector<intvec> board, int turn, bool pieRuleInvoked, int playerLastMoveI, int playerLastMoveJ, Graph* edges){
	//try to grab the middle square on the first turn
	if(not compMadeMove){
		compMoveI = compMoveJ = middle;
		middleIActuallyTaken = middle;
		middleJActuallyTaken = middle;
		compMadeMove = true;
		return;
	}else{
		//not first turn
		//decide on an 'advancing' move or a 'connecting' move,
		//or compensate for the player using the pie rule
		if(turn == 2 and pieRuleInvoked){//first see if it has to compensate for the pie rule being invoked
			//pick a random direction to move, 2 on either side of middle
			int x;
			if(rand() % 2){
				x = -2;
			}else{
				x = 2;
			}
			compMoveI = middle;
			compMoveJ = middle + x;
			middleIActuallyTaken = compMoveI;
			middleJActuallyTaken = compMoveJ;
			return;
		}else if( turn % 4 == 0){//connecting move
			connectingMove(board, playerLastMoveI, playerLastMoveJ, edges);
			return;
		}else{//advancing move, turn % 4 == 2
			advancingMove(board, playerLastMoveI, playerLastMoveJ, edges);
			return;
		}
	}
}

int* HexAI::getMove(vector<intvec> board, int turn, bool pieRuleInvoked, Graph* edges, int lastPlayerMoveI, int lastPlayerMoveJ){
	//this was version 1 of the AI, skip it for version 2
	//AI(board);
	
	//reset counters in prep for new decisions
	gamesWon = gamesLost = 0;
	vector<intvec> tempBoard = board;
	
	MonteCarlo(tempBoard, turn, pieRuleInvoked, lastPlayerMoveI, lastPlayerMoveJ, edges);
	int* ij = new int[2];
	ij[0] = compMoveI;
	ij[1] = compMoveJ;
	//check for bad moves that may come out of AI
	if(board[ij[0]][ij[1]] != 0){
		delete ij;
		ij = RandomMove(board);
	}
	return ij;
}


/*
How this is intended to work:
It starts by grabbing the middle square.  It shoots straight up and down,
skipping rows as it does so.  This way it tries to grab all the hexes that
are connected vertically to another hex via the line segment between the
two hexes in the rows inbetween (i.e. 1 hex inbetween).  It then has 2 options
for filling in and connecting these hexes.  MonteCarlo stuff is used to determine
which potential move in each of these two categories will be better.  If the
pie rule is invoked, it moves over 1 and keeps trying.  Using this as a basic
strategy before recursing drastically cuts down on computational time, as it
is only checking how good different logical moves are, rather than many illogical
moves

*/