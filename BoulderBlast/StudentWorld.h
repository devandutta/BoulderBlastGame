#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Level.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        m_bonusPoints=1000;
        levelLoadStausCode=0;
        m_numJewels=0;
	}
    
    virtual int init()
    {
        //Load level data and create representations
        m_bonusPoints=1000;
        levelLoadStausCode=0;
        finishedLevel=false;
        m_numJewels=0;
        
        loadLevelData(levelLoadStausCode);
        
        //After level 99, does the framework move the level number up to 100, indicating that the level just completed is 99?
        if(levelLoadStausCode==1 || getLevel()==100)
        {
            return GWSTATUS_PLAYER_WON;
        }
        
        if(levelLoadStausCode==2)
        {
            return GWSTATUS_LEVEL_ERROR;
        }

        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move();

    virtual void cleanUp();
    
    void decrementLevelBonusPoints();
    
    std::vector<Actor*> getActors() const;
    
    void addActor(Actor* toAdd);
    
    void loadLevelData(int& levelLoadStatusCode);
    
    Actor* getPlayer() const;
    
    void didCompleteLevel();
    
    int getBonusPoints();
    
    int getNumJewels() const;
    
    void decrementNumJewels();
    
    void playerRestoreHitPoints();
    
    void playerIncreaseAmmo();
    
    ~StudentWorld();

private:
    std::vector<Actor*> m_actors;
    int m_bonusPoints;
    Actor* m_player;
    int levelLoadStausCode;
    int m_numJewels;
    bool finishedLevel;
};

#endif // STUDENTWORLD_H_
