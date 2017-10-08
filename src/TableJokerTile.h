#ifndef TABLE_JOKER_TILE_H
#define TABLE_JOKER_TILE_H

#include "Tile.h"
#include <boost/optional.hpp>
#include <set>

/**
 * Class representing a joker tile on the table.
 * These tiles might be locked into a certain configuration or not.
 */
class TableJokerTile : public Tile
{
	public:
		/**
		 * The group of a locked joker (whether it is locked in a stair or in a
		 * trio)
		 */
		enum LockMode
		{
			STAIR,
			TRIO
		};

		/**
		 * Parameters of a locked joker.
		 */
		struct LockParams
		{
			LockMode lockMode;
			uint8_t number;
			uint8_t color_mask;
		};

		/**
		 * Returns an unlocked joker.
		 */
		static std::unique_ptr<TableJokerTile> unlocked(uint16_t id);

		/**
		 * Returns a conditionally locked joker: it is only unlocked if a
		 * certain tile from the player's deck is played.
		 */
		static std::unique_ptr<TableJokerTile> conditionallyLockedInTrio(
				uint8_t number,
				uint8_t colorMask,
				uint16_t id,
				const std::set<uint16_t>& unlockingTilesIds);

		/**
		 * Returns a conditionally locked joker: it is only unlocked if a
		 * certain tile from the player's deck is played.
		 */
		static std::unique_ptr<TableJokerTile> conditionallyLockedInStair(
				uint8_t number,
				uint8_t colorMask,
				uint16_t id,
				const std::set<uint16_t>& unlockingTilesIds);

		/**
		 * Returns a locked joker which must remain as a particular tile and be
		 * used in a stair.
		 */
		static std::unique_ptr<TableJokerTile> lockedInStair(
				uint8_t number, uint8_t color, uint16_t id);

		/**
		 * Returns a locked joker which must remain as a particular tile and be
		 * used in a trio.
		 * Locked jokers in trios are locked into a single number but can
		 * potentially be locked into more than one color.
		 */
		static std::unique_ptr<TableJokerTile> lockedInTrio(
				uint8_t number, uint8_t colorMask, uint16_t id);

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
		 * Whether the joker is conditionally locked.
		 */
		bool isConditionallyLocked() const;

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

		/**
		 * If the joker is conditionally locked, it returns the set of tiles
		 * that should have been played to unlock it.
		 */
		const std::set<uint16_t>& unlockingTilesIds() const;

	private:
		TableJokerTile(uint16_t id);
		TableJokerTile(
				LockMode lock, uint8_t number,
				uint8_t color_mask, uint16_t id,
				const std::set<uint16_t>& unlockingTilesIds);

	private:
		boost::optional<LockParams> mLockParams;
		std::set<uint16_t> mUnlockingTilesIds;
};

#endif /* TABLE_JOKER_TILE_H */
