#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

/*
ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||
*/
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = 0, double size = 1, unsigned int depth = 0);
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	bool getAlive() const;
	bool checkOffScreen();
	void setDead();
	bool isAlien() const;
	void setAlien();
	int getAlienNum() const;
	void setAlienNum(int num);
	bool isStar() const;
	void setStar();
	bool isExplosion() const;
	void setExplosion();
	bool isProjectile() const;
	void setProjectile();
	int getProjectileNum() const;
	void setProjectileNum(int num);
	bool isGoodie() const;
	void setGoodie();
	int getGoodieNum() const;
	void setGoodieNum(int num);
	double getHealth() const;
	void sufferDamage(double amt);
	void setHealth(double health);
private:
	StudentWorld* m_world;
	bool m_alive;
	bool m_alien;
	bool m_star;
	bool m_explosion;
	bool m_projectile;
	bool m_goodie;
	int m_alien_num;
	int m_projectile_num;
	int m_goodie_num;
	double m_health;
};


/*
STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||
*/
class Star : public Actor
{
public:
	Star(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth);
	virtual void doSomething();
};


/*
DAMAGEABLE OBJECT ||DAMAGEABLE OBJECT ||DAMAGEABLE OBJECT ||DAMAGEABLE OBJECT ||DAMAGEABLE OBJECT ||DAMAGEABLE OBJECT ||
*/
class DamageableObject : public Actor
{
public:
	DamageableObject(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth);
	virtual void doSomething() =0;
	void increaseHitPoints(double amt);
private:
};


/*
NACHENBLASTER || NACHENBLASTER ||NACHENBLASTER ||NACHENBLASTER ||NACHENBLASTER ||NACHENBLASTER ||NACHENBLASTER ||
*/
class NachenBlaster : public DamageableObject
{
public:
	NachenBlaster(StudentWorld* world, int imageID, double startX, double startY, Direction dir = 0, double size = 1, unsigned int depth = 0);
	virtual void doSomething();
	void increaseTorpedoes(int amt);
	int getCabbage() const;
	int getTorpedoes() const;
	double getHealthPerct() const;
	double getCabbagePerct() const;
private:
	int m_cabbage_points;
	int m_FlautlenceTorpedoes;
};


/*
EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||
*/
class Explosion : public Actor
{
public:
	Explosion(StudentWorld* world, int imageID, double startX, double startY, Direction dir = 0, double size = 1, unsigned int depth=0);
	virtual void doSomething();
private:
	int m_ticks;
};


/*
ALIENS || ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||
*/
class Aliens : public DamageableObject
{
public:
	Aliens(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth);
	void setSpeed(double speed);
	double getSpeed() const;
	int getFlightplan() const;
	void setFlightplan(int plan);
	void decreaseFlightplan();
	int getTravelDirection() const;
	void setTravelDirection(int dir);
	void move();
private:
	int m_travelDirection;
	int m_flightplan;
	double m_speed;
};

class Smallgons : public Aliens
{
public:
	Smallgons(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=1.5, unsigned int depth=1);
	virtual void doSomething();
private:
};

class Smoregons : public Aliens
{
public:
	Smoregons(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=1.5, unsigned int depth=1);
	virtual void doSomething();
private:
};

class Snagglegons : public Aliens
{
public:
	Snagglegons(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=1.5, unsigned int depth=1);
	virtual void doSomething();
private:
};


/*
PROJECILES || PROJECILES ||PROJECILES ||PROJECILES ||PROJECILES ||PROJECILES ||PROJECILES ||PROJECILES ||PROJECILES ||
*/
class Projectiles : public Actor
{
public:
	Projectiles(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=.5, unsigned int depth=1);
private:
};

class Cabbages : public Projectiles
{
public:
	Cabbages(StudentWorld* world, int imageID, double startX, double startY, Direction dir =0, double size =.5, unsigned int depth =1);
	virtual void doSomething();
private:
};

class Turnips : public Projectiles
{
public:
	Turnips(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size =.5, unsigned int depth=1);
	virtual void doSomething();
private:
};

class FlatulenceTorpedoes : public Projectiles
{
public:
	FlatulenceTorpedoes(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=.5, unsigned int depth=1);
	virtual void doSomething();
private:
};


/*
GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || GOODIES || 
*/
class Goodies : public Actor
{
public:
	Goodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth);
	virtual void doSomething();
private:
};

class RepairGoodies : public Goodies
{
public:
	RepairGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=.5, unsigned int depth=1);
private:
};

class ExtraLifeGoodies : public Goodies
{
public:
	ExtraLifeGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=.5, unsigned int depth=1);
private:
};

class FlatulenceTorpedoesGoodies : public Goodies
{
public:
	FlatulenceTorpedoesGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir=0, double size=.5, unsigned int depth=1);
private:
};

#endif // ACTOR_H_
