#include "TableRegularTile.h"
#include "TableJokerTile.h"
#include "DeckRegularTile.h"
#include <sstream>

TableRegularTile::TableRegularTile(
		uint8_t number, TileColor::Color color, uint16_t id)
	: Tile::Tile(TABLE_REGULAR, id),
	  mNumber(number),
	  mColor(color)
{  }

uint8_t TableRegularTile::number() const
{
	return mNumber;
}

TileColor::Color TableRegularTile::color() const
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

bool TableRegularTile::canBeFollowedInStairBy(const Tile& other) const
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
			if (!o.isInconditionallyLocked())
			{
				return true;
			}

			if (o.isLockedInTrio())
			{
				return false;
			}

			return (o.lockedColors() & color())
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
			if (!o.isInconditionallyLocked())
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

std::string TableRegularTile::toString() const
{
	std::ostringstream ss;
	ss << TileColor::colorToLetter.at(mColor) << static_cast<int>(mNumber);
	return ss.str();
}
