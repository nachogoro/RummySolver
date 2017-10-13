#ifndef SHUFFLING_TOOLS_H
#define SHUFFLING_TOOLS_H

#include "Tile.h"
#include "Group.h"
#include "GroupConfiguration.h"
#include <chrono>
#include <memory>
#include <vector>

namespace ShufflingTools
{
	/**
	 * Marks all the compatible tiles for each tile.
	 */
	void markCompatibleTiles(std::vector<std::reference_wrapper<Tile>>& tiles);


	/**
	 * Generates all possible groupings of tiles recursively.
	 * The resulting vector is sorted in descendent order using the score as
	 * criteria.
	 */
	std::vector<Group> getAllPossibleGroups(
			const std::vector<std::reference_wrapper<Tile>>& tiles);

	/**
	 * Returns the best configuration of the given groups (i.e. what groups
	 * should be played so that all the tiles in the table are used and the
	 * score of the tiles played from the deck is maximized).
	 * Since this is a very time consuming operation, an optional time limit
	 * can be set, which turns this method into a best-effort method.
	 */
	boost::optional<GroupConfiguration> getBestConfiguration(
			const std::vector<Group>& allGroups,
			boost::optional<std::chrono::duration<int>> duration);
}

#endif /* SHUFFLING_TOOLS_H */
