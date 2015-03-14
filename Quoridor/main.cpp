//
//  main.cpp
//  Quoridor
//
//  Created by Robin Malhotra on 13/03/15.
//  Copyright (c) 2015 BatmanAndRobin. All rights reserved.
//

#include <iostream>
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
    
    gameData(int l, int m, int k)
    {
        graph=NULL;
        length=l;
        breadth=m;
        numberOfWalls=k;
        this->p1=player(l/2, 0, numberOfWalls);
        this->p2 =player(l/2, m-1,numberOfWalls);
    }
    gameData()
    {
        graph=NULL;
        length=0;
        breadth=0;
        numberOfWalls=0;
        p1=player(length/2, 0,numberOfWalls);
        p2=player(length/2, breadth-1,numberOfWalls);
    }
};
gameData GD=gameData(100,100,10);

int encode(int x, int y) {
    int maxLengthBreadth = max(GD.length, GD.breadth);
    bool isMaxLength = (maxLengthBreadth == GD.length);
    if (isMaxLength) {
        return x*GD.length + y;
    }
    else {
        return x + y*GD.breadth;
    }
}

pair<int, int> decode(int encodedPair) {
    int maxLengthBreadth = max(GD.length, GD.breadth);
    bool isMaxLength = (maxLengthBreadth == GD.length);
    if (isMaxLength) {
        return make_pair(encodedPair/GD.length, encodedPair % GD.length);
    }
    else {
        return make_pair(encodedPair % GD.breadth, encodedPair/GD.breadth);
    }
}

int main(int argc, const char * argv[])
{
    // insert code here...
    GD.graph=new bool*[GD.length*GD.breadth+1];
    for(int i=0;i<=GD.length*GD.breadth;i++)
    {
        GD.graph[i]=new bool[GD.length*GD.breadth+1];
    }
    memset(GD.graph,GD.length*GD.breadth*GD.length*GD.breadth,true);
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i != decode(encode(i, j)).first || j!= decode(encode(i,j)).second)
            {
                cout<<"Well, shit";
            }
            cout << encode(i, j) << endl;
        }
    }
    
    return 0;
}
