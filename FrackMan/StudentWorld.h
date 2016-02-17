#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
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
     }

    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setGameStatText(std::string stat);
    void remDirt(int x, int y);
private:
    int key_val;
    std::string text;
    std::vector<base*> m_actor;
    FrackMan* m_frackman;
    Dirt* m_dirt[64][64];
};

#endif // STUDENTWORLD_H_
