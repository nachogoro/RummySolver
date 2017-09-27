#include "PotentialGroup.h"
#include "DeckRegularTile.h"

PotentialGroup::PotentialGroup(
		const Tile& tile,
		const boost::dynamic_bitset<>& compatibleTiles)
	: mCompatibleTiles(compatibleTiles),
	  mTilesId(compatibleTiles.size()),
	  mScore(0)
{
	mTilesId.set(tile.id());
	if (tile.type() == DECK_REGULAR)
	{
		mScore = tile.asDeckRegularTile().number();
	}
	else if (tile.type() == DECK_JOKER)
	{
		mScore = 50;
	}
}

// Make sure mLastReturnedId is not copied, since this particular object has
// not returned any ID yet.
// TODO: Change the approach to use one similar to GroupConfiguration
PotentialGroup::PotentialGroup(const PotentialGroup& o)
	: mCompatibleTiles(o.mCompatibleTiles),
	  mTilesId(o.mTilesId),
	  mScore(o.mScore),
	  mLastReturnedId(boost::none)
{ }

PotentialGroup::~PotentialGroup()
{  }


void PotentialGroup::addTile(const Tile& tile)
{
	mTilesId.set(tile.id());

	if (tile.type() == DECK_REGULAR)
	{
		mScore += tile.asDeckRegularTile().number();
	}
	else if (tile.type() == DECK_JOKER)
	{
		mScore += 50;
	}
}

bool PotentialGroup::isValid() const
{
	return mTilesId.count() >= 3;
}

uint16_t PotentialGroup::score() const
{
	return mScore;
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
