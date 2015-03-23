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

struct player {
	int row;
	int col;
	int wallsLeft;
	
	player(int initialRow, int initialCol, int totalWalls) {
		row = initialRow;
		col = initialCol;
		wallsLeft = totalWalls;
	}
	
	player() {
		row = 9;
		col = 9;
		wallsLeft = 10;
	}
};

struct gameState {
	int n; // Length
	int m; // Breath
	int currentPlayer;
	player players[2];
    vector<gameState> children;
	vector<wall>  wallsPlacedSoFar; // (Orientation, (Row Centre, Col Centre))
	
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
float maxValue(gameState GD, float alpha, float beta);


//minimax stuff


vector<qMove> validMoves(gameState currentState) {
    vector<qMove> currentMoves;
    
    return currentMoves;
}





qMove alphaBetaSearch(gameState GD)
{
    float v=maxValue(GD,-infinity,infinity);
    vector<qMove> allMovesPossible=validMoves(GD);
    return allMovesPossible[0];
}

float maxValue(gameState GD, float alpha, float beta)
{
    if (GD.children.size()==0)
    {
        return evalFunction(GD);
    }
    float v=-infinity;
    
    vector<qMove> actions=validMoves(GD);
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


float minValue(gameState GD, float alpha, float beta)
{
    if (GD.children.size()==0)
    {
        return evalFunction(GD);
    }
    float v=infinity;
    vector<qMove> actions=validMoves(GD);
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


float evalFunction(gameState GD)
{
    return 4.2;
}



bool arePlayersAdjacent(gameState currentState) {
	bool potentiallyAdjacent = (abs(currentState.players[0].row - currentState.players[1].row) + abs(currentState.players[0].col - currentState.players[1].col) == 1);
	if (potentiallyAdjacent) {
		int orientationRequired = abs(currentState.players[0].col - currentState.players[1].col);
		for(std::vector<int>::size_type i = 0; i != currentState.wallsPlacedSoFar.size(); i++) {
			wall currentWall = currentState.wallsPlacedSoFar[i];
			if (currentWall.orientation == orientationRequired) {
				if (orientationRequired == 0) {
					int potentialRowCenter = (currentState.players[0].row + currentState.players[1].row + 1)/2;
					int colDifference = currentWall.colCenter - currentState.players[0].col;
					if ((currentWall.rowCenter == potentialRowCenter) && (colDifference >= 0) && (colDifference <=  1)) {
						return false;
					}
				}
				if (orientationRequired == 1) {
					int potentialColCenter = (currentState.players[0].col + currentState.players[1].col + 1)/2;
					int rowDifference = currentWall.rowCenter - currentState.players[0].row;
					if ((currentWall.colCenter == potentialColCenter) && (rowDifference >= 0) && (rowDifference <= 1)) {
						return false;
					}
				}
			}
		}
	}
	return potentiallyAdjacent;
}




gameState moveState(gameState currentState, qMove myMove) {
	gameState afterMoveState = currentState;
	if (myMove.type == 0) {
		afterMoveState.players[currentState.currentPlayer].row = myMove.row;
		afterMoveState.players[currentState.currentPlayer].col = myMove.col;
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
