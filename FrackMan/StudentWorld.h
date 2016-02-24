#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class base;
class FrackMan;
class Dirt;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir){
         m_frackman=nullptr;
         barrels_collected=0;
         totalP=0;
         for (int i=0; i<64; i++) {
             for (int j=0; j<64; j++) {
                 grid[i][j]=0;
             }
         }
     }

    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void setGameStatText();
    void remDirt(int x, int y);
    
    
    bool checkDirt(int x, int y);
    bool isNotBoulder(int x, int y);
    bool isDirtOrBoulder(int x, int y);
    
    void createSquirt(int x, int y, GraphObject::Direction dir);
    FrackMan* findNearbyFrackMan(base* a, double radius) const;
    double distance(int x1, int y1, int x2, int y2) const;
    void setXandY(int &x, int &y);
    void updateBoulderPosition(int x, int y, int t);
    void discover(int x, int y);
   
    
    void increaseBarrelsCollected(){barrels_collected++;}
    int getBarrelsCollected(){return barrels_collected;}
private:
    int key_val;
    std::string text;
    std::vector<base*> m_actor;
    FrackMan* m_frackman;
    Dirt* m_dirt[64][64];
    int grid[64][64];
    int barrels_collected;
    int totalP;
};

#endif // STUDENTWORLD_H_
