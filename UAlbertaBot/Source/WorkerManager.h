#pragma once

#include <Common.h>
#include "BuildingData.h"
#include "BuildingManager.h"
#include "WorkerData.h"
#include "OpponentView.h"

namespace UAlbertaBot
{
class WorkerManager
{
    WorkerData  workerData;
    BWAPI::Unit previousClosestWorker;
	const AKBot::OpponentView& _opponentView;

    void        setMineralWorker(BWAPI::Unit unit, int currentFrame);
    
    void        handleIdleWorkers(int currentFrame);
    void        handleGasWorkers(int currentFrame);
    void        handleMoveWorkers(int currentFrame);
    void        handleCombatWorkers(int currentFrame);
    void        handleRepairWorkers(int currentFrame);

public:

    WorkerManager(const AKBot::OpponentView& opponentView, const AKBot::Logger& logger);

    void        update(int currentFrame);
    void        onUnitDestroy(BWAPI::Unit unit, int currentFrame);
    void        onUnitMorph(BWAPI::Unit unit);
    void        onUnitShow(BWAPI::Unit unit);
    void        finishedWithWorker(BWAPI::Unit unit, int currentFrame);

    void        finishedWithCombatWorkers(int currentFrame);

    void        updateWorkerStatus(int currentFrame);
	const WorkerData& getWorkerData() const { return workerData; };

    int         getNumMineralWorkers();
    int         getNumGasWorkers();
    int         getNumIdleWorkers();
    void        setScoutWorker(BWAPI::Unit worker, int currentFrame);

    bool        isWorkerScout(BWAPI::Unit worker) const;
    bool        isFree(BWAPI::Unit worker) const;
    bool        isBuilder(BWAPI::Unit worker) const;

    BWAPI::Unit getBuilder(Building & b,bool setJobAsBuilder = true);
    BWAPI::Unit getMoveWorker(BWAPI::Position p);
    BWAPI::Unit getClosestDepot(BWAPI::Unit worker);
    BWAPI::Unit getGasWorker(BWAPI::Unit refinery);
    BWAPI::Unit getClosestEnemyUnit(BWAPI::Unit worker);
    BWAPI::Unit getClosestMineralWorkerTo(BWAPI::Unit enemyUnit);

    void        setBuildingWorker(BWAPI::Unit worker,Building & b);
    void        setRepairWorker(BWAPI::Unit worker,BWAPI::Unit unitToRepair, int currentFrame);
    void        stopRepairing(BWAPI::Unit worker, int currentFrame);
    void        setMoveWorker(int m,int g,BWAPI::Position p);
    void        setCombatWorker(BWAPI::Unit worker, int currentFrame);

    bool        willHaveResources(int mineralsRequired,int gasRequired,double distance);
    void        rebalanceWorkers(int currentFrame);

};

}