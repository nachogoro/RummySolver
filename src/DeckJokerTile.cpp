#include "DeckJokerTile.h"
#include "TableJokerTile.h"

DeckJokerTile::DeckJokerTile(uint16_t id, uint16_t n_of_tiles)
	: Tile::Tile(DECK_JOKER, id, n_of_tiles)
{  }

bool DeckJokerTile::isCompatibleForStair(const Tile& o) const
{
	if (o.type() != TABLE_JOKER || !o.asTableJokerTile().isLockedInTrio())
	{
		return true;
	}

	return false;
}

bool DeckJokerTile::isCompatibleForTrio(const Tile& o) const
{
	if (o.type() != TABLE_JOKER || !o.asTableJokerTile().isLockedInStair())
	{
		return true;
	}

	return false;
}
