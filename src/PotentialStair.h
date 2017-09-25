#ifndef POTENTIAL_STAIR_H
#define POTENTIAL_STAIR_H

#include "PotentialGroup.h"
#include <boost/optional.hpp>

/**
 * A class representing a set of tiles which attempt to conform a stair.
 */
class PotentialStair : public PotentialGroup
{
	public:
		PotentialStair(const Tile& tile);

		/**
		 * Add a new tile to this stair.
		 */
		virtual void addTile(const Tile& tile);
};

#endif /* POTENTIAL_STAIR_H */
