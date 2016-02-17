#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>

using namespace std;

GameWorld* createStudentWorld(string assetDir){
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld(){
    vector<base*>::iterator i;
    
    for (i=m_actor.begin(); i!=m_actor.end(); i++) {
        i=m_actor.erase(i);
    }
    for (int k=0; k<60; k++) {
        for (int l=0; l<60; l++) {
            delete m_dirt[k][l];
        }
    }
    delete m_frackman;
}

void StudentWorld::setGameStatText(std::string stat){
    GameWorld::setGameStatText(text);
}
bool StudentWorld::getKey(){
    return GameWorld::getKey(key_val);
}

int StudentWorld::init(){
    m_frackman = new FrackMan;
    
    //Initializing dirt.
    for (int k=0; k<60; k++) {
        for (int l=0; l<60; l++) {
            //Shaft
            if((k>=30 && k<=33) && (l>=4 && l<=59)){}
            else m_dirt[k][l]=new Dirt(IID_DIRT, k,l);
        }
    }
    
    int b= std::min(2, 6);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return 1;
}

void StudentWorld::cleanUp(){
    vector<base*>::iterator i;
    
    for (i=m_actor.begin(); i!=m_actor.end(); i++) {
        i=m_actor.erase(i);
    }
    for (int k=0; k<60; k++) {
        for (int l=0; l<60; l++) {
            delete m_dirt[k][l];
        }
    }
    delete m_frackman;
    
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
