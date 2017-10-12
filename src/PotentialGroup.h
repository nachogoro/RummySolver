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
	friend class Group;

	public:
	enum GroupType
	{
		TRIO,
		STAIR
	};

	PotentialGroup(
			const Tile& tile,
			const boost::dynamic_bitset<>& compatibleTiles,
			GroupType type);

	PotentialGroup(const PotentialGroup& o);

	virtual ~PotentialGroup();

	/**
	 * Add a new tile to this group.
	 */
	virtual void addTile(const Tile& tile);

	/**
	 * Whether the current group is valid (i.e. has more than 3 tiles).
	 */
	virtual bool isValid() const;

	/**
	 * The score of this group (i.e. the sum of the values of tiles which come
	 * from the player's deck)
	 */
	virtual uint16_t score() const;

	/**
	 * Returns a bitset in which the n-th bit represents the Tile with ID
	 * n. If the bit is set, it means it is compatible with all the ones
	 * contained in this group.
	 */
	const boost::dynamic_bitset<>& compatibleTiles() const;

	protected:
	// ID of the tiles which could be added to this group
	boost::dynamic_bitset<> mCompatibleTiles;
	// ID of the tiles inside this group
	boost::dynamic_bitset<> mTilesId;
	std::vector<std::reference_wrapper<const Tile>> mTiles;

	private:
	GroupType mType;
	uint16_t mScore;
};

#endif /* POTENTIAL_GROUP_H */
