#include "TableRegularTile.h"
#include "TableJokerTile.h"
#include "DeckRegularTile.h"

TableRegularTile::TableRegularTile(
		uint8_t number, TileColor color,
		uint16_t id, uint16_t n_of_tiles)
	: Tile::Tile(TABLE_REGULAR, id, n_of_tiles),
	  mNumber(number),
	  mColor(color)
{  }

uint8_t TableRegularTile::number() const
{
	return mNumber;
}

TileColor TableRegularTile::color() const
{
	return mColor;
}

TableRegularTile& TableRegularTile::asTableRegularTile()
{
	return *this;
}

const TableRegularTile& TableRegularTile::asTableRegularTile() const
{
	return *this;
}

bool TableRegularTile::isCompatibleForStair(const Tile& other) const
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

	throw std::runtime_error("Tile of no known type!");
}

bool TableRegularTile::isCompatibleForTrio(const Tile& other) const
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

	throw std::runtime_error("Tile of no known type!");
}
