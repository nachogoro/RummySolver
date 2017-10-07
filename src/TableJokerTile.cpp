#include "TableJokerTile.h"
#include "DeckRegularTile.h"
#include "TableRegularTile.h"

TableJokerTile::TableJokerTile(uint16_t id)
	: Tile(TABLE_JOKER, id)
{  }

TableJokerTile::TableJokerTile(
		LockMode lock, uint8_t number, uint8_t color_mask, uint16_t id,
		const std::set<uint16_t>& unlockingTilesIds)
	: Tile(TABLE_JOKER, id),
	  mLockParams(LockParams
			  {
				  lock,
				  number,
				  color_mask
			  }),
	  mUnlockingTilesIds(unlockingTilesIds)
{ }

std::unique_ptr<TableJokerTile> TableJokerTile::unlocked(uint16_t id)
{
	return std::unique_ptr<TableJokerTile>(new TableJokerTile(id));
}

std::unique_ptr<TableJokerTile> TableJokerTile::conditionallyLockedInTrio(
		uint8_t number,
		uint8_t colorMask,
		uint16_t id,
		const std::set<uint16_t>& unlockingTilesIds)
{
	return std::unique_ptr<TableJokerTile>(
			new TableJokerTile(TRIO, number, colorMask, id, unlockingTilesIds));
}

std::unique_ptr<TableJokerTile> TableJokerTile::conditionallyLockedInStair(
		uint8_t number,
		uint8_t color,
		uint16_t id,
		const std::set<uint16_t>& unlockingTilesIds)
{
	return std::unique_ptr<TableJokerTile>(
			new TableJokerTile(STAIR, number, color, id, unlockingTilesIds));
}

std::unique_ptr<TableJokerTile> TableJokerTile::lockedInStair(
		uint8_t number, uint8_t color, uint16_t id)
{
	return std::unique_ptr<TableJokerTile>(
			new TableJokerTile(
				STAIR, number, color, id, std::set<uint16_t>()));
}

std::unique_ptr<TableJokerTile> TableJokerTile::lockedInTrio(
		uint8_t number, uint8_t color_mask, uint16_t id)
{
	return std::unique_ptr<TableJokerTile>(
			new TableJokerTile(
				TRIO, number, color_mask, id, std::set<uint16_t>()));
}

TableJokerTile& TableJokerTile::asTableJokerTile()
{
	return *this;
}

const TableJokerTile& TableJokerTile::asTableJokerTile() const
{
	return *this;
}

bool TableJokerTile::canBeFollowedInStairBy(const Tile& other) const
{
	if (!isLocked())
	{
		return true;
	}

	if (isLockedInTrio())
	{
		return false;
	}

	switch (other.type())
	{
		case DECK_REGULAR:
		{
			const auto& o = other.asDeckRegularTile();
			return (o.color() & lockedColors())
				&& (o.number() == lockedNumber() + 1);
		}
		case DECK_JOKER:
		{
			return true;
		}
		case TABLE_REGULAR:
		{
			const auto& o = other.asTableRegularTile();
			return (o.color() & lockedColors())
				&& (o.number() == lockedNumber() + 1);
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

			return (o.lockedColors() & lockedColors())
				&& (o.lockedNumber() == lockedNumber() + 1);
		}
	}

	throw std::runtime_error("Tile is of no known type!");
}

bool TableJokerTile::isCompatibleForTrio(const Tile& other) const
{
	if (!isLocked())
	{
		return true;
	}

	if (isLockedInStair())
	{
		return false;
	}

	switch (other.type())
	{
		case DECK_REGULAR:
		{
			const auto& o = other.asDeckRegularTile();
			return (o.color() ^ lockedColors())
				&& (o.number() == lockedNumber());
		}
		case DECK_JOKER:
		{
			return true;
		}
		case TABLE_REGULAR:
		{
			const auto& o = other.asTableRegularTile();
			return (o.color() ^ lockedColors())
				&& (o.number() == lockedNumber());
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

			return (o.lockedColors() ^ lockedColors())
				&& (o.lockedNumber() == lockedNumber());
		}
	}

	throw std::runtime_error("Tile is of no known type!");
}

bool TableJokerTile::isLocked() const
{
	return !!mLockParams && mUnlockingTilesIds.size() == 0;
}

bool TableJokerTile::isConditionallyLocked() const
{
	return mUnlockingTilesIds.size() != 0;
}

bool TableJokerTile::isLockedInStair() const
{
	return !!mLockParams && mLockParams->lockMode == STAIR;
}

bool TableJokerTile::isLockedInTrio() const
{
	return mLockParams && mLockParams->lockMode == TRIO;
}

uint8_t TableJokerTile::lockedNumber() const
{
	return mLockParams->number;
}

uint8_t TableJokerTile::lockedColors() const
{
	return mLockParams->color_mask;
}

const std::set<uint16_t>& TableJokerTile::unlockingTilesIds() const
{
	return mUnlockingTilesIds;
}
