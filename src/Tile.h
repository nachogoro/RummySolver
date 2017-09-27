#ifndef RUMMY_TILE_H
#define RUMMY_TILE_H

#include <cstdint>
#include <boost/dynamic_bitset.hpp>

class DeckRegularTile;
class DeckJokerTile;
class TableRegularTile;
class TableJokerTile;

enum TileType
{
	DECK_REGULAR,
	DECK_JOKER,
	TABLE_REGULAR,
	TABLE_JOKER
};

/**
 * Base class that represents a generic tile.
 */
class Tile
{
	friend class PotentialStair;
	friend class PotentialTrio;

	public:
		Tile(TileType type, uint16_t id, uint16_t n_of_tiles);
		virtual ~Tile();

		/**
		 * Returns the ID of the tile.
		 */
		uint16_t id() const;

		/**
		 * Returns the type of the tile.
		 */
		TileType type() const;

		/**
		 * Whether another tile could follow this one in a stair.
		 */
		virtual bool canBeFollowedInStairBy(const Tile& o) const = 0;

		/**
		 * Mark the given tile as compatible to form a stair.
		 */
		void addCompatibleTileForStair(uint16_t id);

		/**
		 * Whether another tile could follow this one in a trio.
		 */
		virtual bool isCompatibleForTrio(const Tile& o) const = 0;

		/**
		 * Mark the given tile as compatible to form a trio.
		 */
		void addCompatibleTileForTrio(uint16_t id);

		/**
		 * Returns a reference to the current object as a regular tile from the
		 * player's deck.
		 */
		virtual DeckRegularTile& asDeckRegularTile();

		/**
		 * Returns a reference to the current object as a regular tile from the
		 * table.
		 */
		virtual TableRegularTile& asTableRegularTile();

		/**
		 * Returns a reference to the current object as a joker tile from the
		 * table.
		 */
		virtual TableJokerTile& asTableJokerTile();

		/**
		 * Returns a const reference to the current object as a regular tile
		 * from the player's deck.
		 */
		virtual const DeckRegularTile& asDeckRegularTile() const;

		/**
		 * Returns a const reference to the current object as a regular tile
		 * from the table.
		 */
		virtual const TableRegularTile& asTableRegularTile() const;

		/**
		 * Returns a const reference to the current object as a joker tile from
		 * the table.
		 */
		virtual const TableJokerTile& asTableJokerTile() const;

	private:
		// Unique identifier of the tile
		uint16_t mId;
		// The type of the tile
		TileType mType;
		// A bitset in which the nth bit represents compatibility with the nth
		// tile to form a trio
		boost::dynamic_bitset<> mCompatibleForTrio;
		// A bitset in which the nth bit represents compatibility with the nth
		// tile to form a stair
		boost::dynamic_bitset<> mCompatibleForStair;
};

#endif /* RUMMY_TILE_H */
