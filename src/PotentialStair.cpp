#include "PotentialStair.h"

PotentialStair::PotentialStair(const Tile& tile)
	: PotentialGroup(tile, tile.getCompatibleTilesForStair())
{  }

void PotentialStair::addTile(const Tile& tile)
{
	mTiles.push_back(std::cref(tile));
	mCompatibleTiles = tile.getCompatibleTilesForStair();
}
