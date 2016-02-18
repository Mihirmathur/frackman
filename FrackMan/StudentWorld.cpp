#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include "GameController.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir){
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld(){
    vector<base*>::iterator i;
    
    //Erase actors.
    for (i=m_actor.begin(); i!=m_actor.end();) {
        delete *i;
        i=m_actor.erase(i);
    }
    //Erase dirt elements.
    for (int k=0; k<64; k++) {
        for (int l=0; l<60; l++) {
            delete m_dirt[k][l];
        }
    }
    //Delete FrackMan
    delete m_frackman;
}

void StudentWorld::setGameStatText(std::string stat){
    GameWorld::setGameStatText(text);
}

int StudentWorld::init(){
    //Initializinf FrackMan
    m_frackman = new FrackMan(this);
    
    //Initializing dirt.
    for (int k=0; k<64; k++) {
        for (int l=0; l<60; l++) {
            //Shaft
            if((k>=30 && k<=33) && (l>=4 && l<=59)){}
            
            else m_dirt[k][l]=new Dirt(this,IID_DIRT, k,l);
        }
    }
    //Number of boulders, gold and oil
    /*
    int B= min(int(getLevel()/2 +2), 6);
    int G= max(int(5-getLevel()/2),2);
    int L=min(int(2+getLevel()),20);
    */
    //Boulder* b=new Boulder(this, IID_BOULDER, 10, 15);
    //m_actor.push_back(b);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    string t;
    setGameStatText(t);
    //TODO: Update Display text
    
    vector<base*>::iterator i;
    //FrackMan moves.
    m_frackman->doSomething();
    
    //TODO: ALL OIL COLLECTED
    //RETURN
    
    //Each actor moves.
    for (i=m_actor.begin(); i!=m_actor.end(); i++) {
        if ((*i)->isAlive()) {
            
        (*i)->doSomething();
        
        //If FrackMan dies
        if(!m_frackman->isAlive()){
            decLives();
            return GWSTATUS_PLAYER_DIED;
            }
        }
        
    }
    //TODO: Remove dead objects.
    
    if(!m_frackman->isAlive()){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    //TODO: If current level finished. Play sound. Return GWSTATUS_FINISHED_LEVEL. 
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    vector<base*>::iterator i;
    
    for (i=m_actor.begin(); i!=m_actor.end();) {
        delete *i;
        i=m_actor.erase(i);
    }
    for (int k=0; k<64; k++) {
        for (int l=0; l<60; l++) {
            delete m_dirt[k][l];
            m_dirt [k][l]=nullptr;
        }
    }
    delete m_frackman;
    
}

void StudentWorld::remDirt(int x , int y){
    for (int k=x; k<=x+3; k++) {
        for (int l=y; l<=y+3; l++) {
            if ((k>=0 && k<64) && (l>=0 && l<60)) {
                delete m_dirt[k][l];
                m_dirt[k][l]=nullptr;
            }
        }
    }
}

bool StudentWorld::checkDirt(int x, int y){
    if (m_dirt[x][y-1]!=nullptr)return 1;
    if (m_dirt[x+1][y-1]!=nullptr)return 1;
    if (m_dirt[x+2][y-1]!=nullptr)return 1;
    if (m_dirt[x+3][y-1]!=nullptr)return 1;
    return 0;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
