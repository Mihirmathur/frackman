#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class base :public GraphObject{
public:
    base(StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(1);
        alive=1;
        m_world=w;
    }
    //Destructor/
    virtual ~base(){setVisible(0);}
    virtual void doSomething()=0;//Pure virtual because it would never be called on base.
   
    //Modifier
    void setAlive(bool t){alive = t;}
    
    //Accessor Functions
    bool isAlive(){return alive;}
    StudentWorld* getWorld(){return m_world;}
private:
    bool alive;
    StudentWorld* m_world;
};

class Dirt :public base{
public:
    Dirt(StudentWorld* w, int imageID, int startX, int startY):base(w,IID_DIRT, startX, startY, right, .25, 3){
    }
    virtual void doSomething(){}//Dirt does nothing
};

class FrackMan :public base{
public:
    FrackMan(StudentWorld* w):base(w, IID_PLAYER, 30, 60, right, 1.0, 0){hitPoints=10;}
    virtual void doSomething();
    int getHitPoints(){return hitPoints;}
    //SEE
private:
    int hitPoints;
    //TODO: ADD WATER, SONAR CHARGE, GOLD NUGGETS,
};
#endif // ACTOR_H_
