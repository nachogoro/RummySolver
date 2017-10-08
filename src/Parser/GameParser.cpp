#include "GameParser.h"
#include "GameInfo.h"
#include "DeckJokerTile.h"
#include "DeckRegularTile.h"
#include "TableJokerTile.h"
#include "TableRegularTile.h"
#include "ParsingUtils.h"
#include "TileColor.h"
#include "Tile.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

namespace
{
	uint16_t countWordsInFile(
			const boost::filesystem::path& file)
	{
		std::ifstream infile(file.string());
		std::istream_iterator<std::string> in{ infile }, end;
		return static_cast<uint16_t>(std::distance(in, end));
	}

	std::vector<std::reference_wrapper<const Tile>> getTableJokers(
			const std::vector<std::unique_ptr<Tile>>& tableTiles)
	{
		std::vector<std::reference_wrapper<const Tile>> result;
		for (const auto& tile : tableTiles)
		{
			if (tile->type() == TABLE_JOKER
					&& tile->asTableJokerTile().isLocked())
			{
				result.push_back(std::cref(*tile));
			}
		}
		return result;
	}

	std::vector<std::reference_wrapper<Tile>> mergeTiles(
			const std::vector<std::unique_ptr<Tile>>& deckTiles,
			const std::vector<std::unique_ptr<Tile>>& tableTiles)
	{
		std::vector<std::reference_wrapper<Tile>> result;

		for (const auto& deckTile : deckTiles)
		{
			result.push_back(std::ref(*deckTile));
		}

		for (const auto& tableTile : tableTiles)
		{
			result.push_back(std::ref(*tableTile));
		}

		return result;
	}

	boost::optional<std::vector<std::unique_ptr<Tile>>> parsePlayersDeck(
			const boost::filesystem::path& playersDeckFile)
	{
		std::ifstream input(playersDeckFile.string());
		if (input.fail())
		{
			std::cerr << "Cannot open " << playersDeckFile << "\n";
			return boost::none;
		}

		std::vector<std::unique_ptr<Tile>> tiles;
		int number;
		char colorLetter;
		std::string line;
		uint16_t id = 0;

		// Each line contains a single tile
		while(std::getline(input, line))
		{
			std::istringstream iss(line);
			if (!(iss >> colorLetter))
			{
				std::cerr << "Invalid line: " << line << " in file "
					<< playersDeckFile << "\n";
				return boost::none;
			}

			if (colorLetter == 'J')
			{
				tiles.push_back(std::unique_ptr<Tile>(
							new DeckJokerTile(id++)));
				continue;
			}

			if (!TileColor::letterToColor.count(colorLetter))
			{
				std::cerr << "Invalid color " << colorLetter << " in line "
					<< line << " in file " << playersDeckFile << "\n";
				return boost::none;
			}

			if (!(iss >> number))
			{
				std::cerr << "Invalid line: " << line << " in file "
					<< playersDeckFile << "\n";
				return boost::none;
			}

			tiles.push_back(std::unique_ptr<Tile>(
						new DeckRegularTile(
							number,
							TileColor::letterToColor.at(colorLetter),
							id++)));
		}

		return tiles;
	}

	boost::optional<std::vector<std::unique_ptr<Tile>>> parseTable(
			const boost::filesystem::path& tableFile,
			const std::vector<std::unique_ptr<Tile>>& deckTiles)
	{
		std::ifstream input(tableFile.string());
		if (input.fail())
		{
			std::cerr << "Cannot open " << tableFile << "\n";
			return boost::none;
		}

		boost::optional<std::vector<std::vector<ParsingUtils::TemporaryTile>>> tmpTiles
			= ParsingUtils::parseFile(input);

		if (!tmpTiles)
		{
			return boost::none;
		}

		boost::optional<std::vector<std::unique_ptr<Tile>>> tableTilesOpt
			= ParsingUtils::temporaryTilesToTiles(*tmpTiles, deckTiles);

		if (!tableTilesOpt)
		{
			return boost::none;
		}

		std::vector<std::unique_ptr<Tile>> tableTiles
			= std::move(*tableTilesOpt);

		// All jokers in the table are considered locked at this point
		std::vector<std::reference_wrapper<const Tile>> tableJokers
			= ::getTableJokers(tableTiles);

		if (tableJokers.size() == 0)
		{
			// Nothing else to do
			return tableTiles;
		}

		// If there are any tiles on the table that unlock a joker, that joker
		// is not considered unlocked
		for (const auto& generalJoker : tableJokers)
		{
			auto& joker = generalJoker.get().asTableJokerTile();
			for (const auto& generalTile : tableTiles)
			{
				if (generalTile->type() != TABLE_REGULAR)
				{
					continue;
				}

				auto& tile = generalTile->asTableRegularTile();
				if ((tile.color() & joker.lockedColors()) != 0
						&& tile.number() == joker.lockedNumber())
				{
					// Unlock tile
					(*tableTiles[joker.id()])
						= std::move(*TableJokerTile::unlocked(joker.id()));
				}
			}
		}

		// Re-evaluate what jokers are still locked
		tableJokers = ::getTableJokers(tableTiles);

		if (tableJokers.size() == 0)
		{
			// Nothing else to do
			return tableTiles;
		}

		for (const auto& generalJoker : tableJokers)
		{
			auto& joker = generalJoker.get().asTableJokerTile();
			std::set<uint16_t> unlockingIds;

			for (const auto& generalTile : deckTiles)
			{
				if (generalTile->type() != DECK_REGULAR)
				{
					continue;
				}

				auto& tile = generalTile->asDeckRegularTile();
				if ((tile.color() & joker.lockedColors()) != 0
						&& tile.number() == joker.lockedNumber())
				{
					unlockingIds.insert(tile.id());
				}
			}

			if (unlockingIds.size() != 0)
			{
				// This joker is potentially unlocked
				std::unique_ptr<Tile> lockedValue = joker.isLockedInStair()
					? TableJokerTile::conditionallyLockedInStair(
							joker.lockedNumber(),
							joker.lockedColors(),
							joker.id(),
							unlockingIds)
					: TableJokerTile::conditionallyLockedInTrio(
							joker.lockedNumber(),
							joker.lockedColors(),
							joker.id(),
							unlockingIds);

				(*tableTiles[joker.id()]) = std::move(*lockedValue);
			}
		}

		return tableTiles;
	}
}

GameParser::Result::Result(
		std::vector<std::unique_ptr<Tile>> playerTiles,
		std::vector<std::unique_ptr<Tile>> tableTiles)
	: mPlayerTiles(std::move(playerTiles)),
	  mTableTiles(std::move(tableTiles)),
	  mAllTiles(::mergeTiles(mPlayerTiles, mTableTiles))
{  }


const std::vector<std::unique_ptr<Tile>>&
GameParser::Result::playerTiles() const
{
	return mPlayerTiles;
}

const std::vector<std::unique_ptr<Tile>>&
GameParser::Result::tableTiles() const
{
	return mTableTiles;
}

std::vector<std::reference_wrapper<Tile>>& GameParser::Result::allTiles()
{
	return mAllTiles;
}

boost::optional<GameParser::Result> GameParser::parseGame(
		const boost::filesystem::path& playersDeckFile,
		const boost::filesystem::path& tableFile)
{
	// Before creating any tile we need to set the number of tiles in the game.
	const uint16_t numberOfDeckTiles = ::countWordsInFile(playersDeckFile);
	const uint16_t numberOfTableTiles = ::countWordsInFile(tableFile);

	// Update GameInfo
	GameInfo::mNumberOfTiles = numberOfDeckTiles + numberOfTableTiles;
	GameInfo::setTableTilesMask(numberOfTableTiles);

	auto playersDeck = ::parsePlayersDeck(playersDeckFile);
	if (!playersDeck)
	{
		return boost::none;
	}

	auto table = ::parseTable(tableFile, *playersDeck);
	if (!table)
	{
		return boost::none;
	}

	for (const auto& tile : *table)
	{
		if (tile->type() == TABLE_JOKER
				&& tile->asTableJokerTile().isConditionallyLocked())
		{
			GameInfo::mConditionallyLockedJokers.push_back(
					std::cref(tile->asTableJokerTile()));
		}
	}

	return GameParser::Result(std::move(*playersDeck), std::move(*table));
}
