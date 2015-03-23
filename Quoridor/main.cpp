//
//  main.cpp
//  quoridorPart2
//
//  Created by Robin Malhotra on 17/03/15.
//  Copyright (c) 2015 BatmanAndRobin. All rights reserved.
//


//TODO:
/*
DFS-can player reach goal state
minimax
*/
#include <iostream>
#include <vector>

#define pb push_back
#define mp make_pair
#define infinity 999999
using namespace std;



struct wall {
	int orientation;
	int rowCenter;
	int colCenter;
	
	wall(int o, int r, int c) {
		orientation = o;
		rowCenter = r;
		colCenter = c;
	}
};

struct qMove {
	int type;
	int row;
	int col;
	
	qMove(int t, int r, int c) {
		type = t;
		row = r;
		col = c;
	}
};

struct position {
    int row;
    int col;
};

struct player {
    position pos;
	int wallsLeft;
	
	player(int initialRow, int initialCol, int totalWalls) {
		pos.row = initialRow;
		pos.col = initialCol;
		wallsLeft = totalWalls;
	}
	
	player() {
		pos.row = 9;
		pos.col = 9;
		wallsLeft = 10;
	}
};

struct gameState {
	int n; // Length
	int m; // Breath
	int currentPlayer;
	player players[2];
    vector<gameState> children;
	vector<wall>  wallsPlacedSoFar; // (Orientation, Row Centre, Col Centre) -> Orientation 0 for horizontal, 1 for vertical
	
	gameState(int length, int breadth, int totalWalls)
	{
		currentPlayer = 0;
		n = length;
		m = breadth;
		this->players[0] = player(0, m/2, totalWalls);
		this->players[1] = player(n-1, m/2,totalWalls);
	}
	
};


//function definitions
float evalFunction(gameState);
float maxValue(gameState gameData, float alpha, float beta);
bool arePositionsAdjacent(gameState currentState, position pos0, position pos1);
bool arePlayersAdjacent(gameState currentState);


//minimax stuff


vector<qMove> validMoves(gameState currentState) {
    vector<qMove> currentMoves;
    
    return currentMoves;
}

bool isValidPlayerMove(gameState currentState, qMove playerMove) {
    position currentPos = currentState.players[currentState.currentPlayer].pos;
	position otherPos = currentState.players[1-currentState.currentPlayer].pos;
    position movedPos;
    movedPos.row = playerMove.row;
    movedPos.col = playerMove.col;
	if (currentPos.row == otherPos.row && currentPos.col == currentPos.row) {
		return false;
	}
	if (arePositionsAdjacent(currentState, currentPos, movedPos)) {
		if (movedPos.row == otherPos.row && movedPos.col == otherPos.col) {
			if (otherPos.row == ((1-currentState.currentPlayer)*(currentState.n - 1) + 1)) {
				return true;
			}
			else {
				return false;
			}
		}
		return true;
	}
	if (arePlayersAdjacent(currentState)) {
		if (arePositionsAdjacent(currentState, otherPos, movedPos)) {
			position priorityPos;
			priorityPos.row = currentPos.row + 2*(otherPos.row - currentPos.row);
			priorityPos.col = currentPos.col + 2*(otherPos.col - currentPos.col);
			if (arePositionsAdjacent(currentState, otherPos, priorityPos)) {
				if (priorityPos.row == movedPos.row && priorityPos.col == movedPos.col) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return true;
			}
		}
	}
	return false;
}



qMove alphaBetaSearch(gameState gameData)
{
    float v=maxValue(gameData,-infinity,infinity);
    vector<qMove> allMovesPossible=validMoves(gameData);
    return allMovesPossible[0];
}

float maxValue(gameState gameData, float alpha, float beta)
{
    if (gameData.children.size()==0)
    {
        return evalFunction(gameData);
    }
    float v=-infinity;
    
    vector<qMove> actions=validMoves(gameData);
    for (int i=0; i<actions.size(); i++)
    {
        //        v=max(const _Tp &__a, <#const _Tp &__b#>, <#_Compare __comp#>)
        if (v>=beta)
        {
            return v;
        }
        
        alpha=max(alpha, v);
    }
    
    return v;
}


float minValue(gameState gameData, float alpha, float beta)
{
    if (gameData.children.size()==0)
    {
        return evalFunction(gameData);
    }
    float v=infinity;
    vector<qMove> actions=validMoves(gameData);
    for (int i=0; i<actions.size(); i++)
    {
        //        v=max(<#const _Tp &__a#>, <#const _Tp &__b#>, <#_Compare __comp#>)
        if (v<=alpha)
        {
            return v;
        }
        
        beta=min(beta,v);
    }
    
    return v;

}


//end minimax stuff


float evalFunction(gameState gameData)
{
    return 4.2;
}



bool arePositionsAdjacent(gameState currentState, position pos0, position pos1) {
	bool potentiallyAdjacent = (abs(pos0.row - pos1.row) + abs(pos0.col - pos1.col) == 1);
	if (potentiallyAdjacent) {
		int orientationRequired = abs(pos0.col - pos1.col);
		for(std::vector<int>::size_type i = 0; i != currentState.wallsPlacedSoFar.size(); i++) {
			wall currentWall = currentState.wallsPlacedSoFar[i];
			if (currentWall.orientation == orientationRequired) {
				if (orientationRequired == 0) {
					int potentialRowCenter = (pos0.row + pos1.row + 1)/2;
					int colDifference = currentWall.colCenter - pos0.col;
					if ((currentWall.rowCenter == potentialRowCenter) && (colDifference >= 0) && (colDifference <=  1)) {
						return false;
					}
				}
				if (orientationRequired == 1) {
					int potentialColCenter = (pos0.col + pos1.col + 1)/2;
					int rowDifference = currentWall.rowCenter - pos0.row;
					if ((currentWall.colCenter == potentialColCenter) && (rowDifference >= 0) && (rowDifference <= 1)) {
						return false;
					}
				}
			}
		}
	}
	return potentiallyAdjacent;
}

bool arePlayersAdjacent(gameState currentState) {
    position pos0 = currentState.players[0].pos;
    position pos1 = currentState.players[1].pos;
    return arePositionsAdjacent(currentState, pos0, pos1);
}


gameState moveState(gameState currentState, qMove myMove) {
	gameState afterMoveState = currentState;
	if (myMove.type == 0) {
		afterMoveState.players[currentState.currentPlayer].pos.row = myMove.row;
		afterMoveState.players[currentState.currentPlayer].pos.col = myMove.col;
	}
	else if (myMove.type == 1 || myMove.type == 2) {
		int orientation = myMove.type - 1;
		int rowCenter = myMove.row;
		int colCenter = myMove.col;
		wall movedWall(orientation, rowCenter, colCenter);
		afterMoveState.wallsPlacedSoFar.pb(movedWall);
		afterMoveState.players[currentState.currentPlayer].wallsLeft--;
	}
	afterMoveState.currentPlayer = 1 - afterMoveState.currentPlayer;
	return afterMoveState;
}




int main(int argc, const char * argv[]) {
	// insert code here...
	
	return 0;
}
