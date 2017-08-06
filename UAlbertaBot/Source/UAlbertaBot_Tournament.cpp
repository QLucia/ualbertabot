#include "Common.h"
#include "UAlbertaBot_Tournament.h"
#include "JSONTools.h"
#include "ParseUtils.h"
#include "UnitUtil.h"
#include "Global.h"
#include "BaseLocationManagerDebug.h"
#include "WorkerManagerDebug.h"
#include "MapToolsDebug.h"
#include "UnitInfoManagerDebug.h"
#include "GameCommanderDebug.h"
#include "Micro.h"

using namespace UAlbertaBot;
using namespace AKBot;

UAlbertaBot_Tournament::UAlbertaBot_Tournament(
	shared_ptr<AKBot::OpponentView> opponentView,
	unique_ptr<BaseLocationManager> baseLocationManager,
	unique_ptr<AutoObserver> autoObserver,
	shared_ptr<StrategyManager> strategyManager,
	shared_ptr<UnitInfoManager> unitInfoManager,
	shared_ptr<MapTools> mapTools,
	shared_ptr<WorkerManager> workerManager,
	shared_ptr<ScoutManager> scoutManager,
	shared_ptr<AKBot::Logger> logger)
	: _opponentView(opponentView)
	, _baseLocationManager(std::move(baseLocationManager))
	, _workerManager(workerManager)
	, _bossManager(opponentView)
	, _autoObserver(std::move(autoObserver))
	, _unitInfoManager(unitInfoManager)
	, _strategyManager(strategyManager)
	, _mapTools(mapTools)
	, _scoutManager(scoutManager)
	, _productionManager(opponentView, _bossManager, strategyManager, workerManager, unitInfoManager, _baseLocationManager, mapTools, logger)
	, _combatCommander(_baseLocationManager, opponentView, workerManager, unitInfoManager, mapTools, logger)
	, _gameCommander(opponentView, _bossManager, _combatCommander, scoutManager, _productionManager, workerManager)
{
	// parse the configuration file for the bot's strategies
	auto configurationFile = ParseUtils::FindConfigurationLocation(Config::ConfigFile::ConfigFileLocation);
	ParseUtils::ParseStrategy(configurationFile, _strategyManager);
}

UAlbertaBot_Tournament::~UAlbertaBot_Tournament()
{
}

// This gets called when the bot starts!
void UAlbertaBot_Tournament::onStart()
{
	// Initialize SparCraft, the combat simulation package
	SparCraft::init();
	auto configurationFile = ParseUtils::FindConfigurationLocation(Config::SparCraft::SparCraftConfigFile);
	SparCraft::AIParameters::Instance().parseFile(configurationFile);

	// Initialize BOSS, the Build Order Search System
	BOSS::init();

	// Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(Config::BWAPIOptions::SetLocalSpeed);
	BWAPI::Broodwar->setFrameSkip(Config::BWAPIOptions::SetFrameSkip);

	if (Config::BWAPIOptions::EnableCompleteMapInformation)
	{
		BWAPI::Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);
	}

	if (Config::BWAPIOptions::EnableUserInput)
	{
		BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
	}

	if (Config::BotInfo::PrintInfoOnStart)
	{
		BWAPI::Broodwar->printf("Hello! I am %s, written by %s", Config::BotInfo::BotName.c_str(), Config::BotInfo::Authors.c_str());
	}

	if (Config::Modules::UsingStrategyIO)
	{
		_strategyManager->readResults();
		_strategyManager->setLearnedStrategy();
	}

	_unitInfoManager->onStart();
	_mapTools->onStart();
	_baseLocationManager->onStart(_mapTools);
	_productionManager.onStart();
	_gameCommander.onStart();

	Micro::SetOnAttackUnit([this](const BWAPI::Unit&attacker, const BWAPI::Unit&target)
	{
		auto& canvas = this->getCanvas();
		Micro::drawAction(canvas, attacker->getPosition(), target->getPosition(), Micro::AttackUnitColor);
	});
	Micro::SetOnAttackMove([this](const BWAPI::Unit&attacker, const BWAPI::Position &targetPosition)
	{
		auto& canvas = this->getCanvas();
		Micro::drawAction(canvas, attacker->getPosition(), targetPosition, Micro::AttackMoveColor);
	});
	Micro::SetOnMove([this](const BWAPI::Unit&attacker, const BWAPI::Position &targetPosition)
	{
		auto& canvas = this->getCanvas();
		Micro::drawAction(canvas, attacker->getPosition(), targetPosition, Micro::MoveColor);
	});
	Micro::SetOnRepair([this](const BWAPI::Unit& unit, const BWAPI::Unit& target)
	{
		auto& canvas = this->getCanvas();
		Micro::drawAction(canvas, unit->getPosition(), target->getPosition(), Micro::RepairColor);
	});
	Micro::SetOnRightClick([this](const BWAPI::Unit& unit, const BWAPI::Unit& target)
	{
		auto& canvas = this->getCanvas();
		Micro::drawAction(canvas, unit->getPosition(), target->getPosition(), Micro::RightClickColor);
	});
}

void UAlbertaBot_Tournament::onEnd(bool isWinner) 
{
	_strategyManager->onEnd(isWinner);
}

const shared_ptr<UnitInfoManager> UAlbertaBot_Tournament::UnitInfo() const
{
    return _unitInfoManager;
}

AKBot::ScreenCanvas & UAlbertaBot::UAlbertaBot_Tournament::getCanvas()
{
	return _canvas;
}

void UAlbertaBot_Tournament::onFrame()
{
	auto enemy = Global::getEnemy();
	if (enemy == nullptr)
	{
		return;
	}

	auto currentFrame = BWAPI::Broodwar->getFrameCount();

	// update all of the internal information managers
    _mapTools->update(currentFrame);
	_strategyManager->update();
    _unitInfoManager->update();
    _workerManager->update(currentFrame);
    _baseLocationManager->update(_unitInfoManager);

    // update the game commander
	_gameCommander.update(currentFrame);
	_productionManager.update(currentFrame);
	_combatCommander.update(_gameCommander.getCombatUnits(), currentFrame);
	_scoutManager->update(currentFrame);

	// Draw debug information
	drawDebugInformation(_canvas);

    if (Config::Modules::UsingAutoObserver)
    {
        _autoObserver->onFrame(currentFrame);
    }
}

void UAlbertaBot_Tournament::drawDebugInformation(AKBot::ScreenCanvas& canvas)
{
	if (Config::Debug::DrawLastSeenTileInfo)
	{
		MapToolsDebug mapDebug(_mapTools, _baseLocationManager);
		mapDebug.drawLastSeen(canvas);
	}

	WorkerManagerDebug debug(_workerManager->getWorkerData());
	debug.draw(canvas);

	// draw the debug information for each base location
	BaseLocationManagerDebug baseLocationManagerDebug(_opponentView, _baseLocationManager, _mapTools);
	baseLocationManagerDebug.draw(canvas);

	UnitInfoManagerDebug unitInfoDebug(_opponentView, _unitInfoManager);
	unitInfoDebug.draw(canvas);

	GameCommanderDebug gameCommanderDebug(_gameCommander);
	gameCommanderDebug.draw(canvas);
}

void UAlbertaBot_Tournament::onUnitDestroy(BWAPI::Unit unit)
{
	auto currentFrame = BWAPI::Broodwar->getFrameCount();
	_workerManager->onUnitDestroy(unit, currentFrame);
	_unitInfoManager->onUnitDestroy(unit); 
    _gameCommander.onUnitDestroy(unit, currentFrame);
	_productionManager.onUnitDestroy(unit, currentFrame);
}

void UAlbertaBot_Tournament::onUnitMorph(BWAPI::Unit unit)
{
	_unitInfoManager->onUnitMorph(unit);
	_workerManager->onUnitMorph(unit);
    _gameCommander.onUnitMorph(unit);
}

void UAlbertaBot_Tournament::onSendText(std::string text) 
{ 
    BWAPI::Broodwar->sendText("%s", text.c_str());
}

void UAlbertaBot_Tournament::onUnitCreate(BWAPI::Unit unit)
{ 
	_unitInfoManager->onUnitCreate(unit); 
    _gameCommander.onUnitCreate(unit);
}

void UAlbertaBot_Tournament::onUnitComplete(BWAPI::Unit unit)
{
	_unitInfoManager->onUnitComplete(unit);
    _gameCommander.onUnitComplete(unit);
}

void UAlbertaBot_Tournament::onUnitShow(BWAPI::Unit unit)
{ 
	_unitInfoManager->onUnitShow(unit);
	_workerManager->onUnitShow(unit);
    _gameCommander.onUnitShow(unit);
}

void UAlbertaBot_Tournament::onUnitHide(BWAPI::Unit unit)
{ 
	_unitInfoManager->onUnitHide(unit);
    _gameCommander.onUnitHide(unit);
}

void UAlbertaBot_Tournament::onUnitRenegade(BWAPI::Unit unit)
{ 
	_unitInfoManager->onUnitRenegade(unit);
    _gameCommander.onUnitRenegade(unit);
}