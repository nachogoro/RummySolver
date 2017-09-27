#ifndef GROUP_H
#define GROUP_H

#include "PotentialGroup.h"

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
		 * Set the ID of this group as part of the larger set of n_of_groups
		 * groups.
		 */
		void setId(uint16_t id, uint16_t n_of_groups);
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
		 * The score of this group (i.e. the sum of the values of tiles which
		 * come from the player's deck)
		 */
		uint16_t score() const;

	private:
		boost::dynamic_bitset<> mTilesInGroup;
		uint16_t mId;
		boost::dynamic_bitset<> mCompatibleGroups;
		uint16_t mScore;

};

#endif /* GROUP_H */
