#include "GameInfo.h"

uint16_t GameInfo::mNumberOfTiles;
boost::dynamic_bitset<> GameInfo::mTableTilesMask;
uint16_t GameInfo::mNumberOfGroups;

void GameInfo::setTableTilesMask(uint16_t numberOfTableTiles)
{
	mTableTilesMask = boost::dynamic_bitset<>(mNumberOfTiles);
	for (uint16_t i = mTableTilesMask.size() - numberOfTableTiles;
			i < mTableTilesMask.size();
			++i)
	{
		mTableTilesMask.set(i);
	}
}

uint16_t GameInfo::numberOfTiles()
{
	return mNumberOfTiles;
}

const boost::dynamic_bitset<>& GameInfo::tableTilesMask()
{
	return mTableTilesMask;
}

uint16_t GameInfo::numberOfGroups()
{
	return mNumberOfGroups;
}
