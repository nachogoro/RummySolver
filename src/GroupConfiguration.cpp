#include "GroupConfiguration.h"
#include "GameInfo.h"

GroupConfiguration::GroupConfiguration(const Group& group)
	: mGroups(group.mCompatibleGroups.size()),
	  mTiles(group.mTilesInGroup),
	  mCompatibleGroups(group.compatibleGroups()),
	  mScore(group.score())
{
	mGroups.set(group.id());
}

bool GroupConfiguration::isValid() const
{
	// The configuration is valid if all tiles from the table have been used.
	// The table tiles are the last segment of the tile bitset.
	// TODO: add check for locked jokers:
	// 1) There are no table jokers OR
	// 2) The only table jokers are locked OR
	// 3) The unlocking deck tile has been played OR
	// 4) The conditionally unlocked jokers are kept in their original positions
	return (mTiles & GameInfo::tableTilesMask()) == GameInfo::tableTilesMask();
}

uint16_t GroupConfiguration::score() const
{
	return mScore;
}

const boost::dynamic_bitset<>& GroupConfiguration::compatibleGroups() const
{
	return mCompatibleGroups;
}

void GroupConfiguration::addGroup(const Group& group)
{
	mGroups.set(group.id());
	mTiles |= group.mTilesInGroup;
	mCompatibleGroups &= group.compatibleGroups();
	mScore += group.score();
}
