#include "DeckRegularTile.h"
#include "TableRegularTile.h"
#include "TableJokerTile.h"

DeckRegularTile::DeckRegularTile(
		uint8_t number, TileColor color,
		uint16_t id, uint16_t n_of_tiles)
	: Tile::Tile(DECK_REGULAR, id, n_of_tiles),
	  mNumber(number),
	  mColor(color)
{  }

uint8_t DeckRegularTile::number() const
{
	return mNumber;
}

TileColor DeckRegularTile::color() const
{
	return mColor;
}

DeckRegularTile& DeckRegularTile::asDeckRegularTile()
{
	return *this;
}

const DeckRegularTile& DeckRegularTile::asDeckRegularTile() const
{
	return *this;
}

bool DeckRegularTile::isCompatibleForStair(const Tile& other) const
{
	switch (other.type())
	{
		case DECK_REGULAR:
		{
			const auto& o = other.asDeckRegularTile();
			return (o.color() == color()) && (o.number() == number() + 1);
		}
		case DECK_JOKER:
		{
			return true;
		}
		case TABLE_REGULAR:
		{
			const auto& o = other.asTableRegularTile();
			return (o.color() == color()) && (o.number() == number() + 1);
		}
		case TABLE_JOKER:
		{
			const auto& o = other.asTableJokerTile();
			if (!o.isLocked())
			{
				return true;
			}

			if (o.isLockedInTrio())
			{
				return false;
			}

			return (o.lockedColors() == color())
				&& (o.lockedNumber() == number() + 1);
		}
	}

	throw std::runtime_error("Tile is of no known type!");
}

bool DeckRegularTile::isCompatibleForTrio(const Tile& other) const
{
	switch (other.type())
	{
		case DECK_REGULAR:
		{
			const auto& o = other.asDeckRegularTile();
			return (o.color() != color()) && (o.number() == number());
		}
		case DECK_JOKER:
		{
			return true;
		}
		case TABLE_REGULAR:
		{
			const auto& o = other.asTableRegularTile();
			return (o.color() != color()) && (o.number() == number());
		}
		case TABLE_JOKER:
		{
			const auto& o = other.asTableJokerTile();
			if (!o.isLocked())
			{
				return true;
			}

			if (o.isLockedInStair())
			{
				return false;
			}

			return (o.lockedColors() ^ color())
				&& (o.lockedNumber() == number());
		}
	}

	throw std::runtime_error("Tile is of no known type!");
}
