#ifndef TABLE_JOKER_TILE_H
#define TABLE_JOKER_TILE_H

#include "Tile.h"
#include <boost/optional.hpp>

/**
 * Class representing a joker tile on the table.
 * These tiles might be locked into a certain configuration or not.
 */
class TableJokerTile : public Tile
{
	public:
		/**
		 * Returns an unlocked joker.
		 */
		static TableJokerTile unlocked(uint16_t id, uint16_t n_of_tiles);

		/**
		 * Returns a locked joker which must remain as a particular tile and be
		 * used in a stair.
		 */
		static TableJokerTile lockedInStair(
				uint8_t number, uint8_t color,
				uint16_t id, uint16_t n_of_tiles);

		/**
		 * Returns a locked joker which must remain as a particular tile and be
		 * used in a trio.
		 * Locked jokers in trios are locked into a single number but can
		 * potentially be locked into more than one color.
		 */
		static TableJokerTile lockedInTrio(
				uint8_t number, uint8_t color_mask,
				uint16_t id, uint16_t n_of_tiles);

		/**
		 * Returns a reference to the current object as a regular tile from the
		 * player's deck.
		 */
		virtual TableJokerTile& asTableJokerTile();

		/**
		 * Returns a const reference to the current object as a regular tile
		 * from the player's deck.
		 */
		virtual const TableJokerTile& asTableJokerTile() const;

		/**
		 * Whether the specified tile could follow this one in a stair.
		 */
		virtual bool canBeFollowedInStairBy(const Tile& o) const;

		/**
		 * Whether the specified tile could follow this one in a trio.
		 */
		virtual bool isCompatibleForTrio(const Tile& o) const;

		/**
		 * Whether the joker is locked into a particular tile in general.
		 */
		bool isLocked() const;

		/**
		 * Whether the joker must remain as a particular tile in a stair.
		 */
		bool isLockedInStair() const;

		/**
		 * Whether the joker must remain as a particular tile in a trio.
		 */
		bool isLockedInTrio() const;

		/**
		 * The number to which the joker is locked to.
		 * Must only be called if the joker is locked; it throws otherwise.
		 */
		uint8_t lockedNumber() const;

		/**
		 * The OR mask of the colors to which the joker is locked to.
		 * Must only be called if the joker is locked; it throws otherwise.
		 */
		uint8_t lockedColors() const;

	private:
		enum LockMode
		{
			STAIR,
			TRIO
		};

		TableJokerTile(uint16_t id, uint16_t n_of_tiles);
		TableJokerTile(
				LockMode lock, uint8_t number, uint8_t color_mask,
				uint16_t id, uint16_t n_of_tiles);

	private:
		struct LockParams
		{
			LockMode lockMode;
			uint8_t number;
			uint8_t color_mask;
		};

		boost::optional<LockParams> mLockParams;
};

#endif /* TABLE_JOKER_TILE_H */
