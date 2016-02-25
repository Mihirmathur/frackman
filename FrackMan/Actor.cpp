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
        if(key== KEY_PRESS_ESCAPE){
            setAlive(0);
        }
    if (key== KEY_PRESS_LEFT) {
        if (d!=left) setDirection(left);
        
        else {
            if(x>0 && w->isNotBoulder(x-1, y))moveTo(x-1,y);
        }
    }
    if (key== KEY_PRESS_RIGHT) {
        if (d!=right) setDirection(right);
        //TODO: Condition if boulder.
        else {
            if(x<60 && w->isNotBoulder(x+1, y))moveTo(x+1,y);
        }
    }
    if (key== KEY_PRESS_UP) {
        if (d!=up) setDirection(up);
        //TODO: Condition if boulder.
        else {
            if(y<60 && w->isNotBoulder(x, y+1))moveTo(x,y+1);
        }
    }
    if (key== KEY_PRESS_DOWN) {
        if (d!=down) setDirection(down);
        //TODO: Condition if boulder.
        else {
            if(y>0 && w->isNotBoulder(x, y-1))moveTo(x,y-1);
        }
    }
    if (key==KEY_PRESS_SPACE) {
        if (getWaterCount()>0) {
            Direction dir=getDirection();
            w->createSquirt(x, y, dir);
            reduceWater();
        }
        
    }
    if ((key=='z' || key=='Z') && getSonarCount()>0) {
        reduceSonar();
        w->discover(x, y);
    }
        if(key==KEY_PRESS_TAB && getGoldCount()>0){
            reduceGold();
            w->addGoldForProtestor(x,y);
        }
}
}

//////////////////////////////////////////////////////////////////////////////////
void Protestor::doSomething(){
    if (!isAlive())
        return;
    if (freeze>0) {
        freeze--;
        return;
    }
    StudentWorld* w=getWorld();
    ticksToWait = std::max(0, int(3-(w->getLevel()/4)));
    Direction dir;
    int x=getX();
    int y=getY();
        
    if (freeze==0) {
        FrackMan* f= w->findNearbyFrackMan(this, 4.0);
        if (f!=nullptr) {
            //TODO: Play Sound
            f->annoy(2);
            freeze=15;
            ticks_elapsed++;
            return;
        }
    }
   
    if(getSteps()<=0){
        moveInDir= rand()%(10) + 8;
        while (1) {
            int r=rand()%4;
            std::cerr<<r;
            switch (r) {
                case 0:
                    if (x>0 && w->isDirtOrBoulder(x-1, y)==0 && w->isDirtOrBoulder(x-1, y+1)!=1 && w->isDirtOrBoulder(x-1, y+2)!=1 && w->isDirtOrBoulder(x-1, y+3)!=1 && w->isDirtOrBoulder(x-1, y+4)!=1){
                        setDirection(left);
                        return;
                    }
                    
                case 1:
                    if(x<60 && w->isDirtOrBoulder(x+4, y)==0 && w->isDirtOrBoulder(x+4, y+1)!=1 && w->isDirtOrBoulder(x+4, y+2)!=1 && w->isDirtOrBoulder(x+4, y+3)!=1 && w->isDirtOrBoulder(x+4, y+4)!=1){
                        setDirection(right);
                        return;
                    }
                case 2:
                    if(y<60 && w->isDirtOrBoulder(x, y+4)==0 && w->isDirtOrBoulder(x+1, y+4)!=1 && w->isDirtOrBoulder(x+2, y+4)!=1 && w->isDirtOrBoulder(x+3, y+4)!=1 && w->isDirtOrBoulder(x+4, y+4)!=1){
                        setDirection(up);
                        return;
                    }
                case 3:
                    if (y>0 && w->isDirtOrBoulder(x, y-1)==0 && w->isDirtOrBoulder(x+1, y-1)!=1 && w->isDirtOrBoulder(x+2, y-1)!=1 && w->isDirtOrBoulder(x+3, y-1)!=1 && w->isDirtOrBoulder(x+4, y-1)!=1){
                        setDirection(down);
                        return;
                    }
            }
        }
        
    }
    //Regular move.
    if(ticksToWait==0 || (ticks_elapsed%ticksToWait)==0){
        x=getX();
        y=getY();
        ticksToWait = std::max(0, int(3-(w->getLevel()/4)));
        //If number of steps that can be taken is positive.
        if (getSteps()>0) {
            dir=getDirection();
            if (dir==left) {
                //If there is no dirt or boulder, protestor moves.
                if (w->isDirtOrBoulder(x-1, y)==0 && w->isDirtOrBoulder(x-1, y+1)!=1 && w->isDirtOrBoulder(x-1, y+2)!=1 && w->isDirtOrBoulder(x-1, y+3)!=1 && w->isDirtOrBoulder(x-1, y+4)!=1) {
                    moveTo(x-1, y);
                    reduceSteps();
                }
                else {
                    moveInDir=0;
                    ticks_elapsed++;
                    return;
                    
                }
            }
            if (dir==right) {
                if (w->isDirtOrBoulder(x+4, y)==0 && w->isDirtOrBoulder(x+4, y+1)!=1 && w->isDirtOrBoulder(x+4, y+2)!=1 && w->isDirtOrBoulder(x+4, y+3)!=1 && w->isDirtOrBoulder(x+4, y+4)!=1) {
                moveTo(x+1, y);
                reduceSteps();
                }
                else {
                    moveInDir=0;
                    ticks_elapsed++;
                    return;
                }
            }
            if(dir==up){
                if (w->isDirtOrBoulder(x, y+4)==0 && w->isDirtOrBoulder(x+1, y+4)!=1 && w->isDirtOrBoulder(x+2, y+4)!=1 && w->isDirtOrBoulder(x+3, y+4)!=1 && w->isDirtOrBoulder(x+4, y+4)!=1) {
                moveTo(x, y+1);
                reduceSteps();
                }
                else {
                    moveInDir=0;
                    ticks_elapsed++;
                    return;
                }
            }
            if (dir==down && (y>0)) {
                if (w->isDirtOrBoulder(x, y-1)==0 && w->isDirtOrBoulder(x+1, y-1)!=1 && w->isDirtOrBoulder(x+2, y-1)!=1 && w->isDirtOrBoulder(x+3, y-1)!=1 && w->isDirtOrBoulder(x+4, y-1)!=1) {
                moveTo(x, y-1);
                reduceSteps();
                }
                else {
                    moveInDir=0;
                    ticks_elapsed++;
                    return;
                }
            }
        }
        else{
           
            
        }
        
    }
    //Resting move
    if(getState()==0){
        ticks_elapsed++;
        return;
    }
    
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
        if (ticks_elapsed==30){
         w->updateBoulderPosition(x, y, 0);   
        changeState(-1);
        }
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
            else
                setAlive(false);
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
            else
                setAlive(false);
            break;
        case right:
            if (!w->isDirtOrBoulder(x+1, y)) {
                moveTo(x+1, y);
                steps++;
            }
            else
                setAlive(false);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////
//ACTIVATING OBJECT



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
        base* p=w->findNearbyProtestor(this, 4.0);
        
        if(p!=nullptr){
            //TODO: Play sound
            Protestor *pr=dynamic_cast<Protestor*>(p);
            w->increaseScore(25);
            pr->setState(1);
            setAlive(0);
            return;
        }
    }
    if(getState()==1){
        if (ticks_elapsed<getTicksToLive()) {
            ticks_elapsed++;
        }
        else setAlive(0);
    }
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

