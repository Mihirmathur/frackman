#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <queue>
//FRACKMAN

void FrackMan::doSomething(){
    if (!isAlive()) 
        return;
    StudentWorld* w=getWorld();
    if(getHitPoints()<=0){
        setAlive(0);
        w->playSound(SOUND_PLAYER_GIVE_UP);
    }
    Direction d;
    int key;
    
    int x=getX();
    int y=getY();
    w->remDirt(x, y, 1);
    
    if (w->getKey(key)==true) {
    d=getDirection();
        if(key== KEY_PRESS_ESCAPE){
            setAlive(0);
        }
    if (key== KEY_PRESS_LEFT) {
        if (d!=left) setDirection(left);
        else {
            if(x>0 && w->NotBoulder(x-1, y) ){
                moveTo(x-1,y);
            }
        }
    }
    if (key== KEY_PRESS_RIGHT) {
        if (d!=right) setDirection(right);
        else {
            if(x<60 && w->NotBoulder(x+1, y)){
                moveTo(x+1,y);
            }
        }
    }
    if (key== KEY_PRESS_UP) {
        if (d!=up) setDirection(up);
        else {
            if(y<60 && w->NotBoulder(x, y+1)){
                moveTo(x,y+1);
            }
        }
    }
    if (key== KEY_PRESS_DOWN) {
        if (d!=down) setDirection(down);
        else {
            if(y>0 && w->NotBoulder(x, y-1)){
                moveTo(x,y-1);
            }
        }
    }
    //For creating squirt.
    if (key==KEY_PRESS_SPACE && getWaterCount()>0) {
            Direction dir=getDirection();
            w->createSquirt(x, y, dir);
            w->playSound(SOUND_PLAYER_SQUIRT);
            reduceWater();
    }
    //For Sonar sound
    if ((key=='z' || key=='Z') && getSonarCount()>0) {
        reduceSonar();
        w->playSound(SOUND_SONAR);
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
    //Stops Protestor for freeze number of ticks.
    if (freeze>0) {
        freeze--;
        return;
    }
    StudentWorld* w=getWorld();
    ticksToWait = std::max(0, int(3-(w->getLevel()/4)));
    Direction dir;
    int x=getX();
    int y=getY();
    if(getState()==1 && x==60 && y==60){
        setAlive(0);
        w->reduceProtestor();
    }
    if (getHitPoints()<=0 && getState()!=1) {
        w->playSound(SOUND_PROTESTER_GIVE_UP);
        w->increaseScore(score);
        setState(1);
        return;
    }
    //If state=exit field, Set the direction to direction of
    //Minimum steps to get to exit.
    if (getState()==1) {
        
        Direction d=minDir();
        setDirection(d);
        if(d==right)moveTo(x+1, y);
        if(d==left)moveTo(x-1, y);
        if(d==up)moveTo(x, y+1);
        if(d==down)moveTo(x, y-1);
        ticks_elapsed++;
        return;
    }
    
    if (freeze==0) {
        FrackMan* f= w->findNearbyFrackMan(this, 4.0);
        if (f!=nullptr) {
            w->playSound(SOUND_PROTESTER_YELL);
            f->annoy(2);
            setFreeze(15);
            ticks_elapsed++;
            return;
        }
    }
    
    //If number of steps<=0 a random direction is assigned to protestor.
    if(getSteps()<=0){
        moveInDir= rand()%(50) + 8;
        while (1) {
            int r=rand()%4;
            switch (r) {
                case 0:
                    if (x>0 && w->isDirtOrBoulder(x-1, y)==0 && w->isDirtOrBoulder(x-1, y+1)!=1 && w->isDirtOrBoulder(x-1, y+2)!=1 && w->isDirtOrBoulder(x-1, y+3)!=1){
                        setDirection(left);
                        return;
                    }
                    
                case 1:
                    if(x<60 && w->isDirtOrBoulder(x+4, y)==0 && w->isDirtOrBoulder(x+4, y+1)!=1 && w->isDirtOrBoulder(x+4, y+2)!=1 && w->isDirtOrBoulder(x+4, y+3)!=1 ){
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
                //If there is no dirt or boulder, protestor moves and number of steps is reduced.
                if (w->isDirtOrBoulder(x-1, y)==0 && w->isDirtOrBoulder(x-1, y+1)!=1 && w->isDirtOrBoulder(x-1, y+2)!=1 && w->isDirtOrBoulder(x-1, y+3)!=1) {
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
                if (w->isDirtOrBoulder(x+4, y)==0 && w->isDirtOrBoulder(x+4, y+1)!=1 && w->isDirtOrBoulder(x+4, y+2)!=1 && w->isDirtOrBoulder(x+4, y+3)!=1) {
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

//Returns the direction of minimum distance to 60,60
GraphObject::Direction Protestor::minDir(){
    int x=getX();
    int y=getY();
    StudentWorld *w=getWorld();
    double distances[]={w->getGrid(x-1, y), w->getGrid(x+1, y), w->getGrid(x, y-1), w->getGrid(x, y+1)};
    std::sort(distances, distances + 4);
    if (distances[0]==w->getGrid(x+1, y))return right;
    if (distances[0]==w->getGrid(x-1, y))return left;
    if (distances[0]==w->getGrid(x, y-1))return down;
    else return up;
}

//////////////////////////////////////////////////////////////////////////////////




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
        w->playSound(SOUND_FALLING_ROCK);
        while(y>=0 && w->checkDirt(x,y)==0){
            moveTo(x, y-1);
            x=getX();
            y=getY();
            base *p = w->findNearbyProtestor(this, 3);
            if(p!=nullptr){
                Protestor *pr=dynamic_cast<Protestor*>(p);
                pr->annoy(100);
                w->increaseScore(500);
                pr->setState(1);
                pr->setFreeze(15);
                return;
            }
            FrackMan* f= w->findNearbyFrackMan(this, 4.0);
            if(f!=nullptr){
                f->annoy(100);
            }
        }
        changeState(-2);
    }
    
    //State 0: Waiting
    if(getState()==0){
        ticks_elapsed++;
        if (ticks_elapsed==30){
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
    base *p = w->findNearbyProtestor(this, 3);
    if(p!=nullptr){
        Protestor *pr=dynamic_cast<Protestor*>(p);
        pr->annoy(2);
        w->playSound(SOUND_PROTESTER_ANNOYED);
        int n = std::max(50, int(100- w->getLevel()*10));
        if(pr->getHitPoints()>0)pr->setFreeze(n);
        else pr->setFreeze(2);
        setAlive(0);
        return;
    }
    
    switch (dir) {
        case up:
            if (w->isDirtOrBoulder(x, y+1)==0) {
                moveTo(x, y+1);
                steps++;
            }
            else
                setAlive(false);
            break;
        case down:
            if (w->isDirtOrBoulder(x, y-1)==0) {
                moveTo(x, y-1);
                steps++;
            }
            else{
                setAlive(false);
            }
            break;
        case left:
            if (w->isDirtOrBoulder(x-1, y)==0) {
                moveTo(x-1, y);
                steps++;
            }
            else
                setAlive(false);
            break;
        case right:
            if (w->isDirtOrBoulder(x+1, y)==0) {
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
void ActivatingObject::doSomething(){
    if (!isAlive()) {
        return;
    }
    move();
}


//////////////////////////////////////////////////////////////////////////////////
//OILBARREL
void OilBarrel::move(){
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
        w->playSound(getSoundCode());
        setAlive(0);
    }
}

//////////////////////////////////////////////////////////////////////////////////
//GOLD NUGGET
void GoldNugget::move(){
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
            w->playSound(getSoundCode());
        }
    }
    else if(canProtestorPick()){
        base* p=w->findNearbyProtestor(this, 4.0);
        if(p!=nullptr){
            w->playSound(SOUND_PROTESTER_FOUND_GOLD);
            if (p->getID()==1) {
                Protestor *pr=dynamic_cast<Protestor*>(p);
                w->increaseScore(25);
                pr->setState(1);
            }
            else{
                HardcoreProtestor *hpr=dynamic_cast<HardcoreProtestor*>(p);
                w->increaseScore(50);
                hpr->setFreeze(std::max(50, int(100- w->getLevel()*10)));
            }
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
void SonarKit::move(){
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
        w->playSound(getSoundCode());
    }
    ticks_elapsed++;
}

//////////////////////////////////////////////////////////////////////////////////
//WATER POOL
void WaterPool::move(){
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
        w->playSound(getSoundCode());
    }
    ticks_elapsed++;
}

