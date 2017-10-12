#include "ParsingUtils.h"
#include "TileColor.h"
#include "TableRegularTile.h"
#include "TableJokerTile.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>

namespace
{
	PotentialGroup::GroupType askUserForType(
			const std::vector<ParsingUtils::TemporaryTile>& tiles)
	{
		std::ostringstream repr;
		for (const auto& tile : tiles)
		{
			if (tile.isJoker())
			{
				repr << "J ";
			}
			else
			{
				repr << tile.number() << " "
					<< TileColor::colorToLetter.at(tile.color()) << " ";
			}
		}

		char option;
		// There are two jokers in this group and only one regular tile, so it
		// is impossible to know if it is a trio or a stair without asking
		std::cout << "Is the following group a trio or a stair?\n";
		std::cout << "\t" << repr.str() << "\n";
		std::cout << "\t\t [T]rio\n\t\t [S]tair\n(T/S): ";
		std::cin >> option;

		while (true)
		{
			if (option == 'T' || option == 't')
			{
				return PotentialGroup::TRIO;
			}
			else if (option == 'S' || option == 's')
			{
				return PotentialGroup::STAIR;
			}
			else
			{
				std::cout << "Invalid option. (T/S): ";
				std::cin >> option;
			}
		}
	}

	boost::optional<PotentialGroup::GroupType> deduceGroupType(
			const std::vector<ParsingUtils::TemporaryTile>& tiles)
	{
		uint8_t nonJokerTiles = 0;
		for (const auto& tile : tiles)
		{
			if (!tile.isJoker())
			{
				++nonJokerTiles;
			}
		}

		if (nonJokerTiles == 1)
		{
			// Only one regular tile, cannot deduce type of group
			return askUserForType(tiles);
		}
		else
		{
			std::set<TileColor::Color> colors;
			std::set<uint8_t> numbers;
			for (const auto& tile : tiles)
			{
				if (!tile.isJoker())
				{
					colors.insert(tile.color());
					numbers.insert(tile.number());
				}
			}

			if (colors.size() == 1 && numbers.size() == nonJokerTiles)
			{
				return PotentialGroup::STAIR;
			}
			else if (numbers.size() == 1
					&& colors.size() == nonJokerTiles
					&& tiles.size() <= 4)
			{
				return PotentialGroup::TRIO;
			}
			else
			{
				return boost::none;
			}
		}
	}

	bool containsJoker(const std::vector<ParsingUtils::TemporaryTile>& tiles)
	{
		for (const auto& tile : tiles)
		{
			if (tile.isJoker())
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * Returns the number of the first element of a stair.
	 * This is necessary because it might be a joker.
	 */
	uint8_t getFirstNumberOfStair(
			const std::vector<ParsingUtils::TemporaryTile>& tiles)
	{
		for (size_t i = 0; i < tiles.size(); ++i)
		{
			const auto& tile = tiles[i];
			if (!tile.isJoker())
			{
				return tile.number() - i;
			}
		}

		throw std::runtime_error("Could not find start number of stair!");
	}

	TileColor::Color getStairColor(
			const std::vector<ParsingUtils::TemporaryTile>& tiles)
	{
		for (const auto& tile : tiles)
		{
			if (!tile.isJoker())
			{
				return tile.color();
			}
		}
		throw std::runtime_error("Could not find stair color!");
	}

	/**
	 * Turns a temporary group which contains at least a joker into a tile
	 * group. At this stage we cannot know if the jokers are locked or not, so
	 * we return them as locked to their current position and that must be
	 * changed at a later stage.
	 */
	boost::optional<std::vector<std::unique_ptr<Tile>>> jokerGroupToTiles(
			const std::vector<ParsingUtils::TemporaryTile>& tiles,
			uint16_t& id)
	{
		std::vector<std::unique_ptr<Tile>> result;

		// This method has a joker, so the first thing is to figure out whether
		// it is a stair or a trio
		const auto type = ::deduceGroupType(tiles);
		if (!type)
		{
			std::cout << "Invalid group in table\n";
			return boost::none;
		}

		switch (*type)
		{
			case PotentialGroup::TRIO:
			{
				uint8_t mask = 0;
				uint8_t number;
				for (const auto& tile : tiles)
				{
					if (!tile.isJoker())
					{
						mask |= tile.color();
						number = tile.number();
						result.push_back(std::unique_ptr<Tile>(
									new TableRegularTile(
										tile.number(), tile.color(), id++)));
					}
				}

				for (const auto& tile : tiles)
				{
					if (tile.isJoker())
					{
						result.push_back(
								TableJokerTile::lockedInTrio(
									number,
									static_cast<uint8_t>(
										(~mask) & TileColor::colorMask),
									id++));
					}
				}
				break;
			}
			case PotentialGroup::STAIR:
			{
				const TileColor::Color color = ::getStairColor(tiles);

				uint8_t number = ::getFirstNumberOfStair(tiles);

				for (size_t i = 0; i < tiles.size(); ++i)
				{
					const auto& tile = tiles[i];

					if (!tile.isJoker())
					{
						if (tile.number() != number || tile.color() != color)
						{
							std::cout << "Invalid stair in table!\n";
							return boost::none;
						}

						result.push_back(std::unique_ptr<Tile>(
									new TableRegularTile(
										number++, tile.color(), id++)));
					}
					else
					{
						result.push_back(
								TableJokerTile::lockedInStair(
									number++, color, id++));
					}
				}

				break;
			}
		}

		return result;
	}
}

ParsingUtils::TemporaryTile ParsingUtils::TemporaryTile::joker()
{
	return TemporaryTile(boost::none);
}

ParsingUtils::TemporaryTile ParsingUtils::TemporaryTile::regular(
		uint8_t number, TileColor::Color color)
{
	return TemporaryTile(std::make_pair(number, color));
}

ParsingUtils::TemporaryTile::TemporaryTile(
		const boost::optional<std::pair<uint8_t, TileColor::Color>>& regularInfo)
	: mRegularInfo(regularInfo)
{  }

bool ParsingUtils::TemporaryTile::isJoker() const
{
	return !mRegularInfo;
}

uint8_t ParsingUtils::TemporaryTile::number() const
{
	return mRegularInfo->first;
}

TileColor::Color ParsingUtils::TemporaryTile::color() const
{
	return mRegularInfo->second;
}

boost::optional<std::vector<std::vector<ParsingUtils::TemporaryTile>>>
ParsingUtils::parseFile(std::ifstream& input)
{
	std::string line;
	std::vector<std::vector<TemporaryTile>> result;

	// Each line is a group
	while (std::getline(input, line))
	{
		int number;
		char colorLetter;
		std::vector<TemporaryTile> tiles;

		std::vector<std::string> tilesStr;
		boost::split(tilesStr, line, boost::is_any_of(" "));
		for (const auto& tile : tilesStr)
		{
			std::istringstream iss(tile);
			if (!(iss >> colorLetter))
			{
				std::cerr << "Invalid line: " << line << " in table file\n";
				return boost::none;
			}

			if (colorLetter == 'J')
			{
				// It's a joker, we need to deduce its value.
				tiles.push_back(TemporaryTile::joker());
				continue;
			}

			if (!TileColor::letterToColor.count(colorLetter))
			{
				std::cerr << "Invalid color " << colorLetter << " in line "
					<< line << " in table file\n";
				return boost::none;
			}

			if (!(iss >> number))
			{
				std::cerr << "Invalid line: " << line << " in table file\n";
				return boost::none;
			}

			tiles.push_back(TemporaryTile::regular(
						number, TileColor::letterToColor.at(colorLetter)));
		}

		result.push_back(tiles);
	}

	return result;
}

boost::optional<std::vector<std::unique_ptr<Tile>>>
ParsingUtils::temporaryTilesToTiles(
		const std::vector<std::vector<TemporaryTile>>& tmpTiles,
		const std::vector<std::unique_ptr<Tile>>& deckTiles)
{
	std::vector<std::unique_ptr<Tile>> tiles;
	uint16_t id = deckTiles.size();

	for (const auto& group : tmpTiles)
	{
		if (!::containsJoker(group))
		{
			for (const auto& tile : group)
			{
				tiles.push_back(std::unique_ptr<Tile>(new TableRegularTile(
								tile.number(), tile.color(), id++)));
			}
		}
		else
		{
			boost::optional<std::vector<std::unique_ptr<Tile>>> tmpGroup
				= ::jokerGroupToTiles(group, id);

			if (!tmpGroup)
			{
				return boost::none;
			}

			for (auto& tmp : *tmpGroup)
			{
				tiles.push_back(std::move(tmp));
			}
		}
	}

	return tiles;
}
