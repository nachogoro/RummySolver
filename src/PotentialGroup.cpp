#include "PotentialGroup.h"

PotentialGroup::PotentialGroup(
		const Tile& tile,
		const boost::dynamic_bitset<>& compatibleTiles)
	: mTiles{std::cref(tile)},
	  mCompatibleTiles(compatibleTiles)
{  }

// Make sure mLastReturnedId is not copied, since this particular object has
// not returned any ID yet.
PotentialGroup::PotentialGroup(const PotentialGroup& o)
	: mTiles(o.mTiles),
	  mCompatibleTiles(o.mCompatibleTiles),
	  mLastReturnedId(boost::none)
{ }

PotentialGroup::~PotentialGroup()
{  }

bool PotentialGroup::isValid() const
{
	return mTiles.size() >= 3;
}

boost::optional<uint16_t> PotentialGroup::nextCompatibleId()
{
	boost::dynamic_bitset<>::size_type nextId;

	if (!mLastReturnedId)
	{
		nextId = mCompatibleTiles.find_first();
	}
	else
	{
		nextId = mCompatibleTiles.find_next(*mLastReturnedId);
	}

	if (nextId == boost::dynamic_bitset<>::npos)
	{
		return boost::none;
	}

	mLastReturnedId = nextId;
	return nextId;
}
