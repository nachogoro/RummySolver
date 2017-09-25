#include "PotentialTrio.h"

PotentialTrio::PotentialTrio(const Tile& tile)
	: PotentialGroup(tile, tile.getCompatibleTilesForTrio())
{  }

void PotentialTrio::addTile(const Tile& tile)
{
	mTiles.push_back(std::cref(tile));
	mCompatibleTiles &= tile.getCompatibleTilesForTrio();
}
