#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <iostream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class base;
class FrackMan;
class Dirt;
class Protestor;
class Direction;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir){
         m_frackman=nullptr;
         ticks_elapsed=0;
         barrels_collected=0;
         for (int i=0; i<64; i++) {
             for (int j=0; j<61; j++) {
                 grid[i][j]=1000;
             }
         }
         for (int i=0; i<64; i++) {
             for (int j=0; j<61; j++) {
                 maze[i][j]='.';
             }
         }
     }

    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void setGameStatText();
    
    //Modifiers:
    void remDirt(int x, int y);
    void createSquirt(int x, int y, GraphObject::Direction dir);
    void addGoldForProtestor(int x, int y);
    void increaseBarrelsCollected(){barrels_collected++;}
    void reduceProtestor(){totalP--;}
    void MarkMaze(int x, int y, char d);
    
    //Accessors:
    double getGrid(int x, int y);
    int getBarrelsCollected(){return barrels_collected;}
    bool checkDirt(int x, int y);
    int isDirtOrBoulder(int x, int y);
    bool NotBoulder(int x, int y);
    FrackMan* findNearbyFrackMan(base* a, double radius) const;
    base* findNearbyProtestor(base* a, double radius);
    
    //Helper Functions
    double distance(int x1, int y1, int x2, int y2) const;
    void setXandY(int &x, int &y);
    void discover(int x, int y);
    
private:
    int key_val;
    std::string text;
    std::vector<base*> m_actor;
    FrackMan* m_frackman;
    Dirt* m_dirt[64][64];
    double grid[64][61];
    char maze[64][61];
    int barrels_collected;
    int totalP;
    int ticks_elapsed;
};

#endif // STUDENTWORLD_H_
