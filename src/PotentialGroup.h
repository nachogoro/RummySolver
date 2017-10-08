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
	 * Returns the ID of the next compatible tile which could be added to this
	 * tile or an empty optional if no more tiles are compatible.
	 * Consecutive calls to this method should yield different results until an
	 * empty optional is consistently returned.
	 */
	virtual boost::optional<uint16_t> nextCompatibleId();

	protected:
	// ID of the tiles which could be added to this group
	boost::dynamic_bitset<> mCompatibleTiles;

	private:
	GroupType mType;
	// ID of the tiles inside this group
	boost::dynamic_bitset<> mTilesId;
	std::vector<std::reference_wrapper<const Tile>> mTiles;
	uint16_t mScore;
	boost::optional<boost::dynamic_bitset<>::size_type> mLastReturnedId;
};

#endif /* POTENTIAL_GROUP_H */
