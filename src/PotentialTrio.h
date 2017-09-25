#ifndef POTENTIAL_TRIO_H
#define POTENTIAL_TRIO_H

#include "PotentialGroup.h"
#include <boost/optional.hpp>

/**
 * A class representing a set of tiles which attempt to conform a trio.
 */
class PotentialTrio : public PotentialGroup
{
	public:
		PotentialTrio(const Tile& tile);

		/**
		 * Add a new tile to this trio.
		 */
		void addTile(const Tile& tile);
};

#endif /* POTENTIAL_TRIO_H */
