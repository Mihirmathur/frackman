#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include "GraphObject.h"
#include "GameController.h"
#include <sstream> 
#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir){
	return new StudentWorld(assetDir);
}
//Destructor
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
            grid[k][l]=distance(60, 60, k, l);
            maze[k][l]='.';
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

    int oil=min(int(2+getLevel()),20)- getBarrelsCollected();
    ostringstream oss;
    oss.setf(ios::fixed);
    // “Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2”
    //Add Sonar and Oil
    oss<<"Scr: "<<setw(8)<<setfill('0')<<score<<"  Lives "<<lives<<"  Lvl: "<<setw(2)<<setfill(' ')<<level<<"  Hlth: "<<setw(3)<<setfill(' ')<<health<<"%  Water:"<<setw(2)<<setfill(' ')<<squirts<<"  Gld: "<<setw(2)<<setfill(' ')<<gold<<"  Sonar: "<<setw(2)<<setfill(' ')<<sonar<<"  Oil Left"<<setw(2)<<setfill(' ')<<oil;
    text = oss.str();
    GameWorld::setGameStatText(text);
}

//Init Function
int StudentWorld::init(){
    barrels_collected=0;
    totalP=0;
    //Initializing FrackMan
    m_frackman = new FrackMan(this);
    Protestor *p=new Protestor(this);
    totalP++;
    m_actor.push_back(p);
    
    //Initializing dirt.
    for (int k=0; k<64; k++) {
        for (int l=0; l<60; l++) {
            //Shaft
            if((k>=30 && k<=33) && (l>=4 && l<=59)){}
            else {
                m_dirt[k][l]=new Dirt(this,IID_DIRT, k,l);
                grid[k][l]=10000;
                maze[k][l]='#';
            }
        }
    }
    
    //Number of boulders, oil and gold
    int B= min(int(getLevel()/2 +2), 6);
    int O=min(int(2+getLevel()),20);
    int G= max(int(5-getLevel()/2),2);
    
    int x, y, i;
    vector<base*>::iterator it;
    Boulder *b;
    OilBarrel *o;
    GoldNugget* g;
    //Spawning Boulders
    for(i = 0; i<B; i++){
        setXandY(x,y);
        b=new Boulder(this, IID_BOULDER, x, y);
        remDirt(x, y);
        m_actor.push_back(b);
    }
    //Spawning OilBarrels
    for(i=0; i<O; i++){
        setXandY(x, y);
        o=new OilBarrel(this, x, y);
        m_actor.push_back(o);
    }
    //Spawning GoldNuggets
    for(i=0; i<G; i++){
        setXandY(x, y);
        g=new GoldNugget(this, x, y, SOUND_GOT_GOODIE, 0,1,0,0);
        m_actor.push_back(g);
    }
    //Adding SonarKit
    SonarKit*s=new SonarKit(this);
    m_actor.push_back(s);
   
    return GWSTATUS_CONTINUE_GAME;
}

//Move function
int StudentWorld::move(){
    string t;
    ticks_elapsed++;
    StudentWorld::setGameStatText();
    //Ticks elapsed for adding Protestor, SonarKit, Waterpool
    int T=max(25, int(200-getLevel()));
    int P=min(15, int(getLevel()*1.5 + 2));
    int G=getLevel()*25+300;
    
    //Randomly spawns SonarKit or WaterPool
    if(ticks_elapsed%G==0){
        //Probability of SonarKit is 1/5
        int r=rand()%5;
        if(r==4){
           SonarKit*s=new SonarKit(this);
            m_actor.push_back(s);
        }
        else{
            WaterPool*w;
            int x=rand()%60;
            int y=rand()%56;
            if (m_dirt[x][y]==nullptr) {
                    w=new WaterPool(this, x, y);
            }
            else w=new WaterPool(this, 30, y);
             m_actor.push_back(w);
        }
    }

    if(ticks_elapsed==200){
        for (int j=60; j>=0; j--) {
            for (int i=0; i<=60; i++) {
                std::cerr<<maze[i][j];
            }
            std::cerr<<endl;
        }
    }
//    if(ticks_elapsed==300){
//        for (int i=0; i<=60; i++) {
//            std::cerr<<"Distance from ("<<i<<", 0)"<<grid[i][0]<<"\n";
//            }
//    }
    int probOfHardCore=min(90, int(getLevel()*10  + 30))/10;
    //Spawning protestor
    if (ticks_elapsed%T==0 && totalP<P) {
        int rnum=rand()%10 + 1;
        if(rnum<=probOfHardCore){
            HardcoreProtestor *hp=new HardcoreProtestor(this);
            m_actor.push_back(hp);
            totalP++;
        }
        else{
        Protestor *p=new Protestor(this);
        totalP++;
        m_actor.push_back(p);
        }
    }
    
    for (int i=0; i<64; i++) {
        for (int j=0; j<61; j++) {
            if(grid[i][j]!=10000){
                grid[i][j]=distance(60, 60, i, j);
            }
        }
    }
    
    //If level complete.
    if(getBarrelsCollected()==min(int(2+getLevel()),20)){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //FrackMan moves.
    m_frackman->doSomething();
    
    vector<base*>::iterator i;
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
    //Remove dead objects
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
    
    return GWSTATUS_CONTINUE_GAME;
}

//Cleanup function for deleting dead objects.
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
            grid[k][l]=distance(60, 60, k, l);
            maze[k][l]='.';
        }
    }
    delete m_frackman;
}

//////////////////////////////////////////////////////////////////////////////////
//MODIFIERS

//Removes  dirt
void StudentWorld::remDirt(int x , int y){
    for (int k=x; k<=x+3; k++) {
        for (int l=y; l<=y+3; l++) {
            if ((k>=0 && k<64) && (l>=0 && l<60) && m_dirt[k][l]!=nullptr) {
                delete m_dirt[k][l];
                grid[k][l]=distance(60, 60, k, l);
                maze[k][l]='.';
                m_dirt[k][l]=nullptr;
                //playSound(SOUND_DIG);
            }
        }
    }
}

//Creates a new squirt.
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

//Creates new gold object that is pickable by protestor.
void StudentWorld::addGoldForProtestor(int x, int y){
    GoldNugget*g=new GoldNugget(this, x, y, SOUND_GOT_GOODIE, 1,0,1,1);
    m_actor.push_back(g);
}

//////////////////////////////////////////////////////////////////////////////////
//ACCESSORS
//Checks to see if there is dirt below x,y
bool StudentWorld::checkDirt(int x, int y){
    if (m_dirt[x][y-1]!=nullptr)return 1;
    if (m_dirt[x+1][y-1]!=nullptr)return 1;
    if (m_dirt[x+2][y-1]!=nullptr)return 1;
    if (m_dirt[x+3][y-1]!=nullptr)return 1;
    return 0;
}

//Checks to see if there is dirt or boulder at position (x,y)
int StudentWorld::isDirtOrBoulder(int x, int y){
    //Return -1 for invalid numbers. Return 1 for dirt. Return 0 otherwise.
    if (x<0 || x>64)return -1;
    if(y<0 || y>60)return -1;
    if (m_dirt[x][y]!=nullptr || !NotBoulder(x, y)) return 1;
    else return 0;
}

//Checks to see if there is a boulder at position x,y. Returns True if there is no boulder
bool StudentWorld::NotBoulder(int x, int y){
    vector<base*>::iterator i;
    int x2,y2;
    for (i=m_actor.begin(); i!=m_actor.end();i++) {
        if ((*i)->getID()==2) {
            x2=(*i)->getX();
            y2=(*i)->getY();
            if (distance(x, y, x2, y2) < 4) {
                return false;
            }
        }
    }
    return true;
}

//Returns pointer to Frackman if it is in radius
FrackMan* StudentWorld::findNearbyFrackMan(base* a, double radius) const{
    int x1=a->getX();
    int y1=a->getY();
    int x2=m_frackman->getX();
    int y2=m_frackman->getY();
    double r=distance(x1, y1, x2, y2);
    if(r<=radius)return m_frackman;
    return nullptr;
}

//Returns pointer to base if Protestor is in radius
base* StudentWorld::findNearbyProtestor(base* a, double radius){
    vector<base*>::iterator i;
    int x1=a->getX();
    int y1=a->getY();
    for (i=m_actor.begin(); i!=m_actor.end(); i++) {
        if((*i)->getID()==1 || (*i)->getID()==11){
            int x2=(*i)->getX();
            int y2=(*i)->getY();
            double r=distance(x1, y1, x2, y2);
            if(r<radius)return (*i);
        }
    }
    return nullptr;
}

//Gets distance from position to (60, 60)
double StudentWorld::getGrid(int x, int y){
    if((x>=0 && x<=60) && (y>=0 && y<=60)){
        return grid[x][y];
    }
    else return 10000;
}

//////////////////////////////////////////////////////////////////////////////////
//HLPER FUNCTIONS

//Calculate Euclidian distance between x1, y1, x2, y2.
double StudentWorld::distance(int x1, int y1, int x2, int y2) const{
    return pow((pow(x2-x1, 2) + pow(y2-y1, 2)), .5);
}

//Sets a random x and y value to actors.
void StudentWorld::setXandY(int &x, int &y){
    vector<base*>::iterator it;
    int t, x2, y2;
    do{
        t=0;
        x=rand()%60;
        y=rand()%55 + 1;
        for (it=m_actor.begin(); it!=m_actor.end(); it++ ) {
            x2=(*it)->getX();
            y2=(*it)->getY();
            if(distance(x, y, x2, y2)>6);
            else t=1;
        }
        if (t==0)return;
            
    }while (t!=0);
}


//Sets visible all actors in radius 12.
void StudentWorld::discover(int x, int y){
    vector<base*>::iterator i;
    int x2, y2;
    double d;
    for (i=m_actor.begin(); i!=m_actor.end(); i++) {
        x2=(*i)->getX();
        y2=(*i)->getY();
        d=distance(x, y, x2, y2);
        if(d<12)(*i)->setVisible(1);
    }
}

void StudentWorld::MarkMaze(int x, int y, char d){
    maze[x][y]=d;
}

//////////////////////////////////////////////////////////////////////////////////

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
