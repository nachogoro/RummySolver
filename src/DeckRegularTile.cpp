#include "DeckRegularTile.h"
#include "TableRegularTile.h"
#include "TableJokerTile.h"
#include <sstream>

DeckRegularTile::DeckRegularTile(
		uint8_t number, TileColor::Color color, uint16_t id)
	: Tile::Tile(DECK_REGULAR, id),
	  mNumber(number),
	  mColor(color)
{  }

uint8_t DeckRegularTile::number() const
{
	return mNumber;
}

TileColor::Color DeckRegularTile::color() const
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

bool DeckRegularTile::canBeFollowedInStairBy(const Tile& other) const
{
	switch (other.type())
	{
		case DECK_REGULAR:
		{
			const auto& o = other.asDeckRegularTile();
			return (o.color() == color())
				&& ((o.number() == number() + 1)
						|| (number() == 13 && o.number() == 1));

		}
		case DECK_JOKER:
		{
			return true;
		}
		case TABLE_REGULAR:
		{
			const auto& o = other.asTableRegularTile();
			return (o.color() == color())
				&& ((o.number() == number() + 1)
						|| (number() == 13 && o.number() == 1));
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
				&& ((o.lockedNumber() == number() + 1)
						|| (number() == 13 && o.lockedNumber() == 1));

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

	throw std::runtime_error("Tile is of no known type!");
}

std::string DeckRegularTile::toString() const
{
	std::ostringstream ss;
	ss << TileColor::colorToLetter.at(mColor) << static_cast<int>(mNumber);
	return ss.str();
}
