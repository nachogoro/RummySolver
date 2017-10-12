#ifndef TABLE_REGULAR_TILE_H
#define TABLE_REGULAR_TILE_H

#include "Tile.h"
#include "TileColor.h"

/**
 * Class representing a regular tile (i.e. not a joker) from the table.
 */
class TableRegularTile : public Tile
{
	public:
		TableRegularTile(uint8_t number, TileColor::Color color, uint16_t id);

		/**
		 * Returns a reference to the current object as a regular tile from the
		 * player's deck.
		 */
		virtual TableRegularTile& asTableRegularTile();

		/**
		 * Returns a const reference to the current object as a regular tile
		 * from the player's deck.
		 */
		virtual const TableRegularTile& asTableRegularTile() const;

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

		/**
		 * Returns a string representation of the tile.
		 */
		virtual std::string toString() const;

	private:
		uint8_t mNumber;
		TileColor::Color mColor;
};

#endif /* TABLE_REGULAR_TILE_H */
