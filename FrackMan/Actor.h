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
    virtual int getID(){return 0;}
    
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
};

//////////////////////////////////////////////////////////////////////////////////

class Living :public base{
public:
    Living(StudentWorld* w, int i, int hit=10, int x=30, int y=60, Direction dir=right):base(w, i, x, y, dir, 1, 0){hitPoints=hit;
        setVisible(1);
    }
    virtual ~Living(){}
    virtual void annoy(int amt){hitPoints-=amt;};
    virtual int getHitPoints(){return hitPoints;}
private:
    int hitPoints;
};

//////////////////////////////////////////////////////////////////////////////////
class FrackMan :public Living{
public:
    FrackMan(StudentWorld* w):Living(w, IID_PLAYER){
        water=5;
        gold=0;
        sonar=0;
    }
    ~FrackMan(){
    
    }
    virtual void doSomething();
    
    //Accessors
    unsigned int getWaterCount(){return water;}
    unsigned int getGoldCount(){return gold;}
    unsigned int getSonarCount(){return sonar;}
    
    //Modifiers
    void addWater(){water+=5;}
    void reduceWater(){water--;}
    void addGold(){gold++;}
    void reduceGold(){gold--;}
    void addSonar(){sonar++;}
    void reduceSonar(){sonar--;}
    //SEE
private:
    int water;
    int gold;
    int sonar;
};

//////////////////////////////////////////////////////////////////////////////////
class Protestor : public Living{
public:
    
    Protestor(StudentWorld*w, int i=IID_PROTESTER, int h=5):Living(w, i, h ,60, 60, left ){
        state =0;
        ticks_elapsed=0;
        moveInDir= rand()%(60-8 + 1) + 8;
        ticksToWait=0;
        freeze=0;
    }
    virtual ~Protestor(){
    }
    virtual int getID(){return 1;}
    virtual void doSomething();
    
    void setState(int n){state=n;}
    void setFreeze(int n){freeze=n;}
    void reduceSteps(){moveInDir--;}
    
    int getState(){return state;}
    int getSteps(){return moveInDir;}
private:
    //Rest State: 0, Leave Oil Field State: 1,
    int state;
    int ticks_elapsed;
    int moveInDir;
    int freeze;
    int ticksToWait;
};

//////////////////////////////////////////////////////////////////////////////////
class HardcoreProtestor: public Protestor{
    HardcoreProtestor(StudentWorld*w):Protestor(w, IID_HARD_CORE_PROTESTER, 20){}
    virtual ~HardcoreProtestor(){}
};

class Boulder :public Dirt{
public:
    Boulder(StudentWorld*w, int i, int x, int y):Dirt(w, IID_BOULDER, x, y, down, 1.0, 1){
        state = 1;
        ticks_elapsed=0;
        setVisible(1);
    }
    ~Boulder(){};
    virtual int getID(){return 2;}
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
        sound=soundToPlay;
    }
    virtual ~ActivatingObject(){}
    // Set number of ticks until this object dies
    void setTicksToLive(int n){ticks_tolive=n;}
    int getTicksToLive(){return ticks_tolive;}
    virtual int getSoundCode(){return sound;}
    bool canFrackmanPick(){return frack_pickable;}
    bool canProtestorPick(){return protest_pickable;}
    
private:
    bool frack_pickable;
    bool protest_pickable;
    int ticks_tolive;
    int sound;
};

//////////////////////////////////////////////////////////////////////////////////

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int x, int y):ActivatingObject(world, x, y, IID_BARREL, SOUND_FOUND_OIL, 0, 1, 0){
    }
    ~OilBarrel(){}
    virtual void doSomething();
};

//////////////////////////////////////////////////////////////////////////////////

class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int x, int y, int sound, bool init, bool frack_pick, bool protest_pick, bool temp):ActivatingObject(world, x, y, IID_GOLD, sound, init, frack_pick, protest_pick){state=temp;
        ticks_elapsed=0;
        setTicksToLive(100);
    }
    ~GoldNugget(){}
    virtual void doSomething();
    void setState(bool t){state = t;}
    bool getState(){return state;}
private:
    bool state;
    int ticks_elapsed;
};

//////////////////////////////////////////////////////////////////////////////////

class SonarKit : public ActivatingObject{
public:
    SonarKit(StudentWorld* w): ActivatingObject(w, 0, 60, IID_SONAR, SOUND_GOT_GOODIE, 1, 1,0){
        ticks_elapsed=0;
    }
    ~SonarKit(){}
    virtual void doSomething();
private:
    int ticks_elapsed;
};

//////////////////////////////////////////////////////////////////////////////////

class WaterPool : public ActivatingObject{
public:
    WaterPool(StudentWorld* w, int x, int y): ActivatingObject(w, x, y, IID_WATER_POOL, SOUND_GOT_GOODIE, 1, 1, 0){ticks_elapsed=0;}
    ~WaterPool(){}
    virtual void doSomething();
private:
    int ticks_elapsed;
};
#endif // ACTOR_H_
