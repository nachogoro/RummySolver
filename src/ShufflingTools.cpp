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

	boost::optional<GroupConfiguration> findBestConfigurationRecursively(
			const GroupConfiguration& candidate,
			const boost::optional<GroupConfiguration>& currentBest,
			const std::vector<Group>& allGroups)
	{
		boost::optional<GroupConfiguration> result = currentBest;

		if (candidate.isValid()
				&& (!result || result->score() < candidate.score()))
		{
			result = candidate;
		}

		const auto& compatibleGroups = candidate.compatibleGroups();
		boost::dynamic_bitset<>::size_type nextGroup
			= compatibleGroups.find_first();
		while (nextGroup != boost::dynamic_bitset<>::npos)
		{
			GroupConfiguration copy(candidate);
			copy.addGroup(allGroups[nextGroup]);
			const auto newCandidate = ::findBestConfigurationRecursively(
					copy, result, allGroups);

			if (newCandidate && newCandidate->isValid()
					&& (!result || result->score() < newCandidate->score()))
			{
				result = newCandidate;
			}

			nextGroup = compatibleGroups.find_next(nextGroup);
		}

		return result;
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

	// Sort the groups by score in descending order
	std::sort(
			allGroups.begin(),
			allGroups.end(),
			[](const Group& a, const Group& b) -> bool
			{
				return a.score() > b.score();
			});

	// Assign an ID to each group
	uint16_t id = 0;
	for (auto& group : allGroups)
	{
		group.setId(id++);
	}

	// Make each group aware of other compatible groups
	for (size_t i = 0; i < allGroups.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < allGroups.size(); ++j)
		{
			allGroups[i].markGroupIfCompatible(allGroups[j]);
		}
	}

	return allGroups;
}

boost::optional<GroupConfiguration> ShufflingTools::getBestConfiguration(
		const std::vector<Group>& allGroups,
		boost::optional<std::chrono::duration<int>> /*limitInSecond*/)
{
	boost::optional<GroupConfiguration> bestConfig;

	for (size_t i = 0; i < allGroups.size(); ++i)
	{
		GroupConfiguration candidate(allGroups[i]);
		bestConfig = ::findBestConfigurationRecursively(
				candidate, bestConfig, allGroups);
	}
	return bestConfig;
}

