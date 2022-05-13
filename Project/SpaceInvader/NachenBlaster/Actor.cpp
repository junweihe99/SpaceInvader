#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/*
ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||ACTORS ||
*/
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_world = world;
	m_alive = true;
	m_alien = false;
	m_star = false;
	m_explosion = false;
	m_projectile = false;
	m_goodie = false;
	m_alien_num = 0;
	m_projectile_num = 0;
	m_goodie_num = 0;
	m_health = 1;
};

StudentWorld* Actor::getWorld() const
{
	return m_world;
}

bool Actor::getAlive() const
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

bool Actor::checkOffScreen()
{
	if (getX() < 0 || getX() >= VIEW_WIDTH)
	{
		setDead();
		return true;
	}
	return false;
}

bool Actor::isAlien() const
{
	return m_alien;
}

void Actor::setAlien()
{
	m_alien = true;
}

int Actor::getAlienNum() const
{
	return m_alien_num;
}

void Actor::setAlienNum(int num)
{
	m_alien_num = num;
}

bool Actor::isStar() const
{
	return m_star;
}

void Actor::setStar()
{
	m_star = true;
}

bool Actor::isExplosion() const
{
	return m_explosion;
}

void Actor::setExplosion()
{
	m_explosion = true;
}

bool Actor::isProjectile() const
{
	return m_projectile;
}

void Actor::setProjectile()
{
	m_projectile = true;
}

int Actor::getProjectileNum() const
{
	return m_projectile_num;
}

void Actor::setProjectileNum(int num)
{
	m_projectile_num = num;
}

bool Actor::isGoodie() const
{
	return m_goodie;
}

void Actor::setGoodie()
{
	m_goodie = true;
}

int Actor::getGoodieNum() const
{
	return m_goodie_num;
}

void Actor::setGoodieNum(int num)
{
	m_goodie_num = num;
}

double Actor::getHealth() const
{
		return m_health;
}

void Actor::setHealth(double health)
{
	m_health = health;
}

void Actor::sufferDamage(double amt)
{
	setHealth(getHealth() - amt);
}

/*
STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||STARS ||
*/
Star::Star(StudentWorld* world,int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY, dir, size, depth)
{
	setStar();
};

void Star::doSomething()
{
	if(!checkOffScreen())
		moveTo(getX() - 1.0, getY());
}


/*
DAMAGEABLE OBJECTS|| DAMAGEABLE OBJECTS|| DAMAGEABLE OBJECTS|| DAMAGEABLE OBJECTS|| DAMAGEABLE OBJECTS|| DAMAGEABLE OBJECTS|| 
*/
DamageableObject::DamageableObject(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY, dir, size, depth)
{};


void DamageableObject::increaseHitPoints(double amt)
{
	if(getHealth() < 50)
		setHealth(getHealth() + amt);
	if (getHealth() > 50)
		setHealth(50);
}

/*
NACHENBLASTERS ||NACHENBLASTERS ||NACHENBLASTERS ||NACHENBLASTERS ||NACHENBLASTERS ||NACHENBLASTERS ||NACHENBLASTERS ||
*/
NachenBlaster::NachenBlaster(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:DamageableObject(world, imageID, startX, startY, dir, size, depth)
{
	m_cabbage_points = 30;
	m_FlautlenceTorpedoes = 0;
	setHealth(50);
};

void NachenBlaster::doSomething()
{
	if (getHealth() <= 0)
	{
		setDead();
		return;
	}
	if (m_cabbage_points < 30)
	{
		m_cabbage_points++;
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_LEFT:
				if (getX() > 5)
				{
					moveTo(getX() - 6, getY());
				}
				break;
			case KEY_PRESS_RIGHT:
				if (getX() < VIEW_WIDTH-7)
				{
					moveTo(getX() + 6, getY());
				}
				break;
			case KEY_PRESS_UP:
				if (getY() < VIEW_HEIGHT-7)
				{
					moveTo(getX(), getY() + 6);
				}
				break;
			case KEY_PRESS_DOWN :
				if (getY() > 5)
				{
					moveTo(getX(), getY() - 6);
				}
				break;
			case KEY_PRESS_SPACE:
				if (m_cabbage_points >= 5)
				{
					getWorld()->pushNewObject(new Cabbages(getWorld(), IID_CABBAGE, getX() + 12, getY()));
					getWorld()->playSound(SOUND_PLAYER_SHOOT);
					m_cabbage_points -= 5;
				}
				break;
			case KEY_PRESS_TAB:
				{
					if (m_FlautlenceTorpedoes > 0)
					{
						getWorld()->pushNewObject(new FlatulenceTorpedoes(getWorld(), IID_TORPEDO, getX() + 12, getY()));
						getWorld()->playSound(SOUND_TORPEDO);
						m_FlautlenceTorpedoes--;
					}
					break;
				}
		}
	}
}

void NachenBlaster::increaseTorpedoes(int amt)
{
	m_FlautlenceTorpedoes += amt;
}

int NachenBlaster::getCabbage() const
{
	return m_cabbage_points;
}

int NachenBlaster::getTorpedoes() const
{
	return m_FlautlenceTorpedoes;
}

double NachenBlaster::getHealthPerct() const
{
	return (getHealth() / 50.0) * 100;
}

double NachenBlaster::getCabbagePerct() const
{
	return (getCabbage() / 30.0) * 100;
}

/*
EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||EXPLOSIONS ||
*/
Explosion::Explosion(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY, dir, size, depth)
{
	m_ticks = 0;
	setExplosion();
};

void Explosion::doSomething()
{
	if (m_ticks != 4)
	{
		setSize(getSize() * 1.5);
		m_ticks++;
	}
	if (m_ticks == 4)
	{
		setDead();
	}
}

/*
PROJECTILES ||PROJECTILES ||PROJECTILES ||PROJECTILES ||PROJECTILES ||PROJECTILES ||PROJECTILES ||PROJECTILES ||
*/
Projectiles::Projectiles(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY,dir, size, depth)
{
	setProjectile();
};

/*
CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||CABBAGES ||
*/
Cabbages::Cabbages(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Projectiles(world, imageID, startX, startY, dir, size, depth)
{
	setProjectileNum(1);
};

void Cabbages::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		moveTo(getX() + 8, getY());
		setDirection(getDirection() + 20);
	}
}


/*
TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||TURNIPS ||
*/
Turnips::Turnips(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Projectiles(world, imageID, startX, startY, dir, size, depth)
{
	setProjectileNum(2);
};

void Turnips::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		moveTo(getX() - 6, getY());
		setDirection(getDirection() + 20);
	}
}


/*
FLATUENCETORPEDOES ||FLATUENCETORPEDOES ||FLATUENCETORPEDOES ||FLATUENCETORPEDOES ||FLATUENCETORPEDOES ||FLATUENCETORPEDOES ||
*/
FlatulenceTorpedoes::FlatulenceTorpedoes(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Projectiles(world, imageID, startX, startY, dir, size, depth)
{
	setProjectileNum(3);
};

void FlatulenceTorpedoes::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		if (getDirection() == 0)
		{
			moveTo(getX() + 8, getY());
		}
		if (getDirection() == 180)
		{
			moveTo(getX() - 8, getY());
		}
	}
}


/*
GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||GOODIES ||
*/
Goodies::Goodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY, dir, size, depth)
{
	setGoodie();
};

void Goodies::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		moveTo(getX() - .75, getY() - .75);
	}
}


/*
REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||REPAIRGOODIES ||
*/
RepairGoodies::RepairGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Goodies(world, imageID, startX, startY, dir, size, depth)
{
	setGoodieNum(1);
};

/*
FLATUENCETOEPEDOESGOODIES ||FLATUENCETOEPEDOESGOODIES ||FLATUENCETOEPEDOESGOODIES ||FLATUENCETOEPEDOESGOODIES ||FLATUENCETOEPEDOESGOODIES ||
*/
FlatulenceTorpedoesGoodies::FlatulenceTorpedoesGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Goodies(world, imageID, startX, startY, dir, size, depth)
{
	setGoodieNum(2);
};


/*
EXTRALIFEGOODIES ||EXTRALIFEGOODIES ||EXTRALIFEGOODIES ||EXTRALIFEGOODIES ||EXTRALIFEGOODIES ||EXTRALIFEGOODIES ||
*/
ExtraLifeGoodies::ExtraLifeGoodies(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Goodies(world, imageID, startX, startY, dir, size, depth)
{
	setGoodieNum(3);
};

/*
ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||ALIENS ||
*/
Aliens::Aliens(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	: DamageableObject(world, imageID, startX, startY, dir, size, depth)
{
	setHealth(5 * (1 + (getWorld()->getLevel() - 1) * .1));
	m_flightplan = 0;
	m_speed = 2.0;
	m_travelDirection = 0;
	setAlien();
};

void Aliens::setSpeed(double speed)
{
	m_speed = speed;
}

double Aliens::getSpeed() const
{
	return m_speed;
}

int Aliens::getFlightplan() const
{
	return m_flightplan;
}

void Aliens::setFlightplan(int plan)
{
	m_flightplan = plan;
}

void Aliens::decreaseFlightplan()
{
	m_flightplan--;
}

int Aliens::getTravelDirection() const
{
	return m_travelDirection;
}

void Aliens::setTravelDirection(int dir)
{
	m_travelDirection = dir;
}

void Aliens::move()
{
	if (getTravelDirection() == 1)			//due left
	{
		moveTo(getX() - getSpeed(), getY());
		decreaseFlightplan();
	}
	else if (getTravelDirection() == 2)	//left and up
	{
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		decreaseFlightplan();
	}
	else if (getTravelDirection() ==3)					//left and down
	{
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		decreaseFlightplan();
	}
}

/*
SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||SMALLGONS ||
*/
Smallgons::Smallgons(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Aliens(world, imageID, startX, startY, dir, size, depth)
{
	setAlienNum(1);
};

void Smallgons::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		if (getFlightplan() == 0)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			int direction = randInt(1, 3);
			setTravelDirection(direction);
			move();
		}
		else if (getY() >= VIEW_HEIGHT - 1)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			setTravelDirection(3);
			move();
		}
		else if (getY() <= 0)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			setTravelDirection(2);
			move();
		}
		else
		{
			move();
		}
	}
	if (!checkOffScreen())
	{
		if (getWorld()->getBlaster()->getX() < this->getX())
		{
			double difference = getWorld()->getBlaster()->getY() - this->getY();
			if (difference >= -4.0 && difference <= 4.0)
			{
				if (randInt(1, 20) == 1)
				{
					getWorld()->pushNewObject(new Turnips(getWorld(), IID_TURNIP, this->getX() - 14, this->getY()));
					getWorld()->playSound(SOUND_ALIEN_SHOOT);
				}
			}
		}
	}
}


/*
SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||SMOREGONS ||
*/
Smoregons::Smoregons(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Aliens(world, imageID, startX, startY, dir, size, depth)
{
	setAlienNum(2);
};

void Smoregons::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		if (getFlightplan() == 0)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			int direction = randInt(1, 3);
			setTravelDirection(direction);
			move();
		}
		else if (getY() >= VIEW_HEIGHT - 1)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			setTravelDirection(3);
			move();
		}
		else if (getY() <= 0)
		{
			int newFlightPlan = randInt(1, 32);
			setFlightplan(newFlightPlan);
			setTravelDirection(2);
			move();
		}
		else
		{
			move();
		}
	}
	if (!checkOffScreen())
	{
		if (getWorld()->getBlaster()->getX() < this->getX())
		{
			double difference = getWorld()->getBlaster()->getY() - this->getY();
			if (difference >= -4.0 && difference <= 4.0)
			{
				int num = (20 / (getWorld()->getLevel())) + 5;
				if (randInt(1, num) == 1)
				{
					getWorld()->pushNewObject(new Turnips(getWorld(), IID_TURNIP, this->getX() - 14, this->getY()));
					getWorld()->playSound(SOUND_ALIEN_SHOOT);
				}
				if (randInt(1, num) == 1)
				{
					setTravelDirection(1);
					setFlightplan(VIEW_WIDTH);
					setSpeed(5);
				}
			}
		}
	}
}

/*
SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||SNAGGLEGONS ||
*/
Snagglegons::Snagglegons(StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	:Aliens(world, imageID, startX, startY, dir, size, depth)
{
	setHealth(getHealth() *2);
	setSpeed(1.75);
	setTravelDirection(3);
	setAlienNum(3);
};

void Snagglegons::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (!checkOffScreen())
	{
		if (getY() >= VIEW_HEIGHT - 1)
		{
			setTravelDirection(3);
			move();
		}
		else if (getY() <= 0)
		{
			setTravelDirection(2);
			move();
		}
		else
		{
			move();
		}
	}
	if (!checkOffScreen())
	{
		if (getWorld()->getBlaster()->getX() < this->getX())
		{
			double difference = getWorld()->getBlaster()->getY() - this->getY();
			if (difference >= -4.0 && difference <= 4.0)
			{
				int num = (15 / getWorld()->getLevel()) + 10;
				if (randInt(1, num) == 1)
				{
					getWorld()->pushNewObject(new FlatulenceTorpedoes(getWorld(), IID_TORPEDO, this->getX() - 14, this->getY(), 180));
					getWorld()->playSound(SOUND_TORPEDO);
				}
			}
		}
	}
}
