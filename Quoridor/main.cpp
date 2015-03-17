//
//  main.cpp
//  quoridorPart2
//
//  Created by Robin Malhotra on 17/03/15.
//  Copyright (c) 2015 BatmanAndRobin. All rights reserved.
//

#include <iostream>
#include <vector>

#define pb push_back
#define mp make_pair

using namespace std;

struct qMove {
    int type;
    int row;
    int col;
    
    qMove(int p, int t, int r, int c) {
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
    vector<pair<int, pair<int, int>>>  wallsPlacedSoFar; // (Orientation, (Row Centre, Col Centre))
    
    gameState(int length, int breadth, int totalWalls)
    {
        currentPlayer = 0;
        n = length;
        m = breadth;
        this->players[0] = player(0, m/2, totalWalls);
        this->players[1] = player(n-1, m/2,totalWalls);
    }
    
};

bool arePlayersAdjacent(gameState currentState) {
    return (abs(currentState.players[0].row - currentState.players[1].row) + abs(currentState.players[0].col - currentState.players[1].col) == 1);
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
        afterMoveState.wallsPlacedSoFar.pb(mp(orientation, mp(rowCenter, colCenter)));
    }
    afterMoveState.currentPlayer = 1 - afterMoveState.currentPlayer;
    return afterMoveState;
}

vector<qMove> validMoves(gameState currentState) {
    
}



int main(int argc, const char * argv[]) {
    // insert code here...
    
    std::cout << "Hello, World!\n";
    
    return 0;
}
