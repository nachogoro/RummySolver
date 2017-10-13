#include "ShufflingTools.h"
#include "GameInfo.h"
#include "PotentialStair.h"
#include "PotentialTrio.h"
#include "ProgressBar.h"
#include <iostream>

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
			const std::vector<std::reference_wrapper<Tile>>& allTiles)
	{
		if (group.isValid())
		{
			allGroups.push_back(group);
		}

		const auto& compatibleTiles = group.compatibleTiles();
		boost::dynamic_bitset<>::size_type nextTile
			= compatibleTiles.find_first();

		while (nextTile != boost::dynamic_bitset<>::npos)
		{
			GroupType copy(group);
			copy.addTile(allTiles[nextTile]);
			::generateAll(copy, allGroups, allTiles);
			nextTile = compatibleTiles.find_next(nextTile);
		}
	}

	boost::optional<GroupConfiguration> findBestConfigurationRecursively(
			const GroupConfiguration& candidate,
			const boost::optional<GroupConfiguration>& currentBest,
			const std::vector<Group>& allGroups)
	{
		boost::optional<GroupConfiguration> result = currentBest;

		if ((!result || result->score() < candidate.score())
				&& candidate.isValid())
		{
			result = candidate;
		}

		const auto& compatibleGroups = candidate.compatibleGroups();
		boost::dynamic_bitset<>::size_type nextGroup
			= compatibleGroups.find_first();
		while (nextGroup != boost::dynamic_bitset<>::npos)
		{
			// If the current configuration is now only checking against groups
			// of score zero and its score is already below the current best
			// one, no need to keep checking
			if (allGroups[nextGroup].score() == 0 && result && candidate.score() <= result->score())
			{
				break;
			}

			GroupConfiguration copy(candidate);
			copy.addGroup(allGroups[nextGroup]);
			const auto newCandidate = ::findBestConfigurationRecursively(
					copy, result, allGroups);

			if (newCandidate
					&& (!result || result->score() < newCandidate->score())
					&& newCandidate->isValid())
			{
				result = newCandidate;
				//std::cout << "Found temporary candidate with score: " << newCandidate->score() << std::endl;
			}

			nextGroup = compatibleGroups.find_next(nextGroup);
		}

		return result;
	}
}

void ShufflingTools::markCompatibleTiles(
		std::vector<std::reference_wrapper<Tile>>& tiles)
{
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		for (size_t j = 0; j < tiles.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			Tile& tile = tiles[i];
			const Tile& targetTile = tiles[j];

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
		const std::vector<std::reference_wrapper<Tile>>& allTiles)
{
	std::vector<Group> allGroups;

	for (size_t i = 0; i < allTiles.size(); ++i)
	{
		PotentialTrio trio(std::cref(allTiles[i]));
		::generateAll(trio, allGroups, allTiles);

		PotentialStair stair(std::cref(allTiles[i]));
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

	// Update GameInfo BEFORE setting the IDs
	for (const auto& group : allGroups)
	{
		GameInfo::mAllGroups.push_back(std::cref(group));
	}

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

	// Groups are sorted by score. If the first group has a score of
	// zero, nothing with positive score will come off it, so need to try them
	const size_t lastIndex = std::distance(
			allGroups.begin(),
			std::find_if(
				allGroups.begin(),
				allGroups.end(),
				[](const Group& g) { return g.score() == 0; }));

	std::cout << "Looking for a valid configuration...\n";
	ProgressBar::printBar(0);

	for (size_t i = 0; i < lastIndex; ++i)
	{
		GroupConfiguration candidate(allGroups[i]);

		auto updatedBestConfig = ::findBestConfigurationRecursively(
				candidate, bestConfig, allGroups);

		if ((!bestConfig && updatedBestConfig)
				|| (updatedBestConfig && bestConfig
					&& updatedBestConfig->score() > bestConfig->score()))
		{
			ProgressBar::printMessage("Found new configuration with score: " + std::to_string(updatedBestConfig->score()));
		}

		bestConfig = updatedBestConfig;

		ProgressBar::printBar((static_cast<unsigned int>(static_cast<double>(i)/lastIndex*100)));

		//++show_progress;
	}
	ProgressBar::printBar(100);
	std::cout << std::string(4, '\n');
	return bestConfig;
}

