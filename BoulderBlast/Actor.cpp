#include "Actor.h"
#include "StudentWorld.h"

using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world)
:GraphObject(imageID, startX, startY, none)
{
    m_name="";
    m_alive=true;
    m_studentWorld=world;
}

string Actor::getName() const
{
    return m_name;
}

void Actor::setName(string name)
{
    m_name=name;
}

bool Actor::alive()
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive=false;
}

Actor* Actor::getAnActorAtTheProposedLocation(int x, int y, const vector<Actor*> &actors)
{
    vector<Actor*>::const_iterator it;
    it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->getX()==x && (*it)->getY()==y)
        {
            return *it;
        }
        it++;
    }
    //nullptr indicates an empty space
    return nullptr;
}

StudentWorld* Actor::getWorld()
{
    return m_studentWorld;
}

//Dummy function, since Player will redefine getHitPoints()
int Actor::getHitPoints() const
{
    return 0;
}

//Dummy function, since Player will redefine getAmmo()
int Actor::getAmmo() const
{
    return 0;
}

Actor::~Actor()
{
    
}

Wall::Wall(int startX, int startY, StudentWorld* world)
:Actor(IID_WALL, startX, startY, world)
{
    setName("Wall");
    setVisible(true);
}

void Wall::doSomething()
{
    return;
}

bool Wall::blocksPlayer()
{
    return true;
}

bool Wall::blocksBoulder()
{
    return true;
}

void Wall::isDamaged()
{
    return;
}

Wall::~Wall()
{
    
}

Player::Player(int startX, int startY, StudentWorld* world)
:Actor(IID_PLAYER, startX, startY, world)
{
    setName("Player");
    m_hitPoints=20;
    m_ammo=20;
    setDirection(right);
    setVisible(true);
}

void Player::doSomething()
{
    //Check to see if Player isn't alive
    if(alive()==false)
    {
        return;
    }
    int characterEntered;
    if(getWorld()->getKey(characterEntered))
    {
        //will need base pointer to see if object is in the way, depending on which key the user pressed
        Actor* actor;
        //user hit a key during this tick
        switch(characterEntered)
        {
            case KEY_PRESS_LEFT:
            {
                setDirection(left);
                actor=getAnActorAtTheProposedLocation(getX()-1, getY(), this->getWorld()->getActors());
                if(actor!=nullptr)
                {
                    Boulder* boulder=dynamic_cast<Boulder*>(actor);
                    if(boulder!=nullptr)
                    {
                        int x=boulder->getX();
                        int y=boulder->getY();
                        boulder->push(left);
                        //If the boulder was moved, then its location would have been changed, which would allow the player to move to the boulder's previous location
                        if(boulder->getX()!=x || boulder->getY()!=y)
                        {
                            moveTo(getX()-1, getY());
                        }
                    }
                }
                if(actor==nullptr || (actor->blocksPlayer()==false && actor->getName()!="Boulder"))
                {
                    moveTo(getX()-1, getY());
                }
                break;
            }
                
            case KEY_PRESS_RIGHT:
            {
                setDirection(right);
                actor=getAnActorAtTheProposedLocation(getX()+1, getY(), this->getWorld()->getActors());
                if(actor!=nullptr)
                {
                    Boulder* boulder=dynamic_cast<Boulder*>(actor);
                    if(boulder!=nullptr)
                    {
                        int x=boulder->getX();
                        int y=boulder->getY();
                        boulder->push(right);
                        //If the boulder was moved, then its location would have been changed, which would allow the player to move to the boulder's previous location
                        if(boulder->getX()!=x || boulder->getY()!=y)
                        {
                            moveTo(getX()+1, getY());
                        }
                    }
                }
                if(actor==nullptr || (actor->blocksPlayer()==false && actor->getName()!="Boulder"))
                {
                    moveTo(getX()+1, getY());
                }
                break;
            }
                
            case KEY_PRESS_UP:
            {
                setDirection(up);
                actor=getAnActorAtTheProposedLocation(getX(), getY()+1, this->getWorld()->getActors());
                if(actor!=nullptr)
                {
                    Boulder* boulder=dynamic_cast<Boulder*>(actor);
                    if(boulder!=nullptr)
                    {
                        int x=boulder->getX();
                        int y=boulder->getY();
                        boulder->push(up);
                        //If the boulder was moved, then its location would have been changed, which would allow the player to move to the boulder's previous location
                        if(boulder->getX()!=x || boulder->getY()!=y)
                        {
                            moveTo(getX(), getY()+1);
                        }
                    }
                }
                if(actor==nullptr || (actor->blocksPlayer()==false && actor->getName()!="Boulder"))
                {
                    moveTo(getX(), getY()+1);
                }
                break;
            }
                
            case KEY_PRESS_DOWN:
            {
                setDirection(down);
                actor=getAnActorAtTheProposedLocation(getX(), getY()-1, this->getWorld()->getActors());
                if(actor!=nullptr)
                {
                    Boulder* boulder=dynamic_cast<Boulder*>(actor);
                    if(boulder!=nullptr)
                    {
                        int x=boulder->getX();
                        int y=boulder->getY();
                        boulder->push(down);
                        //If the boulder was moved, then its location would have been changed, which would allow the player to move to the boulder's previous location
                        if(boulder->getX()!=x || boulder->getY()!=y)
                        {
                            moveTo(getX(), getY()-1);
                        }
                    }
                }
                if(actor==nullptr || (actor->blocksPlayer()==false && actor->getName()!="Boulder"))
                {
                    moveTo(getX(), getY()-1);
                }
                break;
            }
            
            case KEY_PRESS_SPACE:
            {
                if(m_ammo>0)
                {
                    int x=getX();
                    int y=getY();
                    Direction dir=getDirection();
                    
                    if(dir==up)
                    {
                        y=y+1;
                    }
                    else if(dir==right)
                    {
                        x=x+1;
                    }
                    else if(dir==down)
                    {
                        y=y-1;
                    }
                    else if(dir==left)
                    {
                        x=x-1;
                    }
                    
                    Actor* bullet = new Bullet(x, y, getWorld(), dir);
                    getWorld()->addActor(bullet);
                    m_ammo--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
                
            case KEY_PRESS_ESCAPE:
            {
                setDead();
                getWorld()->decLives();
                break;
            }
        }
    }
}

bool Player::blocksPlayer()
{
    return false;
}

bool Player::blocksBoulder()
{
    return false;
}

void Player::isDamaged()
{
    m_hitPoints=m_hitPoints-2;
    if(m_hitPoints>0)
    {
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    }
    else
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

int Player::getHitPoints() const
{
    return m_hitPoints;
}

void Player::restoreHealthPoints()
{
    m_hitPoints=20;
}

int Player::getAmmo() const
{
    return m_ammo;
}

void Player::addAmmo()
{
    m_ammo+=20;
}

Player::~Player()
{
    
}

Boulder::Boulder(int startX, int startY, StudentWorld* world)
:Actor(IID_BOULDER, startX, startY, world)
{
    setName("Boulder");
    m_hitPoints=10;
    setVisible(true);
}

void Boulder::doSomething()
{
    return;
}

bool Boulder::blocksPlayer()
{
    return false;
}

bool Boulder::blocksBoulder()
{
    return true;
}

void Boulder::isDamaged()
{
    m_hitPoints=m_hitPoints-2;
    if(m_hitPoints==0)
    {
        setDead();
    }
}

void Boulder::push(Direction dir)
{
    //Check to see if Boulder can be moved
    int x=getX();
    int y=getY();
    bool canBeMoved=false;
    
    if(dir==up)
    {
        y=y+1;
    }
    else if(dir==right)
    {
        x=x+1;
    }
    else if(dir==down)
    {
        y=y-1;
    }
    else if(dir==left)
    {
        x=x-1;
    }
    
    Actor* actorAtLocation=getAnActorAtTheProposedLocation(x, y, getWorld()->getActors());
    if(actorAtLocation==nullptr || actorAtLocation->blocksBoulder()==false)
    {
        canBeMoved=true;
    }
    
    //If it can be moved, move to designated spot
    if(canBeMoved==true)
    {
        moveTo(x, y);
    }
}

int Boulder::getHitPoints() const
{
    return m_hitPoints;
}

Boulder::~Boulder()
{
    
}

Bullet::Bullet(int startX, int startY, StudentWorld* world, Direction dir)
:Actor(IID_BULLET, startX, startY, world)
{
    setName("Bullet");
    setDirection(dir);
    setVisible(true);
    completedFirstTickAlready=false;
}

void Bullet::doSomething()
{
    if(completedFirstTickAlready==false)
    {
        completedFirstTickAlready=true;
        return;
    }
    //Check to see if Bullet isn't alive
    if(alive()==false)
    {
        return;
    }
    
    Bullet::checkLocation();
    if(alive()==false)
    {
        return; 
    }
    
    //Move one square in the initially-specified direction
    Direction currentDir=getDirection();
    if(currentDir==left)
    {
        moveTo(getX()-1, getY());
    }
    else if(currentDir==right)
    {
        moveTo(getX()+1, getY());
    }
    else if(currentDir==up)
    {
        moveTo(getX(), getY()+1);
    }
    else if(currentDir==down)
    {
        moveTo(getX(), getY()-1);
    }
    
    Bullet::checkLocation();
}

bool Bullet::blocksPlayer()
{
    return false;
}

bool Bullet::blocksBoulder()
{
    return false;
}

void Bullet::isDamaged()
{
    return;
}

void Bullet::checkLocation()
{
    //Check to see if any other object is on Bullet's square
    Actor* actor=getAnActorAtTheProposedLocation(getX(), getY(), this->getWorld()->getActors());
    if(actor->getName()=="Boulder" || actor->getName()=="Player" || actor->getName()=="Robot")
    {
        actor->isDamaged();
        setDead();
    }
    else if(actor->getName()=="Bullet" || actor->getName()=="Hole" || actor->getName()=="Jewel" || actor->getName()=="Exit" || actor->getName()=="Extra Life Goodie" || actor->getName()=="Restore Health Goodie" || actor->getName()=="Ammo Goodie")
    {
        return;
    }
    //Else if it's a Wall or Robot Factory
    else
    {
        setDead();
    }
}

Bullet::~Bullet()
{
    
}

Hole::Hole(int startX, int startY, StudentWorld* world)
:Actor(IID_HOLE, startX, startY, world)
{
    setName("Hole");
    setVisible(true);
}

void Hole::doSomething()
{
    //Check to see if Hole isn't alive
    if(alive()==false)
    {
        return;
    }
    
    //Check to see if a Boulder is on the Hole
    
    Actor* potentialBoulder=getPotentialBoulder(getX(), getY(), this->getWorld()->getActors());
    if(potentialBoulder!=nullptr)
    {
        this->setDead();
        potentialBoulder->setDead();
    }
}

bool Hole::blocksPlayer()
{
    return true;
}

bool Hole::blocksBoulder()
{
    return false;
}

void Hole::isDamaged()
{
    return;
}

Actor* Hole::getPotentialBoulder(int x, int y, const std::vector<Actor*> &actors)
{
    vector<Actor*>::const_iterator it;
    it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->getX()==x && (*it)->getY()==y && (*it)->getName()=="Boulder")
        {
            return *it;
        }
        it++;
    }
    //nullptr means that there is not a Boulder occupying the same location as the hole, so return nullptr as default case
    return nullptr;
}

Hole::~Hole()
{
    
}

Jewel::Jewel(int startX, int startY, StudentWorld* world)
:Actor(IID_JEWEL, startX, startY, world)
{
    setName("Jewel");
    setVisible(true);
}

void Jewel::doSomething()
{
    //Check to see if Jewel isn't alive
    if(alive()==false)
    {
        return;
    }
    
    if(getWorld()->getPlayer()->getX()==this->getX() && getWorld()->getPlayer()->getY()==this->getY())
    {
        getWorld()->increaseScore(50);
        this->setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->decrementNumJewels();
    }
}

bool Jewel::blocksPlayer()
{
    return false;
}

bool Jewel::blocksBoulder()
{
    return true;
}

void Jewel::isDamaged()
{
    return;
}

Jewel::~Jewel()
{
    
}

Exit::Exit(int startX, int startY, StudentWorld* world)
:Actor(IID_EXIT, startX, startY, world)
{
    setName("Exit");
    exitRevealed=false;
}

void Exit::doSomething()
{
    if(exitRevealed==false && getWorld()->getNumJewels()==0)
    {
        exitRevealed=true;
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    if(exitRevealed==true && getWorld()->getPlayer()->getX()==this->getX() && getWorld()->getPlayer()->getY()==this->getY())
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->increaseScore(getWorld()->getBonusPoints());
        getWorld()->didCompleteLevel();
    }
}

bool Exit::blocksPlayer()
{
    return false;
}

bool Exit::blocksBoulder()
{
    return true;
}

void Exit::isDamaged()
{
    return;
}

Exit::~Exit()
{
    
}

ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY, StudentWorld* world)
:Actor(IID_EXTRA_LIFE, startX, startY, world)
{
    setName("Extra Life Goodie");
    setVisible(true);
}

void ExtraLifeGoodie::doSomething()
{
    //Check to see if ExtraLifeGoodie isn't alive
    if(alive()==false)
    {
        return;
    }
    
    if(getWorld()->getPlayer()->getX()==this->getX() && getWorld()->getPlayer()->getY()==this->getY())
    {
        getWorld()->increaseScore(1000);
        this->setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
    }
}

bool ExtraLifeGoodie::blocksPlayer()
{
    return false;
}

bool ExtraLifeGoodie::blocksBoulder()
{
    return true;
}

void ExtraLifeGoodie::isDamaged()
{
    return;
}

ExtraLifeGoodie::~ExtraLifeGoodie()
{
    
}

RestoreHealthGoodie::RestoreHealthGoodie(int startX, int startY, StudentWorld* world)
:Actor(IID_RESTORE_HEALTH, startX, startY, world)
{
    setName("Restore Health Goodie");
    setVisible(true);
}

void RestoreHealthGoodie::doSomething()
{
    //Check to see if RestoreHealthGoodie isn't alive
    if(alive()==false)
    {
        return;
    }
    
    if(getWorld()->getPlayer()->getX()==this->getX() && getWorld()->getPlayer()->getY()==this->getY())
    {
        getWorld()->increaseScore(500);
        this->setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->playerRestoreHitPoints();
    }
}

bool RestoreHealthGoodie::blocksPlayer()
{
    return false;
}

bool RestoreHealthGoodie::blocksBoulder()
{
    return true;
}

void RestoreHealthGoodie::isDamaged()
{
    return;
}

RestoreHealthGoodie::~RestoreHealthGoodie()
{
    
}

AmmoGoodie::AmmoGoodie(int startX, int startY, StudentWorld* world)
:Actor(IID_AMMO, startX, startY, world)
{
    setName("Ammo Goodie");
    setVisible(true);
}

void AmmoGoodie::doSomething()
{
    //Check to see if AmmoGoodie isn't alive
    if(alive()==false)
    {
        return;
    }
    
    if(getWorld()->getPlayer()->getX()==this->getX() && getWorld()->getPlayer()->getY()==this->getY())
    {
        getWorld()->increaseScore(100);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->playerIncreaseAmmo();
    }
}

bool AmmoGoodie::blocksPlayer()
{
    return false;
}

bool AmmoGoodie::blocksBoulder()
{
    return true;
}

void AmmoGoodie::isDamaged()
{
    return;
}

AmmoGoodie::~AmmoGoodie()
{
    
}

Robot::Robot(int imageID, int startX, int startY, StudentWorld* world, int initialHitPoints)
:Actor(imageID, startX, startY, world)
{
    m_robotHitPoints=initialHitPoints;
    m_tickMovement=setTickMovement();
    m_tickCounter=0;
    setName("Robot");
    setTickMovement();
    //Don't need to do any other initialization in here because Robot is a base class for all the Robots in the game
}

bool Robot::blocksPlayer()
{
    return true;
}

bool Robot::blocksBoulder()
{
    return true;
}

void Robot::isDamaged()
{
    decrementHitPoints();
    if(m_robotHitPoints>0)
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    else
    {
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(100);
    }
}

int Robot::getHitPoints()
{
    return m_robotHitPoints;
}

void Robot::decrementHitPoints()
{
    m_robotHitPoints=m_robotHitPoints-2;
}

int Robot::setTickMovement()
{
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current level number (0, 1, 2, etc.)
    if (ticks < 3)
    {
        ticks=3; // no SnarlBot moves more frequently than this
    }
    return ticks;
}

int Robot::getTickMovement() const
{
    return m_tickMovement;
}

void Robot::resetTickCounter()
{
    m_tickCounter=0;
}

int Robot::getTickCounter() const
{
    return m_tickCounter;
}

void Robot::incrementTickCounter()
{
    m_tickCounter++;
}

void Robot::fire()
{
    int x=getX();
    int y=getY();
    Direction dir=getDirection();
    
    if(dir==up)
    {
        y=y+1;
    }
    else if(dir==right)
    {
        x=x+1;
    }
    else if(dir==down)
    {
        y=y-1;
    }
    else if(dir==left)
    {
        x=x-1;
    }
    
    Actor* bullet = new Bullet(x, y, getWorld(), dir);
    getWorld()->addActor(bullet);
    getWorld()->playSound(SOUND_ENEMY_FIRE);
}

Robot::~Robot()
{
    
}

SnarlBot::SnarlBot(int startX, int startY, StudentWorld* world, std::string type)
:Robot(IID_SNARLBOT, startX, startY, world, 10)
{
    if(type=="Horizontal")
    {
        setDirection(right);
    }
    else if(type=="Vertical")
    {
        setDirection(down);
    }
    m_type=type;
    setVisible(true);
}

void SnarlBot::doSomething()
{
    //Check to see if SnarlBot isn't alive
    if(alive()==false)
    {
        return;
    }
    
    //Check to see if SnarlBot should rest for this tick
    if(getTickCounter()<getTickMovement())
    {
        incrementTickCounter();
        return;
    }
    
    else if(getTickCounter()==getTickMovement())
    {
        Robot::resetTickCounter();
        //Check first to see if SnarlBot can fire
        
        bool foundObstacle=false;
        bool bulletFired;
        //Check for Horizontal type, same rows, then check for obstacles in the way
        if(m_type=="Horizontal" && getWorld()->getPlayer()->getY()==this->getY())
        {
            if((getX() < getWorld()->getPlayer()->getX() && getDirection()==right) || (getX() > getWorld()->getPlayer()->getX() && getDirection()==left))
            {
                if(getDirection()==right)
                {
                    for(int x=getX()+1; x<getWorld()->getPlayer()->getX(); x++)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(x, getY(), getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
                else if(getDirection()==left)
                {
                    for(int x=getX()-1; x>getWorld()->getPlayer()->getX(); x--)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(x, getY(), getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
            }
        }
        //Check for Vertical type, same columns, then check for obstacles in the way
        else if(m_type=="Vertical" && getWorld()->getPlayer()->getX()==this->getX())
        {
            if((getY() < getWorld()->getPlayer()->getY() && getDirection()==up) || (getY() > getWorld()->getPlayer()->getY() && getDirection()==down))
            {
                if(getDirection()==up)
                {
                    for(int y=getY()+1; y<getWorld()->getPlayer()->getY(); y++)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(getX(), y, getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
                else if(getDirection()==down)
                {
                    for(int y=getY()-1; y>getWorld()->getPlayer()->getY(); y--)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(getX(), y, getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
            }
        }
        
        if(bulletFired==false)
        {
            Direction dir=getDirection();
            
            if(dir==right)
            {
                Actor* actor=getAnActorAtTheProposedLocation(getX()+1, getY(), getWorld()->getActors());
                if(actor!=nullptr && (actor->getName()=="Player" || actor->getName()=="Boulder" || actor->getName()=="Wall" || actor->getName()=="Hole"|| actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                {
                    setDirection(left);
                }
                else
                {
                    moveTo(getX()+1, getY());
                }
            }
            
            if(dir==left)
            {
                Actor* actor=getAnActorAtTheProposedLocation(getX()-1, getY(), getWorld()->getActors());
                if(actor!=nullptr && (actor->getName()=="Player" || actor->getName()=="Boulder" || actor->getName()=="Wall" || actor->getName()=="Hole"|| actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                {
                    setDirection(right);
                }
                else
                {
                    moveTo(getX()-1, getY());
                }
            }
            
            if(dir==up)
            {
                Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()+1, getWorld()->getActors());
                if(actor!=nullptr && (actor->getName()=="Player" || actor->getName()=="Boulder" || actor->getName()=="Wall" || actor->getName()=="Hole"|| actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                {
                    setDirection(down);
                }
                else
                {
                    moveTo(getX(), getY()+1);
                }
            }
            
            if(dir==down)
            {
                Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()-1, getWorld()->getActors());
                if(actor!=nullptr && (actor->getName()=="Player" || actor->getName()=="Boulder" || actor->getName()=="Wall" || actor->getName()=="Hole"|| actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                {
                    setDirection(up);
                }
                else
                {
                    moveTo(getX(), getY()-1);
                }
            }
        }
    }
}

SnarlBot::~SnarlBot()
{
    
}

KleptoBot::KleptoBot(int startX, int startY, StudentWorld* world, int image, int hitPoints)
:Robot(image, startX, startY, world, hitPoints)
{
    m_pickedUpGoodie="";
    m_distanceBeforeTurning=generateDistanceBeforeTurning();
    m_distanceCounter=0;
    setDirection(right);
    setVisible(true);
}

void KleptoBot::doSomething()
{
    //Check to see if KleptoBot isn't alive
    if(alive()==false)
    {
        return;
    }
    
    //Check to see if KleptoBot should rest for this tick
    if(getTickCounter()<getTickMovement())
    {
        incrementTickCounter();
        return;
    }
    
    else if(getTickCounter()==getTickMovement())
    {
        Robot::resetTickCounter();
        
        //If the KleptoBot is on the same square as a Goodie
        Actor* possibleGoodie=getGoodieActor(getX(), getY(), getWorld()->getActors());
        if(possibleGoodie!=nullptr)
        {
            if(m_pickedUpGoodie=="")
            {
                int willPickUpChance=rand()%10;
                if(willPickUpChance==1)
                {
                    m_pickedUpGoodie=possibleGoodie->getName();
                    possibleGoodie->setDead();
                    getWorld()->playSound(SOUND_ROBOT_MUNCH);
                    return;
                }
            }
        }
        //It didn't find itself on a square with a Goodie
        else
        {
            noGoodieFound();
        }
    }
}

int KleptoBot::generateDistanceBeforeTurning()
{
    return rand()%6+1;
}

int KleptoBot::getDistanceBeforeTurning()
{
    return m_distanceBeforeTurning;
}

int KleptoBot::getDistanceCounter()
{
    return m_distanceCounter;
}

Actor* KleptoBot::getGoodieActor(int x, int y, const std::vector<Actor*> &actors)
{
    vector<Actor*>::const_iterator it;
    it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->getX()==x && (*it)->getY()==y && ((*it)->getName()=="Extra Life Goodie" || (*it)->getName()=="Restore Health Goodie" || (*it)->getName()=="Ammo Goodie"))
        {
            return *it;
        }
        it++;
    }
    //nullptr indicates no Goodies on the KleptoBot's spot
    return nullptr;
}

void KleptoBot::noGoodieFound()
{
    bool encounteredObstruction=false;
    if(m_distanceCounter<m_distanceBeforeTurning)
    {
        Direction dir=getDirection();
        if(dir==right)
        {
            Actor* actor=getAnActorAtTheProposedLocation(getX()+1, getY(), getWorld()->getActors());
            if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
            {
                moveTo(getX()+1, getY());
                m_distanceCounter++;
            }
            else
            {
                encounteredObstruction=true;
            }
        }
        
        else if(dir==left)
        {
            Actor* actor=getAnActorAtTheProposedLocation(getX()-1, getY(), getWorld()->getActors());
            if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
            {
                moveTo(getX()-1, getY());
                m_distanceCounter++;
            }
            else
            {
                encounteredObstruction=true;
            }
        }
        
        if(dir==up)
        {
            Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()+1, getWorld()->getActors());
            if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
            {
                moveTo(getX(), getY()+1);
                m_distanceCounter++;
            }
            else
            {
                encounteredObstruction=true;
            }
        }
        
        if(dir==down)
        {
            Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()-1, getWorld()->getActors());
            if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
            {
                moveTo(getX(), getY()-1);
                m_distanceCounter++;
            }
            else
            {
                encounteredObstruction=true;
            }
        }
    }
    else if(m_distanceCounter==m_distanceBeforeTurning || encounteredObstruction==true)
    {
        m_distanceBeforeTurning=generateDistanceBeforeTurning();
        int dir=rand()%4;
        Direction dirToBeConsidered;
        if(dir==0)
        {
            dirToBeConsidered=up;
        }
        else if(dir==1)
        {
            dirToBeConsidered=down;
        }
        else if(dir==2)
        {
            dirToBeConsidered=left;
        }
        else if(dir==3)
        {
            dirToBeConsidered=right;
        }
        Direction directionArray[4]={up, down, left, right};
        
        //Consider moving in direction dirToBeConsidered
        checkAndMove(dirToBeConsidered);
        
        //Remove from directionArray whichever direction you considered first because it didn't work since there was an obstruction and you need to check the adjacent squares in the other three directions
        Direction dirs[3];
        int x=0;
        for(int i=0; i<4; i++)
        {
            if(directionArray[i]!=dirToBeConsidered)
            {
                dirs[x]=directionArray[i];
                x++;
            }
        }
        
        for(int i=0; i<3; i++)
        {
            checkAndMove(dirs[i]);
        }
        
        //If all the adjacent directions fail, set current direction to dirToBeConsidered
        setDirection(dirToBeConsidered);
        return;
    }
}

void KleptoBot::checkAndMove(Direction dir)
{
    if(dir==right)
    {
        Actor* actor=getAnActorAtTheProposedLocation(getX()+1, getY(), getWorld()->getActors());
        if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
        {
            setDirection(right);
            moveTo(getX()+1, getY());
            m_distanceCounter=0;
            return;
        }
    }
    if(dir==left)
    {
        Actor* actor=getAnActorAtTheProposedLocation(getX()-1, getY(), getWorld()->getActors());
        if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
        {
            setDirection(left);
            moveTo(getX()-1, getY());
            m_distanceCounter=0;
            return;
        }
    }
    if(dir==up)
    {
        Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()+1, getWorld()->getActors());
        if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
        {
            setDirection(up);
            moveTo(getX(), getY()+1);
            m_distanceCounter=0;
            return;
        }
    }
    if(dir==down)
    {
        Actor* actor=getAnActorAtTheProposedLocation(getX(), getY()-1, getWorld()->getActors());
        if((actor!=nullptr && (actor->getName()!="Player" && actor->getName()!="Boulder" && actor->getName()!="Wall" && actor->getName()!="Hole"&& actor->getName()!="Robot" && actor->getName()!="Robot Factory")) || actor==nullptr)
        {
            setDirection(down);
            moveTo(getX(), getY()-1);
            m_distanceCounter=0;
            return;
        }
    }
}

void KleptoBot::reDisplayGoodie()
{
    if(m_pickedUpGoodie=="Extra Life Goodie")
    {
        Actor* extraLife = new ExtraLifeGoodie(getX(), getY(), getWorld());
        getWorld()->addActor(extraLife);
    }
    else if(m_pickedUpGoodie=="Restore Health Goodie")
    {
        Actor* restoreHealth = new RestoreHealthGoodie(getX(), getY(), getWorld());
        getWorld()->addActor(restoreHealth);
    }
    else if(m_pickedUpGoodie=="Ammo Goodie")
    {
        Actor* ammo = new AmmoGoodie(getX(), getY(), getWorld());
        getWorld()->addActor(ammo);
    }
}

void KleptoBot::isDamaged(int pointsAwarded)
{
    decrementHitPoints();
    if(getHitPoints()>0)
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    else
    {
        reDisplayGoodie();
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(pointsAwarded);
    }
}

KleptoBot::~KleptoBot()
{
    
}

AngryKleptoBot::AngryKleptoBot(int startX, int startY, StudentWorld* world)
:KleptoBot(startX, startY, world, IID_ANGRY_KLEPTOBOT, 8)
{
    setDirection(right);
    setVisible(true);
}

void AngryKleptoBot::doSomething()
{
    //Check to see if AngryKleptoBot isn't alive
    if(alive()==false)
    {
        return;
    }
    
    //Check to see if KleptoBot should rest for this tick
    if(getTickCounter()<getTickMovement())
    {
        incrementTickCounter();
        return;
    }
    
    else if(getTickCounter()==getTickMovement())
    {
        resetTickCounter();
        
        //Check first to see if AngryKleptoBot can fire
        
        bool foundObstacle=false;
        bool bulletFired;
        //Check for Horizontal type, same rows, then check for obstacles in the way
        if((getDirection()==left || getDirection()==right) && getWorld()->getPlayer()->getY()==this->getY())
        {
            if((getX() < getWorld()->getPlayer()->getX() && getDirection()==right) || (getX() > getWorld()->getPlayer()->getX() && getDirection()==left))
            {
                if(getDirection()==right)
                {
                    for(int x=getX()+1; x<getWorld()->getPlayer()->getX(); x++)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(x, getY(), getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
                else if(getDirection()==left)
                {
                    for(int x=getX()-1; x>getWorld()->getPlayer()->getX(); x--)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(x, getY(), getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
            }
        }
        //Check for Vertical type, same columns, then check for obstacles in the way
        else if((getDirection()==up || getDirection()==down) && getWorld()->getPlayer()->getX()==this->getX())
        {
            if((getY() < getWorld()->getPlayer()->getY() && getDirection()==up) || (getY() > getWorld()->getPlayer()->getY() && getDirection()==down))
            {
                if(getDirection()==up)
                {
                    for(int y=getY()+1; y<getWorld()->getPlayer()->getY(); y++)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(getX(), y, getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
                else if(getDirection()==down)
                {
                    for(int y=getY()-1; y>getWorld()->getPlayer()->getY(); y--)
                    {
                        Actor* actor=getAnActorAtTheProposedLocation(getX(), y, getWorld()->getActors());
                        if(actor!=nullptr && (actor->getName()=="Wall" || actor->getName()=="Boulder" || actor->getName()=="Robot" || actor->getName()=="Robot Factory"))
                        {
                            foundObstacle=true;
                            break;
                        }
                    }
                    if(foundObstacle==false)
                    {
                        fire();
                        bulletFired=true;
                    }
                }
            }
        }
        if(bulletFired==false)
        {
            noGoodieFound();
        }
    }
}

void AngryKleptoBot::isDamaged()
{
    KleptoBot::isDamaged(20);
}

KleptoBotFactory::KleptoBotFactory(int startX, int startY, StudentWorld* world, std::string type)
:Actor(IID_ROBOT_FACTORY, startX, startY, world)
{
    m_type=type;
    setName("Robot Factory");
    setVisible(true);
}

void KleptoBotFactory::doSomething()
{
    int kleptoBotsInArea=countKleptoBots(getX(), getY(), getWorld()->getActors());
    Actor* possibleKleptoBotOnSquare=nullptr;
    if(kleptoBotsInArea<3 && KleptoBotOnSquare(getX(), getY(), getWorld()->getActors(), possibleKleptoBotOnSquare)==false)
    {
        int kleptoBotBornChance=rand()%50;
        if(kleptoBotBornChance==1)
        {
            if(m_type=="Regular Factory")
            {
                Actor* kleptoBot = new KleptoBot(getX(), getY(), getWorld());
                getWorld()->addActor(kleptoBot);
                getWorld()->playSound(SOUND_ROBOT_BORN);
            }
            else if(m_type=="Angry KleptoBot Factory")
            {
                Actor* kleptoBot = new AngryKleptoBot(getX(), getY(), getWorld());
                getWorld()->addActor(kleptoBot);
                getWorld()->playSound(SOUND_ROBOT_BORN);
            }
        }
    }
}

int KleptoBotFactory::countKleptoBots(int startX, int startY, const std::vector<Actor*> &actors)
{
    int count=0;
    //Count all KleptoBots upward
    for(int y=startY; y<startY+3 || y<VIEW_HEIGHT; y++)
    {
        vector<Actor*>::const_iterator it;
        it=actors.begin();
        while(it!=actors.end())
        {
            if((*it)->getX()==startX && (*it)->getY()==y)
            {
                Actor* actor=*it;
                KleptoBot* kleptoBot = dynamic_cast<KleptoBot*>(actor);
                if(kleptoBot!=nullptr)
                {
                    count++;
                }
            }
            it++;
        }
    }
    
    //Count all KleptoBots downward
    for(int y=startY; y>startY-3 || y>0; y--)
    {
        vector<Actor*>::const_iterator it;
        it=actors.begin();
        while(it!=actors.end())
        {
            if((*it)->getX()==startX && (*it)->getY()==y)
            {
                Actor* actor=*it;
                KleptoBot* kleptoBot = dynamic_cast<KleptoBot*>(actor);
                if(kleptoBot!=nullptr)
                {
                    count++;
                }
            }
            it++;
        }
    }
    
    //Count all KleptoBots leftward
    for(int x=startX; x>startX-3 || x>0; x--)
    {
        vector<Actor*>::const_iterator it;
        it=actors.begin();
        while(it!=actors.end())
        {
            if((*it)->getX()==x && (*it)->getY()==startY)
            {
                Actor* actor=*it;
                KleptoBot* kleptoBot = dynamic_cast<KleptoBot*>(actor);
                if(kleptoBot!=nullptr)
                {
                    count++;
                }
            }
            it++;
        }
    }
    
    //Count all KleptoBots rightward
    for(int x=startX; x<startX+3 || x<VIEW_WIDTH; x++)
    {
        vector<Actor*>::const_iterator it;
        it=actors.begin();
        while(it!=actors.end())
        {
            if((*it)->getX()==x && (*it)->getY()==startY)
            {
                Actor* actor=*it;
                KleptoBot* kleptoBot = dynamic_cast<KleptoBot*>(actor);
                if(kleptoBot!=nullptr)
                {
                    count++;
                }
            }
            it++;
        }
    }
    return count;
}

AngryKleptoBot::~AngryKleptoBot()
{
    
}

bool KleptoBotFactory::KleptoBotOnSquare(int startX, int startY, const std::vector<Actor*> &actors, Actor* &actorAtLocation)
{
    vector<Actor*>::const_iterator it;
    it=actors.begin();
    while(it!=actors.end())
    {
        if((*it)->getX()==startX && (*it)->getY()==startY)
        {
            Actor* actor=*it;
            KleptoBot* kleptoBot = dynamic_cast<KleptoBot*>(actor);
            if(kleptoBot!=nullptr)
            {
                actorAtLocation=actor;
                return true;
            }
        }
        it++;
    }
    return false;
}

bool KleptoBotFactory::blocksPlayer()
{
    return true;
}

bool KleptoBotFactory::blocksBoulder()
{
    return true;
}

void KleptoBotFactory::isDamaged()
{
    Actor* possibleKleptoBotOnFactory=nullptr;
    if(KleptoBotOnSquare(getX(), getY(), getWorld()->getActors(), possibleKleptoBotOnFactory)==true)
    {
        possibleKleptoBotOnFactory->isDamaged();
    }
}

KleptoBotFactory::~KleptoBotFactory()
{
    
}