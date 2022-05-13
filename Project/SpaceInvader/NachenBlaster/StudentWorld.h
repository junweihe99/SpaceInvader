#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	int getShipDestroyed() const;	//how many ships destroyed
	int getTotalShip() const;		//nummber of total ships to be destroyed before level complete
	int getMaxShip() const;			//max number of ship on screen
	int getCurrShip() const;		//current  number of ship on screen
	void pushNewObject(Actor* actor);
	NachenBlaster* getBlaster() const;
	void removeDeadObjects();
	void addNewStar();
	void addNewAlien();
	double euclidean_dist(double x1, double y1, double x2, double y2);
	bool collide(Actor* a1, Actor* a2);
	void updateStatus();

private:
	vector<Actor*> m_actor;
	vector<NachenBlaster*> m_blaster;
	int m_alienshipDestroyed;
	int m_totalShip;
	int m_maxShip;
	int m_currShip;
};

#endif // STUDENTWORLD_H_
