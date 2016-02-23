#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
///////////////////////////////////////////////////////////////////////////////

class base :public GraphObject{
public:
    base(StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth) {
        //setVisible(1);
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
        setVisible(1);
    }
    Dirt(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth):base(w,imageID, startX, startY, dir, size, depth){
        setVisible(1);
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
        gold=0;
        sonar=0;
        setVisible(1);
    }
    ~FrackMan(){
    
    }
    virtual void doSomething();
    virtual void annoy();
    
    //Accessors
    unsigned int getHitPoints(){return hitPoints;}
    unsigned int getWaterCount(){return water;}
    unsigned int getGoldCount(){return gold;}
    unsigned int getSonarCount(){return sonar;}
    
    //Modifiers
    void addWater(){water++;}
    void reduceWater(){water--;}
    void addGold(){gold++;}
    void reduceGold(){gold--;}
    void addSonar(){sonar++;}
    void reduceSonar(){sonar--;}
    //SEE
private:
    int hitPoints;
    int water;
    int gold;
    int sonar;
    //TODO: ADD WATER, SONAR CHARGE, GOLD NUGGETS,
};

//////////////////////////////////////////////////////////////////////////////////

class Boulder :public Dirt{
public:
    Boulder(StudentWorld*w, int i, int x, int y):Dirt(w, IID_BOULDER, x, y, down, 1.0, 1){
        state = 1;
        ticks_elapsed=0;
        setVisible(1);
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
        setVisible(1);
        
    }
    ~Squirt(){}
    virtual void doSomething();
    virtual void annoy(){};
   
private:
    int steps;
};

//////////////////////////////////////////////////////////////////////////////////

class ActivatingObject : public base{
public:
    ActivatingObject(StudentWorld* world, int x, int y, int imageID,
                     int soundToPlay, bool init, bool frack_pick, bool protest_pick):base(world, imageID, x, y, right, 1.0, 2){
        setVisible(init);
        frack_pickable=frack_pick;
        protest_pickable=protest_pick;
    }
    
    // Set number of ticks until this object dies
    void setTicksToLive();
    bool canFrackmanPick(){return frack_pickable;}
    bool canProtestorPick(){return protest_pickable;}
private:
    bool frack_pickable;
    bool protest_pickable;
};

//////////////////////////////////////////////////////////////////////////////////

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int x, int y):ActivatingObject(world, x, y, IID_BARREL, SOUND_FOUND_OIL, 0, 1, 0){
    }
    virtual void doSomething();
    virtual void annoy(){};
};

//////////////////////////////////////////////////////////////////////////////////

class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int x, int y, int sound, bool init, bool frack_pick, bool protest_pick, bool temp):ActivatingObject(world, x, y, IID_GOLD, sound, init, frack_pick, protest_pick){state=temp;}
    virtual void doSomething();
    virtual void annoy(){};
    void setState(bool t){state = t;}
    bool getState(bool t){return state;}
private:
    bool state;
};

//////////////////////////////////////////////////////////////////////////////////

class SonarKit : public ActivatingObject{
public:
    SonarKit(StudentWorld* w, int x, int y): ActivatingObject(w, x, y, IID_SONAR, SOUND_GOT_GOODIE, 1, 1,0){}
    virtual void doSomething();
    virtual void annoy(){};
};
#endif // ACTOR_H_
