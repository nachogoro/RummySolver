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

// TODO: mutability is bad. Make the compatibility checkings a set of
// independent functions and pass this information in the constructor.
void Tile::addCompatibleTileForStair(uint16_t id)
{
	mCompatibleForStair.set(id);
}

// TODO: mutability is bad. Make the compatibility checkings a set of
// independent functions and pass this information in the constructor.
void Tile::addCompatibleTileForTrio(uint16_t id)
{
	mCompatibleForTrio.set(id);
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
