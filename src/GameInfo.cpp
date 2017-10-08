#include "GameInfo.h"

uint16_t GameInfo::mNumberOfTiles;
boost::dynamic_bitset<> GameInfo::mTableTilesMask;
std::vector<std::reference_wrapper<const TableJokerTile>> GameInfo::mConditionallyLockedJokers;
std::vector<std::reference_wrapper<const Group>> GameInfo::mAllGroups;


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

const std::vector<std::reference_wrapper<const TableJokerTile>>&
GameInfo::conditionallyLockedJokers()
{
	return mConditionallyLockedJokers;
}

uint16_t GameInfo::numberOfGroups()
{
	return mAllGroups.size();
}

const std::vector<std::reference_wrapper<const Group>>& GameInfo::allGroups()
{
	return mAllGroups;
}
