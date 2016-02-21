#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
///////////////////////////////////////////////////////////////////////////////

class base :public GraphObject{
public:
    base(StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(1);
        alive=1;
        m_world=w;
    }
    //Destructor.
    virtual ~base(){setVisible(0);}
    
    //Pure virtual because it would never be called on base.
    virtual void doSomething()=0;
    virtual void annoy()=0;
    
    //Modifier
    void setAlive(bool t){alive = t;}
    
    //Accessor Functions
    bool isAlive(){return alive;}
    StudentWorld* getWorld(){return m_world;}
private:
    bool alive;
    StudentWorld* m_world;
};

//////////////////////////////////////////////////////////////////////////////////


class Dirt :public base{
public:
    Dirt(StudentWorld* w, int imageID, int startX, int startY):base(w,imageID, startX, startY, right, .25, 3){
    }
    Dirt(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth):base(w,imageID, startX, startY, dir, size, depth){
    }
    virtual ~Dirt(){}
    virtual void doSomething(){}//Dirt does nothing
    virtual void annoy(){}
};

//////////////////////////////////////////////////////////////////////////////////

class FrackMan :public base{
public:
    FrackMan(StudentWorld* w):base(w, IID_PLAYER, 30, 60, right, 1.0, 0){
        hitPoints=10;
        water=5;
    }
    ~FrackMan(){
    
    }
    virtual void doSomething();
    virtual void annoy();
    int getHitPoints(){return hitPoints;}
    unsigned int getWater(){return water;}
    void addWater(){water++;}
    void reduceWater(){water--;}
    //SEE
private:
    int hitPoints;
    int water;
    //TODO: ADD WATER, SONAR CHARGE, GOLD NUGGETS,
};

//////////////////////////////////////////////////////////////////////////////////

class Boulder :public Dirt{
public:
    Boulder(StudentWorld*w, int i, int x, int y):Dirt(w, IID_BOULDER, x, y, down, 1.0, 1){
        state = 1;
        ticks_elapsed=0;
    }
    ~Boulder(){};
    virtual void doSomething();
    void changeState(int n){state=n;}
    int getState(){return state;}
private:
    //state=1 => stable. state=0 => waiting state=-1 => falling
    int state;
    int ticks_elapsed;
};

//////////////////////////////////////////////////////////////////////////////////

class Squirt: public base{
public:
    Squirt(StudentWorld*w, int i, int x, int y, Direction dir, double size=1.0, unsigned int depth =1 ):base(w, IID_WATER_SPURT, x, y, dir, size, depth){
        steps=0;
        
    }
    ~Squirt(){}
    virtual void doSomething();
    virtual void annoy(){};
   
private:
    int steps;
};
#endif // ACTOR_H_
