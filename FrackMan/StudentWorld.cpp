#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include "GraphObject.h"
#include "GameController.h"
#include <sstream> 
#include <iostream>
#include <iomanip>
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

void StudentWorld::setGameStatText(){
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = m_frackman->getHitPoints()*10;
    int gold = m_frackman->getGoldCount();
    int squirts = m_frackman->getWaterCount();
    int sonar = m_frackman->getSonarCount();
    //TODO: CHANGE OIL
    int oil=0;
    ostringstream oss;  // oss is a name of our choosing.
    oss.setf(ios::fixed);
    // “Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2”
    //Add Sonar and Oil
    oss<<"Scr: "<<setw(8)<<setfill('0')<<score<<"  Lives "<<lives<<"  Lvl: "<<setw(2)<<setfill(' ')<<level<<"  Hlth: "<<setw(3)<<setfill(' ')<<health<<"%  Water:"<<setw(2)<<setfill(' ')<<squirts<<"  Gld: "<<setw(2)<<setfill(' ')<<gold<<"  Sonar: "<<setw(2)<<setfill(' ')<<sonar<<"  Oil Left"<<setw(2)<<setfill(' ')<<oil;
    text = oss.str();
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
    
    int B= min(int(getLevel()/2 +2), 6);
    int O=min(int(2+getLevel()),20);
    //int G= max(int(5-getLevel()/2),2);
    
    
    int x, y,t, i;
    int x2, y2;
    vector<base*>::iterator it;
    Boulder *b;
    OilBarrel *o;
    x=rand()%60;
    y=rand()%56;
    b=new Boulder(this, IID_BOULDER, x, y);
    remDirt(x, y);
    m_actor.push_back(b);
    for(i = 1; i<B; i++){
        do{
            t=0;
            x=rand()%60;
            y=rand()%56;
            for (it=m_actor.begin(); it!=m_actor.end(); it++ ) {
                x2=(*it)->getX();
                y2=(*it)->getY();
                if(distance(x, y, x2, y2)<6)break;
                else t=1;
            }
        }while (t!=1);
        b=new Boulder(this, IID_BOULDER, x, y);
        remDirt(x, y);
        m_actor.push_back(b);
    }
    for(i=0; i<O; i++){
        do{
            t=0;
            x=rand()%60;
            y=rand()%56;
            for (it=m_actor.begin(); it!=m_actor.end(); it++ ) {
                x2=(*it)->getX();
                y2=(*it)->getY();
                if(distance(x, y, x2, y2)<6)break;
                else t=1;
            }
        }while (t!=1);
        o=new OilBarrel(this, x, y);
        m_actor.push_back(o);
    }
    
    GoldNugget* g=new GoldNugget(this, 30, 15, SOUND_GOT_GOODIE, 0,1,0,0);
   
    m_actor.push_back(g);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    string t;
    StudentWorld::setGameStatText();
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
    for (i=m_actor.begin(); i!=m_actor.end();) {
        if(!(*i)->isAlive()){
            delete *i;
            i=m_actor.erase(i);
        }
        else i++;
    }
    
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
            if ((k>=0 && k<64) && (l>=0 && l<60) && m_dirt[k][l]!=nullptr) {
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

//Just works for dirt right now.
bool StudentWorld::isDirtOrBoulder(int x, int y){
    
    if ((x>=0 && x<64) && (y>=0 && y<60) && m_dirt[x][y]!=nullptr) return true;
    else return false;
}


void StudentWorld::createSquirt(int x, int y, GraphObject::Direction dir){
    Squirt*s;
    switch (dir) {
        case Boulder::up:
            s=new Squirt(this, IID_WATER_SPURT, x, y+4, dir);
            m_actor.push_back(s);
            break;
        case Boulder::down:
            s=new Squirt(this, IID_WATER_SPURT, x, y-4, dir);
            m_actor.push_back(s);
            break;
        case Boulder::left:
            s=new Squirt(this, IID_WATER_SPURT, x-4, y, dir);
            m_actor.push_back(s);
            break;
        case Boulder::right:
            s=new Squirt(this, IID_WATER_SPURT, x+4, y, dir);
            m_actor.push_back(s);
            break;
    }
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) const{
    return pow((pow(x2-x1, 2) + pow(y2-y1, 2)), .5);
}

FrackMan* StudentWorld::findNearbyFrackMan(base* a, double radius) const{
    int x1=a->getX();
    int y1=a->getY();
    int x2=m_frackman->getX();
    int y2=m_frackman->getY();
    //cerr<<"("<<x1<<", "<<y1<<") , ("<<x2<<", "<<y2<<") : ";
    double r=distance(x1, y1, x2, y2);
    //cerr<<r<<endl;
    if(r<=radius)return m_frackman;
    return nullptr;
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
