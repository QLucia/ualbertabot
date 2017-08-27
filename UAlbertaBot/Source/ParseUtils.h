#pragma once
#include <string>
#include <memory>
#include <BWAPI/Race.h>

namespace UAlbertaBot
{

class StrategyManager;

namespace ParseUtils
{
	/**
	 * Finds location of the configuration file.
	 */
	std::string FindConfigurationLocation(const std::string & filename);
    void ParseConfigFile(const std::string & filename, bool& configFileFound, bool& configFileParsed);
    void ParseStrategy(const std::string & filename, std::shared_ptr<StrategyManager> strategyManager);

    std::string ReadFile(const std::string & filename);
}
}