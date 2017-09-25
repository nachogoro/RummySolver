#ifndef POTENTIAL_GROUP_H
#define POTENTIAL_GROUP_H

#include "Tile.h"
#include <boost/optional.hpp>

/**
 * Base class for a set of tiles which attempt to conform a valid individual
 * group of tiles.
 */
class PotentialGroup
{
	public:
		PotentialGroup(
				const Tile& tile,
				const boost::dynamic_bitset<>& compatibleTiles);

		PotentialGroup(const PotentialGroup& o);

		virtual ~PotentialGroup();

		/**
		 * Add a new tile to this group.
		 */
		virtual void addTile(const Tile& tile) = 0;

		/**
		 * Whether the current group is valid (i.e. has more than 3 tiles).
		 */
		virtual bool isValid() const;

		/**
		 * Returns the ID of the next compatible tile which could be added to
		 * this tile or an empty optional if no more tiles are compatible.
		 * Consecutive calls to this method should yield different results
		 * until an empty optional is consistently returned.
		 */
		virtual boost::optional<uint16_t> nextCompatibleId();

	protected:
		std::vector<std::reference_wrapper<const Tile>> mTiles;
		boost::dynamic_bitset<> mCompatibleTiles;
	private:
		boost::optional<boost::dynamic_bitset<>::size_type> mLastReturnedId;
};

#endif /* POTENTIAL_GROUP_H */
