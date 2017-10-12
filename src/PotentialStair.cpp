#include "PotentialStair.h"
#include "DeckRegularTile.h"
#include "GameInfo.h"
#include "TableJokerTile.h"
#include "TableRegularTile.h"
#include "TileColor.h"

PotentialStair::PotentialStair(const Tile& tile)
	: PotentialGroup(tile, tile.mCompatibleForStair, PotentialGroup::STAIR)
{  }

void PotentialStair::addTile(const Tile& tile)
{
	PotentialGroup::addTile(tile);
	// We need to be careful with jokers: every tile believes it can be
	// followed by a joker and jokers believe they can be followed by any tile.

	// Don't add a tile twice
	auto compatibleTiles = tile.mCompatibleForStair & ~mTilesId;

	if (tile.type() == DECK_JOKER
			|| (tile.type() == TABLE_JOKER
				&& !tile.asTableJokerTile().isInconditionallyLocked()))
	{
		// The joker states it can be followed by any tile, but in reality it
		// depends on the current state of the stair
		boost::optional<std::pair<uint8_t, TileColor::Color>> params;
		for (size_t i = 0; i < mTiles.size(); ++i)
		{
			const auto& tmpTile = mTiles[mTiles.size() - 1 - i].get();
			if (tmpTile.type() == DECK_REGULAR)
			{
				const auto& regTile = tmpTile.asDeckRegularTile();
				params = std::make_pair(
						regTile.number() + i + 1, regTile.color());
				break;
			}
			else if (tmpTile.type() == TABLE_REGULAR)
			{
				const auto& regTile = tmpTile.asTableRegularTile();
				params = std::make_pair(
						regTile.number() + i + 1, regTile.color());
				break;
			}
		}

		if (params)
		{
			const uint8_t number = params->first;
			const TileColor::Color color = params->second;
			// The stair has somehting else than jokers, so there are some
			// restrictions
			for (const auto& compTile : GameInfo::allTiles())
			{
				switch (compTile.get().type())
				{
					case DECK_REGULAR:
					{
						const auto& o = compTile.get().asDeckRegularTile();
						if ((o.color() != color) || (o.number() != number))
						{
							compatibleTiles.reset(o.id());
						}
						break;
					}
					case TABLE_REGULAR:
					{
						const auto& o = compTile.get().asTableRegularTile();
						if ((o.color() != color) || (o.number() != number))
						{
							compatibleTiles.reset(o.id());
						}
						break;
					}
					case TABLE_JOKER:
					{
						const auto& o = compTile.get().asTableJokerTile();
						if (!o.isInconditionallyLocked())
						{
							break;
						}

						if (o.isLockedInTrio())
						{
							compatibleTiles.reset(o.id());
							break;
						}

						if (!(o.lockedColors() & color)
							|| (o.lockedNumber() != number))
						{
							compatibleTiles.reset(o.id());
							break;
						}
					}
					default:
						break;
				}
			}
		}
	}

	mCompatibleTiles = compatibleTiles;
}
