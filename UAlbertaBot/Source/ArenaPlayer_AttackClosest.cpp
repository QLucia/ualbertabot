#include "ArenaPlayer_AttackClosest.h"
#include "Micro.h"
#include "UnitUtil.h"
#include <BWAPI/Game.h>
#include <BWAPI/Player.h>
#include <BWAPI/Unitset.h>
#include "BWAPIOpponentView.h"

using namespace AKBot;
using namespace UAlbertaBot;

ArenaPlayer_AttackClosest::ArenaPlayer_AttackClosest()
{
    _name = "ArenaPlayer_AttackClosest";
}

void ArenaPlayer_AttackClosest::onStart()
{

}

void ArenaPlayer_AttackClosest::onFrame(int currentFrame)
{
    for (auto unit : BWAPI::Broodwar->self()->getUnits())
    {
		auto closestUnit = getClosestEnemyUnit(unit);
		if (closestUnit != nullptr)
		{
			Micro::SmartAttackUnit(unit, closestUnit, currentFrame);
		}
    }
}

BWAPI::Unit ArenaPlayer_AttackClosest::getClosestEnemyUnit(BWAPI::Unit ourUnit) const
{
    BWAPI::Unit closestUnit = nullptr;
	double closestDist = std::numeric_limits<double>::max();

	for (auto unit : UnitUtil::getEnemyUnits(std::make_shared<BWAPIOpponentView>()))
	{
		double dist = unit->getDistance(ourUnit);
		if (!closestUnit || dist < closestDist)
		{
			closestUnit = unit;
			closestDist = dist;
		}
	}

	return closestUnit;
}

void ArenaPlayer_AttackClosest::onBattleBegin()
{

}

void ArenaPlayer_AttackClosest::onBattleEnd()
{

}