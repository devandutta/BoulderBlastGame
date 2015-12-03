#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <cstdlib>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* world);
    std::string getName() const;
    void setName(std::string name);
    virtual void doSomething()=0;
    bool alive();
    void setDead();
    Actor* getAnActorAtTheProposedLocation(int x, int y, const std::vector<Actor*> &actors);
    virtual bool blocksPlayer()=0;
    virtual bool blocksBoulder()=0;
    virtual void isDamaged()=0;
    StudentWorld* getWorld();
    virtual int getHitPoints() const;
    virtual int getAmmo() const;
    virtual ~Actor();
    
private:
    std::string m_name;
    bool m_alive;
    StudentWorld* m_studentWorld;
};

class Wall: public Actor
{
public:
    Wall(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~Wall();
    
private:
    
};

class Player: public Actor
{
public:
    Player(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    //Defined the next method as a pure virtual one in base class, so need a definition here
    virtual bool blocksPlayer();
    //Defined the next method as a pure virtual one in base class, so need a definition here
    virtual bool blocksBoulder();
    virtual void isDamaged();
    int getHitPoints() const;
    void restoreHealthPoints();
    int getAmmo() const;
    void addAmmo();
    virtual ~Player();
    
private:
    int m_hitPoints;
    int m_ammo;
};

class Boulder: public Actor
{
public:
    Boulder(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    void push(Direction dir);
    int getHitPoints() const;
    virtual ~Boulder();
    
private:
    int m_hitPoints;
};

class Bullet: public Actor
{
public:
    Bullet(int startX, int startY, StudentWorld* world, Direction dir);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    void checkLocation();
    virtual ~Bullet();
private:
    bool completedFirstTickAlready;
};

class Hole: public Actor
{
public:
    Hole(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    Actor* getPotentialBoulder(int x, int y, const std::vector<Actor*> &actors);
    virtual ~Hole();
private:
    
};

class Jewel: public Actor
{
public:
    Jewel(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~Jewel();
private:
    
};

class Exit: public Actor
{
public:
    Exit(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~Exit();
private:
    bool exitRevealed;
};

class ExtraLifeGoodie: public Actor
{
public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~ExtraLifeGoodie();
private:
    
};

class RestoreHealthGoodie: public Actor
{
public:
    RestoreHealthGoodie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~RestoreHealthGoodie();
};

class AmmoGoodie: public Actor
{
public:
    AmmoGoodie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~AmmoGoodie();
};

class Robot: public Actor
{
public:
    Robot(int imageID, int startX, int startY, StudentWorld* world, int initialHitPoints);
    //Inherit the abstract function doSomething() directly from Actor
    
    //All Robots will block the Player and Boulders, so make blocksPlayer() and blocksRobot() non virtual functions so that all Robots derived from Robot will block the Player and Boulders
    bool blocksPlayer();
    bool blocksBoulder();
    virtual void isDamaged();
    
    virtual int getHitPoints();
    virtual void decrementHitPoints();
    int setTickMovement();
    int getTickMovement() const;
    void resetTickCounter();
    int getTickCounter() const;
    void incrementTickCounter();
    void fire();
    virtual ~Robot();
private:
    int m_robotHitPoints;
    int m_tickMovement;
    int m_tickCounter;
};

class SnarlBot: public Robot
{
public:
    SnarlBot(int startX, int startY, StudentWorld* world, std::string type);
    virtual void doSomething();
    //Inherit the isDamaged() function from Robot class
    virtual ~SnarlBot();
    
private:
    std::string m_type;
};

class KleptoBot: public Robot
{
public:
    KleptoBot(int startX, int startY, StudentWorld* world, int image=IID_KLEPTOBOT, int hitPoints=5);
    virtual void doSomething();
    int generateDistanceBeforeTurning();
    int getDistanceBeforeTurning();
    int getDistanceCounter();
    Actor* getGoodieActor(int x, int y, const std::vector<Actor*> &actors);
    void noGoodieFound();
    void checkAndMove(Direction dir);
    virtual void reDisplayGoodie();
    virtual void isDamaged(int pointsAwarded=10);
    virtual ~KleptoBot();
    
private:
    std::string m_pickedUpGoodie;
    int m_distanceBeforeTurning;
    int m_distanceCounter;
};

class AngryKleptoBot: public KleptoBot
{
public:
    AngryKleptoBot(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual void isDamaged();
    virtual ~AngryKleptoBot();
    
private:
    
};

class KleptoBotFactory: public Actor
{
public:
    KleptoBotFactory(int startX, int startY, StudentWorld* world, std::string type);
    virtual void doSomething();
    int countKleptoBots(int startX, int startY, const std::vector<Actor*> &actors);
    bool KleptoBotOnSquare(int startX, int startY, const std::vector<Actor*> &actors, Actor* &actorAtLocation);
    virtual bool blocksPlayer();
    virtual bool blocksBoulder();
    virtual void isDamaged();
    virtual ~KleptoBotFactory();
private:
    std::string m_type;
};

#endif // ACTOR_H_
