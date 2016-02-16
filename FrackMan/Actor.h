#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class base :public GraphObject{
public:
    base(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(1);
    }
    virtual ~base(){
        setVisible(0);
    }
    virtual void doSomething()=0; //Pure virtual because it would never be called on base.
};

class Dirt :public base{
    Dirt(int imageID, int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3):base(IID_DIRT, startX, startY, dir, size, depth){
    }
    virtual void doSomething(){}//Dirt does nothing
};

class FrackMan :public base{
public:
    FrackMan(int imageID, int startX=30, int startY=60, Direction dir = right, double size = 1, unsigned int depth = 0):base(IID_PLAYER, startX, startY, dir, size, depth){
        hitPoints=10;
        water=5;
        sonarCharge=1;
    }
    virtual void doSomething(){
        
    }
    
private:
    int hitPoints;
    int water;
    int sonarCharge;
    int goldNuggets;
};
#endif // ACTOR_H_
