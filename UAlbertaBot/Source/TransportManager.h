#pragma once;

#include <Common.h>
#include "MicroManager.h"
#include "PlayerLocationProvider.h"

namespace UAlbertaBot
{
class MicroManager;

class TransportManager : public MicroManager
{
	std::vector<BWAPI::Position>    _mapEdgeVertices; 
	BWAPI::UnitInterface*	        _transportShip;
	int                             _currentRegionVertexIndex;
	BWAPI::Position					_minCorner;
	BWAPI::Position					_maxCorner;
	std::vector<BWAPI::Position>	_waypoints;
	BWAPI::Position					_to;
	BWAPI::Position					_from;
	AKBot::PlayerLocationProvider&	_locationProvider;

	void							calculateMapEdgeVertices();
	void							moveTransport();
	void							moveTroops();
	BWAPI::Position                 getFleePosition(int clockwise=1);
	void                            followPerimeter(int clockwise=1);
	void							followPerimeter(BWAPI::Position to, BWAPI::Position from);
	int                             getClosestVertexIndex(BWAPI::UnitInterface * unit);
	int								getClosestVertexIndex(BWAPI::Position p);
	std::pair<int, int>				findSafePath(BWAPI::Position from, BWAPI::Position to);
	
public:

	TransportManager(const AKBot::OpponentView& opponentView, const BaseLocationManager& bases, AKBot::PlayerLocationProvider& locationProvider);

	void							executeMicro(const std::vector<BWAPI::Unit> & targets);
	void							update();
	void							setTransportShip(BWAPI::UnitInterface * unit);
	void							setFrom(BWAPI::Position from);
	void							setTo(BWAPI::Position to);
	void							drawTransportInformation(AKBot::ScreenCanvas& canvas, int x, int y);
};
}