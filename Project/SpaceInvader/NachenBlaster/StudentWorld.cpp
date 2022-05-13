#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{
	m_alienshipDestroyed = 0;
	m_currShip = 0;
	m_totalShip = 6 + (4 * getLevel());
	m_maxShip = 4 + (.5 * getLevel());
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::getShipDestroyed() const
{
	return m_alienshipDestroyed;
}

int StudentWorld::getTotalShip() const
{
	return m_totalShip;
}

int StudentWorld::getMaxShip() const
{
	return m_maxShip;
}

int StudentWorld::getCurrShip() const
{
	return m_currShip;
}

void StudentWorld::pushNewObject(Actor* actor)
{
	m_actor.push_back(actor);
}

NachenBlaster* StudentWorld::getBlaster() const
{
	return m_blaster[0];
}

void StudentWorld::removeDeadObjects()
{
	for (vector<Actor*>::iterator it = m_actor.begin(); it != m_actor.end(); )
	{
		if (!(*it)->getAlive())
		{
			if ((*it)->isAlien())
			{
				m_currShip--;
			}
			delete (*it);
			it = m_actor.erase(it);
		}
		else
		{
			it++;

		}
	}
}

void StudentWorld::addNewStar()
{
	int number = randInt(1, 15);
	if (number == 1)
	{
		int y = randInt(0, VIEW_HEIGHT - 1);
		int num = randInt(5, 50);
		double size = num / 100.0;
		m_actor.push_back(new Star(this, IID_STAR, VIEW_WIDTH - 1, y, 0, size, 3));
	}
}

void StudentWorld::addNewAlien()
{
	int R = getTotalShip() - getShipDestroyed();
	if (getCurrShip() < min(getMaxShip(), R) && getCurrShip() < getMaxShip())
	{
		int s1 = 60;
		int s2 = 20 + (getLevel() * 5);
		int s3 = 5 + (getLevel() * 10);
		int s = s1 + s2 + s3;
		int y = randInt(0, VIEW_HEIGHT - 1);
		int num = randInt(1, s);
		if (num <= 60)
		{
			m_actor.push_back(new Smallgons(this, IID_SMALLGON, VIEW_WIDTH - 1, y));
			m_currShip++;
		}
		else if (num > 60 && num <= (s - s3))
		{
			m_actor.push_back(new Smoregons(this, IID_SMOREGON, VIEW_WIDTH - 1, y));
			m_currShip++;
		}
		else
		{
			m_actor.push_back(new Snagglegons(this, IID_SNAGGLEGON, VIEW_WIDTH - 1, y));
			m_currShip++;
		}
	}
}

double StudentWorld::euclidean_dist(double x1, double y1, double x2, double y2)
{
	double one = x1 - x2;
	double two = y1 - y2;
	double first = pow(one, 2.0);
	double second = pow(two, 2.0);
	return sqrt(first + second);
}

bool StudentWorld::collide(Actor* a1, Actor* a2)
{
	return (euclidean_dist(a1->getX(), a1->getY(), a2->getX(), a2->getY()) < .75 * (a1->getRadius() + a2->getRadius()));
}

void StudentWorld::updateStatus()
{
	ostringstream oss;
	oss.setf(ios::fixed);
	oss.precision(0);
	oss << "Lives:  " << getLives() << "  Health:  "<< m_blaster[0]->getHealthPerct() << "%" << "  Score:  " << getScore() << "  Level:  " << getLevel() << "  Cabbages:  " << m_blaster[0]->getCabbagePerct() << "%" << "  Torpedoes:  " << m_blaster[0]->getTorpedoes() <<endl;
	setGameStatText(oss.str());
}

int StudentWorld::init()
{
	m_alienshipDestroyed = 0;
	m_currShip = 0;
	m_totalShip = 6 + (4 * getLevel());
	m_maxShip = 4 + (.5 * getLevel());
	for (int i = 0; i < 30; i++)
	{
		int x = randInt(0, VIEW_WIDTH - 1);
		int y = randInt(0, VIEW_HEIGHT - 1);
		int num = randInt(5, 50);
		double size = num / 100.0;
		m_actor.push_back(new Star(this,IID_STAR, x, y, 0, size, 3));
	}
	m_blaster.push_back(new NachenBlaster(this, IID_NACHENBLASTER, 0, 128));
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	updateStatus();
	for (int i = 0; i < m_actor.size(); i++)
	{
		if (m_actor[i]->isAlien())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getAlienNum() <= 2)
				{
					m_blaster[0]->sufferDamage(5);
					m_actor[i]->setDead();
					increaseScore(250);
					m_alienshipDestroyed++;
					playSound(SOUND_DEATH);
					m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
					if (m_actor[i]->getAlienNum() == 2)
					{
						if (randInt(1, 3) == 1)
						{
							if (randInt(1, 2) == 1)
							{
								m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
							else
							{
								m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
					}
					if (m_blaster[0]->getHealth() <= 0)
					{
						m_blaster[0]->setDead();
						decLives();
						return GWSTATUS_PLAYER_DIED;
					}
					if (m_alienshipDestroyed == m_totalShip)
					{
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
				else
				{
					m_blaster[0]->sufferDamage(15);
					m_actor[i]->setDead();
					increaseScore(1000);
					m_alienshipDestroyed++;
					playSound(SOUND_DEATH);
					m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
					if (randInt(1, 6) == 1)
					{
						m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
					}
					if (m_blaster[0]->getHealth() <= 0)
					{
						m_blaster[0]->setDead();
						decLives();
						return GWSTATUS_PLAYER_DIED;
					}
					if (m_alienshipDestroyed == m_totalShip)
					{
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
		else if (m_actor[i]->isProjectile())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getProjectileNum() == 2)
				{
					m_blaster[0]->sufferDamage(2);
					m_actor[i]->setDead();
					playSound(SOUND_BLAST);
				}
				if (m_actor[i]->getProjectileNum() == 3)
				{
					m_blaster[0]->sufferDamage(8);
					m_actor[i]->setDead();
					playSound(SOUND_BLAST);
				}
				if (m_blaster[0]->getHealth() <= 0)
				{
					m_blaster[0]->setDead();
					decLives();
					return GWSTATUS_PLAYER_DIED;
				}
			}
		}
		else if (m_actor[i]->isGoodie())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getGoodieNum() == 1)
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					m_blaster[0]->increaseHitPoints(10);
				}
				else if (m_actor[i]->getGoodieNum() == 2)
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					m_blaster[0]->increaseTorpedoes(5);
				}
				else
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					incLives();
				}
			}
		}
		else
		{
			continue;
		}
	}
	for (int i = 0; i < m_actor.size(); i++)
	{
		for (int j = i + 1; j < m_actor.size(); j++)
		{
			if (m_actor[i]->isAlien() && m_actor[j]->getProjectileNum() == 1)
			{
				if (collide(m_actor[i], m_actor[j]))
				{
					m_actor[i]->sufferDamage(2);
					m_actor[j]->setDead();
					if (m_actor[i]->getHealth() <= 0)
					{
						m_actor[i]->setDead();
						playSound(SOUND_DEATH);
						if (m_actor[i]->getAlienNum() < 3)
						{
							increaseScore(250);
							if (m_actor[i]->getAlienNum() == 2)
							{
								if (randInt(1, 3) == 1)
								{
									if (randInt(1, 2) == 1)
									{
										m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
									else
									{
										m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
								}
							}
						}
						else
						{
							increaseScore(1000);
							if (randInt(1, 6) == 1)
							{
								m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
						m_alienshipDestroyed++;
						m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
						if (m_alienshipDestroyed == m_totalShip)
						{
							playSound(SOUND_FINISHED_LEVEL);
							return GWSTATUS_FINISHED_LEVEL;
						}
					}
					else
					{
						playSound(SOUND_BLAST);
					}
				}
			}
			else if (m_actor[i]->isAlien() && m_actor[j]->getProjectileNum() == 3 && m_actor[j]->getDirection() == 0)
			{
				if (collide(m_actor[i], m_actor[j]))
				{
					m_actor[i]->sufferDamage(8);
					m_actor[j]->setDead();
					if (m_actor[i]->getHealth() <= 0)
					{
						m_actor[i]->setDead();
						playSound(SOUND_DEATH);
						if (m_actor[i]->getAlienNum() < 3)
						{
							increaseScore(250);
							if (m_actor[i]->getAlienNum() == 2)
							{
								if (randInt(1, 3) == 1)
								{
									if (randInt(1, 2) == 1)
									{
										m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
									else
									{
										m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
								}
							}
						}
						else
						{
							increaseScore(1000);
							if (randInt(1, 6) == 1)
							{
								m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
						m_alienshipDestroyed++;
						m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
						if (m_alienshipDestroyed == m_totalShip)
						{
							playSound(SOUND_FINISHED_LEVEL);
							return GWSTATUS_FINISHED_LEVEL;
						}
					}
					else
					{
						playSound(SOUND_BLAST);
					}
				}
			}
			else
			{
				continue;
			}
		}
	}
	for (int i = 0; i < m_actor.size(); i++)
	{
		m_actor[i]->doSomething();
	}
	m_blaster[0]->doSomething();
	for (int i = 0; i < m_actor.size(); i++)
	{
		if (m_actor[i]->isAlien())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getAlienNum() <= 2)
				{
					m_blaster[0]->sufferDamage(5);
					m_actor[i]->setDead();
					increaseScore(250);
					m_alienshipDestroyed++;
					playSound(SOUND_DEATH);
					m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
					if (m_actor[i]->getAlienNum() == 2)
					{
						if (randInt(1, 3) == 1)
						{
							if (randInt(1, 2) == 1)
							{
								m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
							else
							{
								m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
					}
					if (m_blaster[0]->getHealth() <= 0)
					{
						m_blaster[0]->setDead();
						decLives();
						return GWSTATUS_PLAYER_DIED;
					}
					if (m_alienshipDestroyed == m_totalShip)
					{
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
				else
				{
					m_blaster[0]->sufferDamage(15);
					m_actor[i]->setDead();
					increaseScore(1000);
					m_alienshipDestroyed++;
					playSound(SOUND_DEATH);
					m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
					if (randInt(1, 6) == 1)
					{
						m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
					}
					if (m_blaster[0]->getHealth() <= 0)
					{
						m_blaster[0]->setDead();
						decLives();
						return GWSTATUS_PLAYER_DIED;
					}
					if (m_alienshipDestroyed == m_totalShip)
					{
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
		else if (m_actor[i]->isProjectile())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getProjectileNum() == 2)
				{
					m_blaster[0]->sufferDamage(2);
					m_actor[i]->setDead();
					playSound(SOUND_BLAST);
				}
				if (m_actor[i]->getProjectileNum() == 3)
				{
					m_blaster[0]->sufferDamage(8);
					m_actor[i]->setDead();
					playSound(SOUND_BLAST);
				}
				if (m_blaster[0]->getHealth() <= 0)
				{
					m_blaster[0]->setDead();
					decLives();
					return GWSTATUS_PLAYER_DIED;
				}
			}
		}
		else if(m_actor[i]->isGoodie())
		{
			if (collide(m_blaster[0], m_actor[i]))
			{
				if (m_actor[i]->getGoodieNum() == 1)
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					m_blaster[0]->increaseHitPoints(10);
				}
				else if (m_actor[i]->getGoodieNum() == 2)
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					m_blaster[0]->increaseTorpedoes(5);
				}
				else
				{
					increaseScore(100);
					m_actor[i]->setDead();
					playSound(SOUND_GOODIE);
					incLives();
				}
			}
		}
		else
		{
			continue;
		}
	}
	for (int i = 0; i < m_actor.size(); i++)
	{
		for (int j = i + 1; j < m_actor.size(); j++)
		{
			if (m_actor[i]->isAlien() && m_actor[j]->getProjectileNum() == 1)
			{
				if (collide(m_actor[i], m_actor[j]))
				{
					m_actor[i]->sufferDamage(2);
					m_actor[j]->setDead();
					if (m_actor[i]->getHealth() <= 0)
					{
						m_actor[i]->setDead();
						playSound(SOUND_DEATH);
						if (m_actor[i]->getAlienNum() < 3)
						{
							increaseScore(250);
							if (m_actor[i]->getAlienNum() == 2)
							{
								if (randInt(1, 3) == 1)
								{
									if (randInt(1, 2) == 1)
									{
										m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
									else
									{
										m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
								}
							}
						}
						else
						{
							increaseScore(1000);
							if (randInt(1, 6) == 1)
							{
								m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
						m_alienshipDestroyed++;
						m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
						if (m_alienshipDestroyed == m_totalShip)
						{
							playSound(SOUND_FINISHED_LEVEL);
							return GWSTATUS_FINISHED_LEVEL;
						}
					}
					else
					{
						playSound(SOUND_BLAST);
					}
				}
			}
			else if (m_actor[i]->isAlien() && m_actor[j]->getProjectileNum() == 3 && m_actor[j]->getDirection() == 0)
			{
				if (collide(m_actor[i], m_actor[j]))
				{
					m_actor[i]->sufferDamage(8);
					m_actor[j]->setDead();
					if (m_actor[i]->getHealth() <= 0)
					{
						m_actor[i]->setDead();
						playSound(SOUND_DEATH);
						if (m_actor[i]->getAlienNum() < 3)
						{
							increaseScore(250);
							if (m_actor[i]->getAlienNum() == 2)
							{
								if (randInt(1, 3) == 1)
								{
									if (randInt(1, 2) == 1)
									{
										m_actor.push_back(new RepairGoodies(this, IID_REPAIR_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
									else
									{
										m_actor.push_back(new FlatulenceTorpedoesGoodies(this, IID_TORPEDO_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
									}
								}
							}
						}
						else
						{
							increaseScore(1000);
							if (randInt(1, 6) == 1)
							{
								m_actor.push_back(new ExtraLifeGoodies(this, IID_LIFE_GOODIE, m_actor[i]->getX(), m_actor[i]->getY()));
							}
						}
						m_alienshipDestroyed++;
						m_actor.push_back(new Explosion(this, IID_EXPLOSION, m_actor[i]->getX(), m_actor[i]->getY()));
						if (m_alienshipDestroyed == m_totalShip)
						{
							playSound(SOUND_FINISHED_LEVEL);
							return GWSTATUS_FINISHED_LEVEL;
						}
					}
					else
					{
						playSound(SOUND_BLAST);
					}
				}
			}
			else
			{
				continue;
			}
		}
	}
	removeDeadObjects();
	addNewStar();
	addNewAlien();
	return GWSTATUS_CONTINUE_GAME;
	//return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	for (vector<Actor*>::iterator it = m_actor.begin(); it!= m_actor.end();)
	{
		delete (*it);
		it = m_actor.erase(it);
	}
	for (vector<NachenBlaster*>::iterator iter = m_blaster.begin(); iter != m_blaster.end();)
	{
		delete *iter;
		iter=m_blaster.erase(iter);
	}
}
