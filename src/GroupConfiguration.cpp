#include "GroupConfiguration.h"

GroupConfiguration::GroupConfiguration(const Group& group)
	: mGroups(group.mCompatibleGroups.size()),
	  mTiles(group.mTilesInGroup),
	  mCompatibleGroups(group.compatibleGroups()),
	  mScore(group.score())
{
	mGroups.set(group.id());
}


