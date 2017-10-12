#include "DeckJokerTile.h"
#include "TableJokerTile.h"

DeckJokerTile::DeckJokerTile(uint16_t id)
	: Tile::Tile(DECK_JOKER, id)
{  }

bool DeckJokerTile::canBeFollowedInStairBy(const Tile& o) const
{
	if (o.type() != TABLE_JOKER
			|| o.asTableJokerTile().isConditionallyLocked()
			|| !o.asTableJokerTile().isLockedInTrio())
	{
		return true;
	}

	return false;
}

bool DeckJokerTile::isCompatibleForTrio(const Tile& o) const
{
	if (o.type() != TABLE_JOKER
			|| o.asTableJokerTile().isConditionallyLocked()
			|| !o.asTableJokerTile().isLockedInStair())
	{
		return true;
	}

	return false;
}

std::string DeckJokerTile::toString() const
{
	return "J";
}
