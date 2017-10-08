#include "PotentialTrio.h"

PotentialTrio::PotentialTrio(const Tile& tile)
	: PotentialGroup(tile, tile.mCompatibleForTrio, PotentialGroup::TRIO)
{  }

void PotentialTrio::addTile(const Tile& tile)
{
	PotentialGroup::addTile(tile);
	mCompatibleTiles &= tile.mCompatibleForTrio;
}
