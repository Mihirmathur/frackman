#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
//FRACKMAN

void FrackMan::doSomething(){
    if (!isAlive()) 
        return;
    if(getHitPoints()<=0){
        setAlive(0);
        //TODO: Dying Sound
    }
    Direction d;
    int key;
    StudentWorld* w=getWorld();
    int x=getX();
    int y=getY();
    
    w->remDirt(x, y);
    
    if (w->getKey(key)==true) {
    d=getDirection();
    if (key== KEY_PRESS_LEFT) {
        if (d!=left) setDirection(left);
        //TODO: Condition if boulder.
        else {
            if(x>0)moveTo(x-1,y);
        }
    }
    if (key== KEY_PRESS_RIGHT) {
        if (d!=right) setDirection(right);
        //TODO: Condition if boulder.
        else {
            if(x<60)moveTo(x+1,y);
        }
    }
    if (key== KEY_PRESS_UP) {
        if (d!=up) setDirection(up);
        //TODO: Condition if boulder.
        else {
            if(y<60)moveTo(x,y+1);
        }
    }
    if (key== KEY_PRESS_DOWN) {
        if (d!=down) setDirection(down);
        //TODO: Condition if boulder.
        else {
            if(y>0)moveTo(x,y-1);
        }
    }
    if (key==KEY_PRESS_SPACE) {
        if (getWaterCount()>0) {
            Direction dir=getDirection();
            w->createSquirt(x, y, dir);
            reduceWater();
        }
        
        }
    }
}

void FrackMan::annoy(int amt){
    
}
//////////////////////////////////////////////////////////////////////////////////
void Protestor::doSomething(){
    
}


//////////////////////////////////////////////////////////////////////////////////
//BOULDER
void Boulder::doSomething(){
    if (!isAlive())
        return;
    int x=getX();
    int y=getY();
    StudentWorld* w=getWorld();
    if (getState()==-2) {
        setAlive(false);
    }
    //State -1: falling
    if (getState()==-1) {
        while(y>=0 && w->checkDirt(x,y)==0){
            moveTo(x, y-1);
            y--;
        }
        //Add annoy feature.
        changeState(-2);
    }
    
    //State 0: Waiting
    if(getState()==0){
        ticks_elapsed++;
        if (ticks_elapsed==30)changeState(-1);
    }
    
    //State 1: Stable
    if(getState()==1){
        if (w->checkDirt(x, y)==1);
        else{
            changeState(0);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////
//SQUIRT

void Squirt::doSomething(){
    if (!isAlive()) {
        return;
    }
    
    if(steps==3)setAlive(false);
    int x=getX();
    int y=getY();
    StudentWorld* w=getWorld();
    Direction dir = getDirection();
    
    //Add protestor annoying.
    
    switch (dir) {
        case up:
            if (!w->isDirtOrBoulder(x, y+1)) {
                moveTo(x, y+1);
                steps++;
            }
            else{
                setAlive(false);
            }
            break;
        case down:
            if (!w->isDirtOrBoulder(x, y-1)) {
                moveTo(x, y-1);
                steps++;
            }
            else{
                setAlive(false);
            }
            break;
        case left:
            if (!w->isDirtOrBoulder(x-1, y)) {
                moveTo(x-1, y);
                steps++;
            }
            else{
                setAlive(false);
            }
            break;
        case right:
            if (!w->isDirtOrBoulder(x+1, y)) {
                moveTo(x+1, y);
                steps++;
            }
            else{
                setAlive(false);
            }
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////
//ACTIVATING OBJECT

void ActivatingObject::setTicksToLive(){
    
}

//////////////////////////////////////////////////////////////////////////////////
//OILBARREL
void OilBarrel::doSomething(){
    if (!isAlive()) {
        return;
    }
    StudentWorld*w=getWorld();
    base* f= w->findNearbyFrackMan(this, 4.0);
    if(!isVisible() && f!=nullptr){
        setVisible(1);
        return;
    }
    f= w->findNearbyFrackMan(this, 3.0);
    if(f!=nullptr){
        w->increaseScore(1000);
        w->increaseBarrelsCollected();
        setAlive(0);
    }
    
}


//////////////////////////////////////////////////////////////////////////////////
//GOLD NUGGET
void GoldNugget::doSomething(){
    if (!isAlive())return;
    
    StudentWorld*w=getWorld();
    FrackMan* f= w->findNearbyFrackMan(this, 4.0);
    if(!isVisible() && f!=nullptr){
        setVisible(1);
        return;
    }
    if (canFrackmanPick()) {
        f= w->findNearbyFrackMan(this, 3.0);
        if(f!=nullptr){
            f->addGold();
            setAlive(0);
            w->increaseScore(10);
        }
        //PLAY SOUND
        
        
    }
    else if(canProtestorPick()){
        //ADD PROTESTOR CODE
    }
    //IF STATE=TEMP
}

//////////////////////////////////////////////////////////////////////////////////
//SONAR KIT
void SonarKit::doSomething(){
    if (!isAlive())return;
    
    StudentWorld*w=getWorld();
    int t= std::min(100, int(300 - 10*(w->getLevel())));
    if (ticks_elapsed == t) {
        setAlive(0);
        return;
    }
    FrackMan* f= w->findNearbyFrackMan(this, 3.0);
    if(f!=nullptr){
        setAlive(0);
        w->increaseScore(75);
        f->addSonar();
        //PlaySound
    }
    ticks_elapsed++;
    
}

//////////////////////////////////////////////////////////////////////////////////
//WATER POOL
void WaterPool::doSomething(){
    if (!isAlive())return;
    
    StudentWorld*w=getWorld();
    int t= std::min(100, int(300 - 10*(w->getLevel())));
    if (ticks_elapsed == t) {
        setAlive(0);
        return;
    }
    FrackMan* f= w->findNearbyFrackMan(this, 3.0);
    if(f!=nullptr){
        setAlive(0);
        w->increaseScore(100);
        f->addWater();
        //PlaySound
    }
    ticks_elapsed++;
}

