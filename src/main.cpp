#include "GameInfo.h"
#include "Group.h"
#include "Parser/GameParser.h"
#include "ShufflingTools.h"
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Print this message")
		("deck",
		 po::value<std::string>()->default_value("deck.txt"),
		 "Player's deck tile file")
		("table",
		 po::value<std::string>()->default_value("table.txt"),
		 "Table tile file")
		("time_limit",
		 po::value<int>()->default_value(120),
		 "Time limit (in seconds) to find a solution");

	po::variables_map opts;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), opts);
		po::notify(opts);
	}
	catch (const po::error& e)
	{
		std::cout << desc << "\n";
		return 1;
	}

	if (opts.count("help"))
	{
		std::cout << desc << "\n";
		return 1;
	}

	const std::string& deckFile = opts["deck"].as<std::string>();
	const std::string& tableFile = opts["table"].as<std::string>();
	const boost::optional<std::chrono::duration<int>> duration
		= opts["time_limit"].as<int>() > 0
			? boost::optional<std::chrono::duration<int>>(opts["time_limit"].as<int>())
			: boost::none;

	boost::optional<GameParser::Result> parsingResult
		= GameParser::parseGame(deckFile, tableFile);

	if (!parsingResult)
	{
		return 1;
	}

	ShufflingTools::markCompatibleTiles(parsingResult->allTiles());

	const std::vector<Group> allGroups
		= ShufflingTools::getAllPossibleGroups(parsingResult->allTiles());

	std::cout << "Found " << allGroups.size() << " groups!\n";

	// Record start time
	const auto start = std::chrono::high_resolution_clock::now();

	const boost::optional<GroupConfiguration> bestConfig
		= ShufflingTools::getBestConfiguration(
				allGroups,
				duration);

	const auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "Elapsed time: "
		<< std::chrono::duration_cast<std::chrono::seconds>(finish - start).count()
		<< "s\n";

	if (bestConfig)
	{
		std::cout << "Found configuration with score: "
			<< bestConfig->score() << "\n";
		std::cout << *bestConfig;
	}
	else
	{
		std::cout << "No valid configuration found! :(\n";
	}
}
