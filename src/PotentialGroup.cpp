#include "PotentialGroup.h"
#include "DeckRegularTile.h"

PotentialGroup::PotentialGroup(
		const Tile& tile,
		const boost::dynamic_bitset<>& compatibleTiles,
		GroupType type)
	: mCompatibleTiles(compatibleTiles),
	  mTilesId(compatibleTiles.size()),
	  mTiles({tile}),
	  mType(type),
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

PotentialGroup::PotentialGroup(const PotentialGroup& o)
	: mCompatibleTiles(o.compatibleTiles()),
	  mTilesId(o.mTilesId),
	  mTiles(o.mTiles),
	  mType(o.mType),
	  mScore(o.mScore)
{  }

PotentialGroup::~PotentialGroup()
{  }

void PotentialGroup::addTile(const Tile& tile)
{
	mTilesId.set(tile.id());
	mTiles.push_back(std::cref(tile));

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

const boost::dynamic_bitset<>& PotentialGroup::compatibleTiles() const
{
	return mCompatibleTiles;
}
