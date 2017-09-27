#include "PotentialStair.h"

PotentialStair::PotentialStair(const Tile& tile)
	: PotentialGroup(tile, tile.mCompatibleForStair)
{  }

void PotentialStair::addTile(const Tile& tile)
{
	PotentialGroup::addTile(tile);
	mCompatibleTiles = tile.mCompatibleForStair;
}
