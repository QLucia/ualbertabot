#pragma once

#include "Common.h"
#include "MeleeManager.h"
#include "RangedManager.h"
#include "DetectorManager.h"
#include "TransportManager.h"
#include "SquadOrder.h"
#include "StrategyManager.h"
#include "CombatSimulation.h"
#include "TankManager.h"
#include "MedicManager.h"
#include "UnitHandler.h"
#include "MapTools.h"
#include "OpponentView.h"

namespace UAlbertaBot
{
	using namespace AKBot;
    
class Squad
{
    std::string         _name;
	std::set<BWAPI::Unit> _units;
	std::string         _regroupStatus;
    int                 _lastRetreatSwitch;
    bool                _lastRetreatSwitchVal;
    size_t              _priority;
	UnitHandler			_onRemoveHandler;
	const AKBot::OpponentView& _opponentView;
	const UnitInfoManager& _unitInfo;
	
	SquadOrder          _order;
	MeleeManager        _meleeManager;
	RangedManager       _rangedManager;
	DetectorManager     _detectorManager;
	TransportManager    _transportManager;
    TankManager         _tankManager;
    MedicManager        _medicManager;

	std::map<BWAPI::Unit, bool>	_nearEnemy;
	BWAPI::Position _lastRegroupPosition;
	bool			_needToRegroup;

	BWAPI::Unit		unitClosestToEnemy(std::function<int(const BWAPI::Position & src, const BWAPI::Position & dest)> distance);
	void                        updateUnits();
	void                        addUnitsToMicroManagers();
	void                        setNearEnemyUnits();
	void                        setAllUnits();
	
	bool                        unitNearEnemy(BWAPI::Unit unit);
	bool                        needsToRegroup(const MapTools& map);
	int                         squadUnitsNear(BWAPI::Position p);

public:

	Squad(const std::string & name, SquadOrder order, size_t priority, AKBot::PlayerLocationProvider& locationProvider, const AKBot::OpponentView& opponentView, const UnitInfoManager& unitInfo, const BaseLocationManager& bases);
    ~Squad();

	void                update(const MapTools& map);
	void                setSquadOrder(const SquadOrder & so);
	void                addUnit(BWAPI::Unit u);
	void                removeUnit(BWAPI::Unit u);
    bool                containsUnit(BWAPI::Unit u) const;
    bool                isEmpty() const;
    void                clear();
    size_t              getPriority() const;
    void                setPriority(const size_t & priority);

	// Get name of the squad.
    const std::string & getName() const;
	const std::string& getRegroupStatus() const;
	bool getNeedToRegroup() const;
	const MeleeManager getMeleeManager() const { return _meleeManager; }
	const RangedManager getRangedManager() const { return _rangedManager; }
	const TankManager getTankManager() const { return _tankManager; }
	const MedicManager getMedicManager() const { return _medicManager; }
	const DetectorManager getDetectorManager() const { return _detectorManager; }
	const TransportManager getTransportManager() const { return _transportManager; }

	/*
	 Calculate center of the squad.
	*/
	BWAPI::Position     calcCenter();
	BWAPI::Position     calcRegroupPosition();

	const std::set<BWAPI::Unit> &  getUnits() const;
	const SquadOrder &  getSquadOrder()	const;
	void onUnitRemoved(UnitHandler handler);
	bool isNearEnemy(const BWAPI::Unit& unit) const;

	BWAPI::Position getLastRegroupPosition() const { return _lastRegroupPosition; };
};
}