#ifndef DECK_JOKER_TILE_H
#define DECK_JOKER_TILE_H

#include "Tile.h"

/**
 * Class representing a joker tile in the player's deck.
 */
class DeckJokerTile : public Tile
{
	public:
		DeckJokerTile(uint16_t id);

		/**
		 * Whether the specified tile could follow this one in a stair.
		 */
		virtual bool canBeFollowedInStairBy(const Tile& o) const;

		/**
		 * Whether the specified tile could follow this one in a trio.
		 */
		virtual bool isCompatibleForTrio(const Tile& o) const;

		/**
		 * Returns a string representation of the tile.
		 */
		virtual std::string toString() const;
};

#endif /* DECK_JOKER_TILE_H */
