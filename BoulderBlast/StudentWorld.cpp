#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::move()
{
    //Update the status text
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    
    //Calculate health
    double preFormattedHealth=m_player->getHitPoints()/20.0*100;
    int health = preFormattedHealth;  //Will truncate decimals
    
    //Get ammo
    int ammo = m_player->getAmmo();
    int bonus = m_bonusPoints;
    
    ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(7) << score << "  ";
    oss << "Level: " << setw(2) << level << "  ";
    oss.fill(' ');
    oss << "Lives: " << setw(2) << lives << "  ";
    oss << "Health: " << setw(3) << health << "%  ";
    oss << "Ammo: " << setw(3) << ammo << "  ";
    oss << "Bonus: " << setw(4) << bonus;
    this->setGameStatText(oss.str());
    
    //Ask all actors in m_actors to do something
    for(int x=0; x<m_actors.size(); x++)
    {
        if(m_actors[x]->alive()==true)
        {
            m_actors[x]->doSomething();
            if(m_player->alive()==false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    //If the Player dies, return GWSTATUS_PLAYER_DIED
    if(m_player->alive()==false)
    {
        return GWSTATUS_PLAYER_DIED;
    }
    
    //If the Player wins the level, return GWSTATUS_FINISHED_LEVEL
    if(finishedLevel==true)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //Remove any dead actors
    vector<Actor*>::iterator it;
    it=m_actors.begin();
    while(it!=m_actors.end())
    {
        if((*it)->alive()==false)
        {
            delete *it;
            m_actors.erase(it);
            it=m_actors.begin();
        }
        else
        {
            it++;
        }
    }
    
    //Decrement level's bonus points if bonus points > 0
    if(m_bonusPoints>0)
    {
        decrementLevelBonusPoints();
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //Deallocate all Actors
    for(int x=0; x<m_actors.size(); x++)
    {
        delete m_actors[x];
    }
    //Empty their pointers from m_actors
    vector<Actor*>::iterator it;
    it=m_actors.begin();
    while(it!=m_actors.end() && m_actors.size()>0)
    {
        m_actors.erase(it);
        it=m_actors.begin();
    }
}

void StudentWorld::decrementLevelBonusPoints()
{
    m_bonusPoints--;
}

std::vector<Actor*> StudentWorld::getActors() const
{
    return m_actors;
}

void StudentWorld::didCompleteLevel()
{
    finishedLevel=true;
}

void StudentWorld::addActor(Actor* toAdd)
{
    m_actors.push_back(toAdd);
}

void StudentWorld::loadLevelData(int& levelLoadStatusCode)
{
    Level lev(assetDirectory());
    ostringstream oss;
    int level=getLevel();
    oss<<"level0" << level <<".dat";
    Level::LoadResult result = lev.loadLevel(oss.str());
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find level data file\n";
        levelLoadStausCode=1;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted\n";
        levelLoadStausCode=2;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for(int x=0; x<VIEW_WIDTH; x++)
        {
            for(int y=0; y<VIEW_HEIGHT; y++)
            {
                Level::MazeEntry ge = lev.getContentsOf(x, y);
                switch(ge)
                {
                    case Level::player:
                    {
                        m_player = new Player(x, y, this);
                        m_actors.push_back(m_player);
                        break;
                    }
                    case Level::wall:
                    {
                        Actor* wall = new Wall(x,y, this);
                        m_actors.push_back(wall);
                        break;
                    }
                    case Level::boulder:
                    {
                        Actor* boulder = new Boulder(x, y, this);
                        m_actors.push_back(boulder);
                        break;
                    }
                    case Level::hole:
                    {
                        Actor* hole = new Hole(x, y, this);
                        m_actors.push_back(hole);
                        break;
                    }
                    case Level::jewel:
                    {
                        Actor* jewel = new Jewel(x, y, this);
                        m_actors.push_back(jewel);
                        m_numJewels++;
                        break;
                    }
                    case Level::exit:
                    {
                        Actor* exit = new Exit(x, y, this);
                        m_actors.push_back(exit);
                        break;
                    }
                    case Level::extra_life:
                    {
                        Actor* extraLife = new ExtraLifeGoodie(x, y, this);
                        m_actors.push_back(extraLife);
                        break;
                    }
                    case Level::restore_health:
                    {
                        Actor* restoreHealth = new RestoreHealthGoodie(x, y, this);
                        m_actors.push_back(restoreHealth);
                        break;
                    }
                    case Level::ammo:
                    {
                        Actor* ammo = new AmmoGoodie(x, y, this);
                        m_actors.push_back(ammo);
                        break;
                    }
                    case Level::horiz_snarlbot:
                    {
                        Actor* horizontalSnarlBot = new SnarlBot(x, y, this, "Horizontal");
                        m_actors.push_back(horizontalSnarlBot);
                        break;
                    }
                    case Level::vert_snarlbot:
                    {
                        Actor* verticalSnarlBot = new SnarlBot(x, y, this, "Vertical");
                        m_actors.push_back(verticalSnarlBot);
                        break;
                    }
                    case Level::kleptobot_factory:
                    {
                        KleptoBotFactory* kleptoBotFactory = new KleptoBotFactory(x, y, this, "Regular Factory");
                        m_actors.push_back(kleptoBotFactory);
                        break;
                    }
                    case Level::angry_kleptobot_factory:
                    {
                        KleptoBotFactory* kleptoBotFactory = new KleptoBotFactory(x, y, this, "Angry KleptoBot Factory");
                        m_actors.push_back(kleptoBotFactory);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

StudentWorld::~StudentWorld()
{
    //Deallocate all Actors
    for(int x=0; x<m_actors.size(); x++)
    {
        delete m_actors[x];
    }
    //Empty them from the vector
    vector<Actor*>::iterator it;
    it=m_actors.begin();
    while(it!=m_actors.end() && m_actors.size()>0)
    {
        m_actors.erase(it);
        it=m_actors.begin();
    }
}

Actor* StudentWorld::getPlayer() const
{
    return m_player;
}

int StudentWorld::getBonusPoints()
{
    return m_bonusPoints;
}

int StudentWorld::getNumJewels() const
{
    return m_numJewels;
}

void StudentWorld::decrementNumJewels()
{
    m_numJewels--;
}

void StudentWorld::playerRestoreHitPoints()
{
    Player* player = dynamic_cast<Player*>(m_player);
    player->restoreHealthPoints();
}

void StudentWorld::playerIncreaseAmmo()
{
    Player* player = dynamic_cast<Player*>(m_player);
    player->addAmmo();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
