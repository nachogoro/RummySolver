#ifndef SHUFFLING_TOOLS_H
#define SHUFFLING_TOOLS_H

#include "Tile.h"
#include "Group.h"
#include <memory>
#include <vector>

namespace ShufflingTools
{
	/**
	 * Marks all the compatible tiles for each tile.
	 */
	void markCompatibleTiles(std::vector<std::unique_ptr<Tile>>& tiles);


	/**
	 * Generates all possible groupings of tiles recursively.
	 */
	std::vector<Group> getAllPossibleGroups(
			std::vector<std::unique_ptr<Tile>>& tiles);
}

#endif /* SHUFFLING_TOOLS_H */
