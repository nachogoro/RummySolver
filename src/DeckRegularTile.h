#ifndef DECK_REGULAR_TILE_H
#define DECK_REGULAR_TILE_H

#include "Tile.h"
#include "TileColor.h"

/**
 * Class representing a regular tile (i.e. not a joker) from the player's deck.
 */
class DeckRegularTile : public Tile
{
	public:
		DeckRegularTile(uint8_t number, TileColor::Color color, uint16_t id);

		/**
		 * Returns a reference to the current object as a regular tile from the
		 * player's deck.
		 */
		virtual DeckRegularTile& asDeckRegularTile();

		/**
		 * Returns a const reference to the current object as a regular tile
		 * from the player's deck.
		 */
		virtual const DeckRegularTile& asDeckRegularTile() const;

		/**
		 * Returns the number of the tile.
		 */
		uint8_t number() const;

		/**
		 * Returns the color of the tile.
		 */
		TileColor::Color color() const;

		/**
		 * Whether the specified tile could follow this one in a stair.
		 */
		virtual bool canBeFollowedInStairBy(const Tile& o) const;

		/**
		 * Whether the specified tile could follow this one in a trio.
		 */
		virtual bool isCompatibleForTrio(const Tile& o) const;

	private:
		uint8_t mNumber;
		TileColor::Color mColor;
};

#endif /* DECK_REGULAR_TILE_H */
