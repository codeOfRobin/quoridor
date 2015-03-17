//
//  main.cpp
//  Quoridor
//
//  Created by Robin Malhotra on 13/03/15.
//  Copyright (c) 2015 BatmanAndRobin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stack>
#include <list>
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
//    bool **graph;
    player p1;
    player p2;
    vector<pair<int, pair<int, int>>>  wallsPlacedSoFar; // Orientation, pair of coordinates
    
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
    
    gameData *parent;
};

struct move {
    int type;
    int row;
    int column;
};

gameData theGame=gameData(9,9,10);




int encode(int x, int y) {
    int base = theGame.length;
    return x + y*base;
}

pair<int, int> decode(int encodedPair) {
    int base = theGame.length;
    return make_pair(encodedPair % base, encodedPair/base);
}

bool isValidPoint(int x, int y) {
    if (x < 0) {
        return false;
    }
    if (y < 0) {
        return false;
    }
    if (x >= theGame.length) {
        return false;
    }
    if (y >= theGame.breadth) {
        return false;
    }
    return true;
}

list<int> Edges(int i, gameData GD)
{
    list<int> edges;

    for (int j=0; j<(GD.length*GD.breadth); j++)
    {
        if (GD.graph[i][j])
        {
            edges.push_back(j);
        }
    }
    return edges;
}

//TODO:check for goal states here:
bool isGoal(int encodedPair, int player)
{
    pair<int, int> decodedPair = decode(encodedPair);
    int posY = decodedPair.second;
    return (posY == ((theGame.length-1)*(2-player)));
}

bool isThereAPath(int player, int x, int y, gameData GD)
{
    stack<int> S;
    S.push(encode(x, y));
    bool *discovered = new bool[GD.length*GD.breadth];
    memset(discovered,GD.length*GD.breadth,false);
    while (!S.empty())
    {
        int temp=S.top();
        if (isGoal(temp, player))
        {
            return true;
        }
        S.pop();
        if (discovered[temp]==false)
        {
            discovered[temp]=true;
            list<int> neighbours=Edges(temp, GD);
            std::list<int>::const_iterator iterator;
            for (iterator = neighbours.begin(); iterator != neighbours.end(); ++iterator) {
                std::cout << *iterator;
                S.push(*iterator);
            }
        }
        
    }
    return false;
}


void initializeGraph()
{
    theGame.graph=new bool*[theGame.length*theGame.breadth+1];
    for(int i=0;i<=theGame.length*theGame.breadth;i++)
    {
        theGame.graph[i]=new bool[theGame.length*theGame.breadth+1];
    }
    memset(theGame.graph,theGame.length*theGame.breadth*theGame.length*theGame.breadth,false);
    for (int i = 0; i < theGame.length; i++) {
        for (int j = 0; j < theGame.breadth; j++) {
            int encodedPoint = encode(i,j);
            if (isValidPoint(i+1, j)) {
                theGame.graph[encodedPoint][encode(i+1, j)] = true;
                theGame.graph[encode(i+1, j)][encodedPoint] = true;
            }
            if (isValidPoint(i, j+1)) {
                theGame.graph[encodedPoint][encode(i, j+1)] = true;
                theGame.graph[encode(i, j+1)][encodedPoint] = true;
            }
            if (isValidPoint(i-1, j)) {
                theGame.graph[encodedPoint][encode(i-1, j)] = true;
                theGame.graph[encode(i-1, j)][encodedPoint] = true;
            }
            if (isValidPoint(i, j-1)) {
                theGame.graph[encodedPoint][encode(i, j-1)] = true;
                theGame.graph[encode(i, j-1)][encodedPoint] = true;
            }
        }
    }
}


bool arePlayersAdjacent(gameData GD)
{
    int p1Pos = encode(GD.p1.posX, GD.p1.posY);
    int p2Pos = encode(GD.p2.posX, GD.p2.posY);
    return GD.graph[p1Pos][p2Pos];
}

// Removes edges based on where the wall is positioned. (Center assumed here.)
void insertWall(int playerIndex, int orientation, int x, int y, gameData GD) {
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
bool isValidWallMove(int orientation, int x, int y, gameData GD) {
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

bool isValidPlayerMove(int playerIndex, int x, int y, gameData GD) {
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
    bool adjacentPlayers = arePlayersAdjacent(GD);
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
//    insertWall(1,1, 3, 3);
//    cout << isValidWallMove(1, 5,4);
    cout << isGoal(encode(2, 6), 2);
    return 0;
}
    
