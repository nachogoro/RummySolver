#ifndef GAME_INFO_H
#define GAME_INFO_H

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
	friend class ShufflingTools;
	friend class RummyParser;

	public:
		static uint16_t numberOfTiles();
		static const boost::dynamic_bitset<>& tableTilesMask();
		static uint16_t numberOfGroups();
	private:
		static void setTableTilesMask(uint16_t numberOfTableTiles);
		static uint16_t mNumberOfTiles;
		static boost::dynamic_bitset<> mTableTilesMask;
		static uint16_t mNumberOfGroups;
};

#endif /* GAME_INFO_H */
