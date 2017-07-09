#pragma once

#include "Common.h"
#include "MicroManager.h"

namespace UAlbertaBot
{
	class BaseLocation;
	class BaseLocationManager;

class ScoutManager 
{
	const BaseLocationManager& _baseLocationManager;
	BWAPI::Unit     _workerScout;
    std::string     _scoutStatus;
	int             _numWorkerScouts;
	bool            _scoutUnderAttack;
    int             _currentRegionVertexIndex;
    int             _previousScoutHP;
	std::vector<BWAPI::Position>    _enemyRegionVertices;

	bool            enemyWorkerInRadius();
    bool            immediateThreat();
    int             getClosestVertexIndex(BWAPI::Unit unit);
    BWAPI::Position getFleePosition();
	BWAPI::Unit     getEnemyGeyser();
	BWAPI::Unit     closestEnemyWorker();
    //void            followPerimeter();
	void            moveScouts();
	bool exploreEnemyBases();
	bool allEnemyBasesExplored() const;
	void harrasEnemyBaseIfPossible(const BaseLocation * enemyBaseLocation, int scoutHP);
    void            drawScoutInformation(int x, int y);
    //void            calculateEnemyRegionVertices();


public:
    
	ScoutManager(const BaseLocationManager& baseLocationManager);

	void update();
    void setWorkerScout(BWAPI::Unit unit);

};
}