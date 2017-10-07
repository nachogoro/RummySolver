#include "Group.h"
#include "GameInfo.h"

Group::Group(const PotentialGroup& g)
	: mTilesInGroup(g.mTilesId),
	  mId(0),
	  mScore(g.score())
{  }

// TODO: Mutability is bad. This leaves the window open for mId to be
// uninitialised when id() is called for the first time. Ideally we should use
// dependency injection in the constructor.
void Group::setId(uint16_t id)
{
	mId = id;
	mCompatibleGroups = boost::dynamic_bitset<>(GameInfo::numberOfGroups());
}

uint16_t Group::id() const
{
	return mId;
}

// TODO: Mutability is bad. This leaves the window open for mCompatibleGroups
// to be uninitialised when compatibleGroups() is called for the first time.
// Ideally we should use dependency injection in the constructor.
void Group::markGroupIfCompatible(const Group& o)
{
	if ((mTilesInGroup & o.mTilesInGroup).none())
	{
		mCompatibleGroups.set(o.id());
	}
}

const boost::dynamic_bitset<>& Group::compatibleGroups() const
{
	return mCompatibleGroups;
}

uint16_t Group::score() const
{
	return mScore;
}
