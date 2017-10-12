#ifndef GROUP_H
#define GROUP_H

#include "PotentialGroup.h"

using GroupType = PotentialGroup::GroupType;

/**
 * A class representing a set of tiles which form a valid individual
 * configuration.
 */
class Group
{
	friend class GroupConfiguration;
	public:
		Group(const PotentialGroup& g);

		/**
		 * Set the ID of this group.
		 */
		void setId(uint16_t id);

		/**
		 * Returns the ID of this group.
		 */
		uint16_t id() const;

		/**
		 * Check if the specified group is compatible with this one (i.e. no
		 * tile is being used by both of them at the same time), and mark it as
		 * a compatible group if it is.
		 */
		void markGroupIfCompatible(const Group& o);

		/**
		 * Returns a bitset in which the n-th bit represents the Group with ID
		 * n. If the bit is set, it means it is compatible with this one.
		 */
		const boost::dynamic_bitset<>& compatibleGroups() const;

		/**
		 * Returns a bitset in which the n-th bit represents the Tile with ID
		 * n. If the bit is set, it means the tile is in use in this group.
		 */
		const boost::dynamic_bitset<>& tileIdsInGroup() const;

		/**
		 * Returns all the tiles in the group in order.
		 */
		const std::vector<std::reference_wrapper<const Tile>>& tilesInGroup() const;

		/**
		 * The score of this group (i.e. the sum of the values of tiles which
		 * come from the player's deck)
		 */
		uint16_t score() const;

		/**
		 * Returns the type of the group.
		 */
		GroupType type() const;

		/**
		 * Returns a string representation of the group.
		 */
		std::string toString() const;

	private:
		GroupType mType;
		boost::dynamic_bitset<> mTileIdsInGroup;
		std::vector<std::reference_wrapper<const Tile>> mTilesInGroup;
		uint16_t mId;
		boost::dynamic_bitset<> mCompatibleGroups;
		uint16_t mScore;

};

#endif /* GROUP_H */
