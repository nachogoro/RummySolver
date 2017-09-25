#include "Tile.h"

Tile::Tile(TileType type, uint16_t id, uint16_t n_of_tiles)
	: mId(id),
	  mType(type),
	  mCompatibleForTrio(n_of_tiles),
	  mCompatibleForStair(n_of_tiles)
{}

Tile::~Tile()
{}

uint16_t Tile::id() const
{
	return mId;
}

TileType Tile::type() const
{
	return mType;
}

void Tile::addCompatibleTileForStair(uint16_t id)
{
	mCompatibleForStair[id] = 1;
}

void Tile::addCompatibleTileForTrio(uint16_t id)
{
	mCompatibleForTrio[id] = 1;
}
const boost::dynamic_bitset<>& Tile::getCompatibleTilesForTrio() const
{
	return mCompatibleForTrio;
}

const boost::dynamic_bitset<>& Tile::getCompatibleTilesForStair() const
{
	return mCompatibleForStair;
}

DeckRegularTile& Tile::asDeckRegularTile()
{
	throw std::runtime_error(
			"asDeckRegularTile() invoked for tile of type "
			+ std::to_string(mType));
}

TableRegularTile& Tile::asTableRegularTile()
{
	throw std::runtime_error(
			"asTableRegularTile() invoked for tile of type "
			+ std::to_string(mType));
}

TableJokerTile& Tile::asTableJokerTile()
{
	throw std::runtime_error(
			"asTableJokerTile() invoked for tile of type "
			+ std::to_string(mType));
}

const DeckRegularTile& Tile::asDeckRegularTile() const
{
	throw std::runtime_error(
			"asDeckRegularTile() invoked for tile of type "
			+ std::to_string(mType));
}

const TableRegularTile& Tile::asTableRegularTile() const
{
	throw std::runtime_error(
			"asTableRegularTile() invoked for tile of type "
			+ std::to_string(mType));
}

const TableJokerTile& Tile::asTableJokerTile() const
{
	throw std::runtime_error(
			"asTableJokerTile() invoked for tile of type "
			+ std::to_string(mType));
}
