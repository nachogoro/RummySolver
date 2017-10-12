#ifndef GROUP_CONFIGURATION_H
#define GROUP_CONFIGURATION_H

#include "Group.h"

/**
 * Class representing a configuration of compatible groups (trios and stairs)
 * that might represent a valid layout of the table.
 */
class GroupConfiguration
{
	public:
		GroupConfiguration(const Group& group);

		/**
		 * Whether the configuration is valid (i.e. all the tiles which were
		 * originally at the table are in use, and the table jokers have been
		 * used in a valid manner).
		 */
		bool isValid() const;

		/**
		 * The score of this configuration (i.e. the sum of the scores of the
		 * groups contained in it).
		 */
		uint16_t score() const;

		/**
		 * Returns a bitset in which the n-th bit represents the Group with ID
		 * n. If the bit is set, it means it is compatible with all the ones
		 * contained in this configuration.
		 */
		const boost::dynamic_bitset<>& compatibleGroups() const;

		/**
		 * Adds a new group to this configuration.
		 */
		void addGroup(const Group& group);

		friend std::ostream& operator<<(
				std::ostream& stream,
				const GroupConfiguration& g);

	private:
		// Groups in use in this configuration
		boost::dynamic_bitset<> mGroups;
		// Tiles in use in this configuration
		boost::dynamic_bitset<> mTiles;
		// Groups which could be added to this configuration
		boost::dynamic_bitset<> mCompatibleGroups;
		uint16_t mScore;
};

#endif /* GROUP_CONFIGURATION_H */
