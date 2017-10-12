#include "Group.h"
#include "GameInfo.h"
#include <sstream>

Group::Group(const PotentialGroup& g)
	: mType(g.mType),
	  mTileIdsInGroup(g.mTilesId),
	  mTilesInGroup(g.mTiles),
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
	if ((mTileIdsInGroup & o.mTileIdsInGroup).none())
	{
		mCompatibleGroups.set(o.id());
	}
}

const boost::dynamic_bitset<>& Group::compatibleGroups() const
{
	return mCompatibleGroups;
}

const boost::dynamic_bitset<>& Group::tileIdsInGroup() const
{
	return mTileIdsInGroup;
}

const std::vector<std::reference_wrapper<const Tile>>& Group::tilesInGroup() const
{
	return mTilesInGroup;
}

uint16_t Group::score() const
{
	return mScore;
}

GroupType Group::type() const
{
	return mType;
}

std::string Group::toString() const
{
	std::ostringstream stream;
	for (const auto& tile : mTilesInGroup)
	{
		stream << tile.get().toString() << " ";
	}
	return stream.str();
}
