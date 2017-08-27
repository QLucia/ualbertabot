#pragma once

#include "DebugInfoProvider.h"
#include "GameCommander.h"
#include "ScreenCanvas.h"

namespace AKBot
{
	using UAlbertaBot::GameCommander;

	class GameCommanderDebug : public DebugInfoProvider
	{
		shared_ptr<GameCommander> _gameCommander;
		shared_ptr<AKBot::Logger> _logger;
		const BotDebugConfiguration& _debugConfiguration;

		void drawGameInformation(ScreenCanvas& canvas, int x, int y) const;
	public:
		GameCommanderDebug(
			shared_ptr<GameCommander> gameCommander,
			shared_ptr<AKBot::Logger> logger,
			const BotDebugConfiguration& debugConfiguration);
		void draw(ScreenCanvas& canvas) override;
	};
}
