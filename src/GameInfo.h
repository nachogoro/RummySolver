#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "Group.h"
#include "Parser/GameParser.h"
#include "ShufflingTools.h"
#include <boost/dynamic_bitset.hpp>
#include <cstdint>

/**
 * A class containing information about the current game.
 * The idea is that the information is only written once, as soon as possible,
 * and can then be accessed by different classes in a read-only way.
 * A hacky way to get around using globals or having to pass a constant value
 * around.
 */
class GameInfo
{
	friend boost::optional<GameParser::Result> GameParser::parseGame(
			const boost::filesystem::path& playersDeckFile,
			const boost::filesystem::path& tableFile);
	friend std::vector<Group> ShufflingTools::getAllPossibleGroups(
		const std::vector<std::reference_wrapper<Tile>>& allTiles);

	public:
		static uint16_t numberOfTiles();
		static const boost::dynamic_bitset<>& tableTilesMask();
		static const std::vector<std::reference_wrapper<Tile>>& allTiles();
		static const std::vector<std::reference_wrapper<const TableJokerTile>>& conditionallyLockedJokers();
		static uint16_t numberOfGroups();
		static const std::vector<std::reference_wrapper<const Group>>& allGroups();
	private:
		static void setTableTilesMask(uint16_t numberOfTableTiles);
	private:
		static uint16_t mNumberOfTiles;
		static std::vector<std::reference_wrapper<Tile>> mAllTiles;
		static boost::dynamic_bitset<> mTableTilesMask;
		static std::vector<std::reference_wrapper<const TableJokerTile>> mConditionallyLockedJokers;
		static std::vector<std::reference_wrapper<const Group>> mAllGroups;
};

#endif /* GAME_INFO_H */
