//
//  main.cpp
//  Quoridor
//
//  Created by Robin Malhotra on 13/03/15.
//  Copyright (c) 2015 BatmanAndRobin. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;
struct player
{
    int posX,posY; //reverse of encoding
    int wallsLeft;
    player(int iposx, int iposy, int k)//initial posx, initial posy
    {
        posX=iposx;
        posY=iposy;
        wallsLeft=k;
    }
    player()
    {
        posX=0;
        posY=0;
    }
};


struct gameData
{
    int length;
    int breadth;
    int numberOfWalls;
    bool **graph;
    player p1;
    player p2;
    vector<pair<int, pair<int, int>>>  wallsPlacedSoFar;
    
    gameData(int n, int m, int k)
    {
        graph=NULL;
        length=n;
        breadth=m;
        numberOfWalls=k;
        this->p1=player(0, m/2, numberOfWalls);
        this->p2 =player(n-1, m/2,numberOfWalls);
    }
    gameData()
    {
        graph=NULL;
        length=0;
        breadth=0;
        numberOfWalls=0;
        p1=player(0, 0, numberOfWalls);
        p2=player(0, 0, numberOfWalls);
    }
};
gameData GD=gameData(9,9,10);

int encode(int x, int y) {
    int base = GD.length;
    return x + y*base;
}

pair<int, int> decode(int encodedPair) {
    int base = GD.length;
    return make_pair(encodedPair % base, encodedPair/base);
}

bool isValidPoint(int x, int y) {
    if (x < 0) {
        return false;
    }
    if (y < 0) {
        return false;
    }
    if (x >= GD.length) {
        return false;
    }
    if (y >= GD.breadth) {
        return false;
    }
    return true;
}


void initializeGraph()
{
    GD.graph=new bool*[GD.length*GD.breadth+1];
    for(int i=0;i<=GD.length*GD.breadth;i++)
    {
        GD.graph[i]=new bool[GD.length*GD.breadth+1];
    }
    memset(GD.graph,GD.length*GD.breadth*GD.length*GD.breadth,false);
    for (int i = 0; i < GD.length; i++) {
        for (int j = 0; j < GD.breadth; j++) {
            int encodedPoint = encode(i,j);
            if (isValidPoint(i+1, j)) {
                GD.graph[encodedPoint][encode(i+1, j)] = true;
                GD.graph[encode(i+1, j)][encodedPoint] = true;
            }
            if (isValidPoint(i, j+1)) {
                GD.graph[encodedPoint][encode(i, j+1)] = true;
                GD.graph[encode(i, j+1)][encodedPoint] = true;
            }
            if (isValidPoint(i-1, j)) {
                GD.graph[encodedPoint][encode(i-1, j)] = true;
                GD.graph[encode(i-1, j)][encodedPoint] = true;
            }
            if (isValidPoint(i, j-1)) {
                GD.graph[encodedPoint][encode(i, j-1)] = true;
                GD.graph[encode(i, j-1)][encodedPoint] = true;
            }
        }
    }
}


bool arePlayersAdjacent()
{
    int p1Pos = encode(GD.p1.posX, GD.p1.posY);
    int p2Pos = encode(GD.p2.posX, GD.p2.posY);
    return GD.graph[p1Pos][p2Pos];
}

// Removes edges based on where the wall is positioned. (Center assumed here.)
void insertWall(int playerIndex, int orientation, int x, int y) {
    x--; y--;
    if (playerIndex == 1) {
        GD.p1.wallsLeft--;
    }
    else {
        GD.p2.wallsLeft--;
    }
    // Orientation 1 is for horizontal wall, 2 for vertical.
    if (orientation == 1) {
        GD.graph[encode(x-1, y-1)][encode(x-1, y)] = false;
        GD.graph[encode(x-1, y)][encode(x-1, y-1)] = false;
        GD.graph[encode(x, y-1)][encode(x, y)] = false;
        GD.graph[encode(x, y)][encode(x, y-1)] = false;
        
    }
    else if (orientation == 2) {
        GD.graph[encode(x-1, y-1)][encode(x, y-1)] = false;
        GD.graph[encode(x, y-1)][encode(x-1, y-1)] = false;
        GD.graph[encode(x-1, y)][encode(x, y)] = false;
        GD.graph[encode(x, y)][encode(x-1, y)] = false;
    }
    GD.wallsPlacedSoFar.push_back(make_pair(orientation, make_pair(x, y)));
    
}

// Check for path existence needed.
bool isValidWallMove(int orientation, int x, int y) {
    x--; y--;
    if (x < 1 || y < 1) {
        return false;
    }
    if (x >= GD.length || y >= GD.breadth) {
        return false;
    }
    for (int i = 0; i < GD.wallsPlacedSoFar.size(); i++) {
        int comparableOrientation = GD.wallsPlacedSoFar.at(i).first;
        int comparablePosX = GD.wallsPlacedSoFar.at(i).second.first;
        int comparablePosY = GD.wallsPlacedSoFar.at(i).second.second;
        if (comparablePosX == x && comparablePosY == y) {
            return false;
        }
        else if (comparableOrientation == orientation) {
            if (orientation == 1 && (comparablePosX == x)) {
                return (abs(comparablePosY - y) > 1);
            }
            else if (orientation == 2 && (comparablePosY == y)) {
                return (abs(comparablePosX - x) > 1);
            }
        }
        
    }
    return true;
}

bool isValidPlayerMove(int playerIndex, int x, int y) {
    player currentPlayer, otherPlayer;
    if (playerIndex == 1) {
        currentPlayer = GD.p1;
        otherPlayer = GD.p2;
    }
    else {
        currentPlayer = GD.p2;
        otherPlayer = GD.p1;
    }
    int myX = currentPlayer.posX;
    int myY = currentPlayer.posY;
    int otherX = currentPlayer.posX;
    int otherY = currentPlayer.posY;
    if (x == otherX && y == otherY) {
        return false;
    }
    bool adjacentPlayers = arePlayersAdjacent();
    if (!adjacentPlayers) {
        if (!isValidPoint(x, y) && !GD.graph[encode(x, y)][encode(myX, myY)]) {
            return false;
        }
    }
    else {
        if (GD.graph[encode(myX, myY)][encode(x, y)]) {
            return true;
        }
        else {
            if (isValidPoint(myX+(2*(otherX-myX)), myY+(2*(otherY-myY))) && GD.graph[encode(myX+(2*(otherX-myX)), myY+(2*(otherY-myY)))][encode(otherX, otherY)]) {
                if (x != (myX+(2*(otherX-myX))) || y != (myY+(2*(otherY-myY)))) {
                    return false;
                }
            }
            else {
                int potentialX = otherX + (otherY - myY);
                int potentialY = otherY + (otherX - myX);
                if(isValidPoint(potentialX, potentialY)) {
                    if (x == potentialX && y == potentialY) {
                        return true;
                    }
                    potentialX = otherX - (otherY - myY);
                    potentialY = otherY - (otherX - myX);
                    if (x == potentialX && y == potentialY) {
                        return true;
                    }
                }
            }
        }
    }
    return true;

}

    
int main(int argc, const char * argv[])
{
    // insert code here...
    initializeGraph();
    insertWall(1,1, 3, 3);
    cout << isValidWallMove(1, 5,4);
    
    return 0;
}
    
