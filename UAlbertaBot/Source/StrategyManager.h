#pragma once

#include "Common.h"
#include <BWAPI/Player.h>
#include "BuildOrder.h"
#include "Strategy.h"
#include "UnitInfoManager.h"
#include "BaseLocationManager.h"
#include "Logger.h"

namespace UAlbertaBot
{

class StrategyManager
{
    std::map<std::string,Strategy> _strategies;
    int                             _totalGamesPlayed;
    BuildOrder                      _emptyBuildOrder;
	const UnitInfoManager&			_unitInfo;
	std::string						_strategyName;
	const AKBot::OpponentView& _opponentView;
	const BaseLocationManager& _bases;
	const AKBot::Logger& _logger;

    void        writeResults();
    const int   getScore(BWAPI::Player player) const;
    const bool  shouldExpandNow(int currentFrame) const;
    const MetaPairVector getProtossBuildOrderGoal(int currentFrame) const;
    const MetaPairVector getTerranBuildOrderGoal(int currentFrame) const;
    const MetaPairVector getZergBuildOrderGoal(int currentFrame) const;

public:

    StrategyManager(
		std::string strategyName,
		const AKBot::OpponentView& opponentView,
		const UnitInfoManager & unitInfo,
		const BaseLocationManager& bases,
		const AKBot::Logger& logger);

    void update();
    void onEnd(const bool isWinner);
    void addStrategy(const std::string & name,Strategy & strategy);
    void setLearnedStrategy();
	void setPreferredStrategy(std::string strategy);
    void readResults();
    const MetaPairVector getBuildOrderGoal(int currentFrame) const;
    const BuildOrder & getOpeningBookBuildOrder() const;
};
}