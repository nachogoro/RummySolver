#ifndef DECK_JOKER_TILE_H
#define DECK_JOKER_TILE_H

#include "Tile.h"

/**
 * Class representing a joker tile in the player's deck.
 */
class DeckJokerTile : public Tile
{
	public:
		DeckJokerTile(uint16_t id, uint16_t n_of_tiles);

		/**
		 * Whether the specified tile could follow this one in a stair.
		 */
		virtual bool canBeFollowedInStairBy(const Tile& o) const;

		/**
		 * Whether the specified tile could follow this one in a trio.
		 */
		virtual bool isCompatibleForTrio(const Tile& o) const;
};

#endif /* DECK_JOKER_TILE_H */
