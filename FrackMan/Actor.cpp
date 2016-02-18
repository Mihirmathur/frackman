#include "Actor.h"
#include "StudentWorld.h"

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
    }
}

void FrackMan::annoy(){
    
}

//////////////////////////////////////////////////////////////////////////////////

void Boulder::doSomething(){
    if (!isAlive())
        return;
    int x=getX();
    int y=getY();
    if (getState()==-1) {
        moveTo(x, y-3);
    }
    if(getState()==0){
        ticks_elapsed++;
        if (ticks_elapsed==30)changeState(-1);
    }
    if(getState()==1){
        
        StudentWorld* w=getWorld();
        if (w->checkDirt(x, y)==1);
        else{
            changeState(0);
        }
    }
    
}