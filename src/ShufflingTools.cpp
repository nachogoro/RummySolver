#include "ShufflingTools.h"
#include "PotentialTrio.h"
#include "PotentialStair.h"

namespace
{
	/**
	 * Recursively generate all potential groups of a certain type.
	 * The parameter group must be pass by value so that nextCompatibleId()
	 * works correctly.
	 */
	template<typename GroupType>
	void generateAll(
			GroupType group,
			std::vector<Group>& allGroups,
			const std::vector<std::unique_ptr<Tile>>& allTiles)
	{
		if (group.isValid())
		{
			allGroups.push_back(group);
		}

		boost::optional<uint16_t> nextId = group.nextCompatibleId();

		while (nextId)
		{
			group.addTile(*allTiles[*nextId]);
			::generateAll(group, allGroups, allTiles);
			nextId = group.nextCompatibleId();
		}
	}
}

void ShufflingTools::markCompatibleTiles(
		std::vector<std::unique_ptr<Tile>>& tiles)
{
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		for (size_t j = 0; j < tiles.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			Tile& tile = *tiles[i];
			const Tile& targetTile = *tiles[j];

			if (tile.canBeFollowedInStairBy(targetTile))
			{
				tile.addCompatibleTileForStair(targetTile.id());
			}

			if (j > i && tile.isCompatibleForTrio(targetTile))
			{
				tile.addCompatibleTileForTrio(targetTile.id());
			}
		}
	}
}

std::vector<Group> ShufflingTools::getAllPossibleGroups(
		std::vector<std::unique_ptr<Tile>>& allTiles)
{
	std::vector<Group> allGroups;

	for (size_t i = 0; i < allTiles.size(); ++i)
	{
		PotentialTrio trio(std::cref(*allTiles[i]));
		::generateAll(trio, allGroups, allTiles);

		PotentialStair stair(std::cref(*allTiles[i]));
		::generateAll(stair, allGroups, allTiles);
	}

	return allGroups;
}
