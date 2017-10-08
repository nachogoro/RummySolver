#include "GroupConfiguration.h"
#include "GameInfo.h"
#include "TableJokerTile.h"
#include <map>
#include <set>

using LockMode = TableJokerTile::LockMode;
using LockParams = TableJokerTile::LockParams;

namespace
{
	unsigned int numberOfConditionallyLockedJokersUnlocked(
			const boost::dynamic_bitset<>& tiles)
	{
		const auto conditionallyLockedJokers
			= GameInfo::conditionallyLockedJokers();

		if (conditionallyLockedJokers.size() == 0)
		{
			return 0;
		}

		// Map from ID of a conditionally locked joker to the list of tiles
		// from the player's deck that have been played and unlock it
		std::map<uint16_t, std::set<uint16_t>> unlockingTiles;

		for (const auto& condJoker : conditionallyLockedJokers)
		{
			unlockingTiles[condJoker.get().id()];
			for (const auto unlockingId : condJoker.get().unlockingTilesIds())
			{
				if (tiles.test(unlockingId))
				{
					unlockingTiles[condJoker.get().id()].insert(unlockingId);
				}
			}
		}

		// If there is only one joker...
		if (conditionallyLockedJokers.size() == 1)
		{
			return unlockingTiles.begin()->second.size() ? 1 : 0;
		}

		// There are two table jokers in the game.
		// There is an edge case in which both jokers are unlocked by and only
		// by the same tile, so we cannot simply count the number of jokers
		// with at least one unlocking tile.
		const size_t numberOfJokersWithNoUnlockingTile = std::count_if(
				unlockingTiles.begin(),
				unlockingTiles.end(),
				[](const std::pair<uint16_t, std::set<uint16_t>>& x) -> bool
				{ return x.second.size() == 0; });

		if (numberOfJokersWithNoUnlockingTile > 0)
		{
			// At least one of the jokers is definitely locked, so we are not
			// in the edge case and we can simply count.
			return 2 - numberOfJokersWithNoUnlockingTile;
		}

		// At this point, there are two jokers and both of them have at least
		// one unlocking tile. If any of them has more than one unlocking tile,
		// both of them are unlocked.
		if (std::any_of(
				unlockingTiles.begin(),
				unlockingTiles.end(),
				[](const std::pair<uint16_t, std::set<uint16_t>>& x) -> bool
				{ return x.second.size() > 1; }))
		{
			return 2;
		}

		// At this point, we have two jokers and both of them have exactly one
		// unlocking tile. If they are unlocked by the same tile, only one of
		// them can really be unlocked.
		return
			(unlockingTiles.begin()->second != unlockingTiles.rbegin()->second)
				? 2 : 1;
	}

	std::vector<LockParams> getRoleOfJokersInGroup(const Group& /*group*/)
	{
		// TODO Implement
		return std::vector<LockParams>();
	}

	bool groupContainsAnyOf(const Group& group, const std::set<uint16_t>& ids)
	{
		for (const auto& id : ids)
		{
			if (group.tileIdsInGroup().test(id))
			{
				return true;
			}
		}
		return false;
	}

	bool doRolesMatch(
			const LockParams& tableRole,
			const TableJokerTile& joker)
	{
		const LockMode mode = joker.isLockedInTrio()
			? TableJokerTile::TRIO
			: TableJokerTile::STAIR;

		return mode == tableRole.lockMode
			&& joker.lockedNumber() == tableRole.number
			&& (joker.lockedColors() & tableRole.color_mask) != 0;
	}

	unsigned int numberOfConditionallyLockedJokersInTheirLockedPosition(
			const boost::dynamic_bitset<>& groups)
	{
		const auto conditionallyLockedJokers
			= GameInfo::conditionallyLockedJokers();

		std::set<uint16_t> jokerIds;
		for (const auto& joker : conditionallyLockedJokers)
		{
			jokerIds.insert(joker.get().id());
		}

		std::vector<LockParams> roleOfJokers;

		size_t groupId = groups.find_first();
		while (groupId != boost::dynamic_bitset<>::npos
				&& roleOfJokers.size() < conditionallyLockedJokers.size())
		{
			const Group& group = GameInfo::allGroups()[groupId].get();

			if (::groupContainsAnyOf(group, jokerIds))
			{
				const auto roles = ::getRoleOfJokersInGroup(group);
				roleOfJokers.insert(
						roleOfJokers.end(), roles.begin(), roles.end());
			}

			groupId = groups.find_next(groupId);
		}

		// If there is only one joker...
		if (conditionallyLockedJokers.size() == 1)
		{
			if (::doRolesMatch(
						roleOfJokers[0],
						conditionallyLockedJokers.begin()->get()))
			{
				return 1;
			}

			return 0;
		}

		// There are two conditionally locked jokers
		std::map<uint16_t, std::vector<size_t>> lockedRoles;

		for (const auto& joker : conditionallyLockedJokers)
		{
			lockedRoles[joker.get().id()];

			for (size_t i = 0; i < roleOfJokers.size(); ++i)
			{
				if (::doRolesMatch(roleOfJokers[i], joker.get()))
				{
					lockedRoles[joker.get().id()].push_back(i);
				}
			}
		}

		// There are two table jokers in the game.
		// There is an edge case in which both jokers remain locked by and only
		// by the same option, so we cannot simply count the number of jokers
		// with at least one locked position.
		const size_t numberOfJokersWithNoLockedOption = std::count_if(
				lockedRoles.begin(),
				lockedRoles.end(),
				[](const std::pair<
					uint16_t,
					std::vector<size_t>>& x)
				-> bool
				{ return x.second.size() == 0; });

		if (numberOfJokersWithNoLockedOption > 0)
		{
			// At least one of the jokers is not in its locked position, so we
			// are not in the edge case and we can simply count.
			return 2 - numberOfJokersWithNoLockedOption;
		}

		// At this point, there are two jokers and both of them have at least
		// one locked position. If any of them has more than one locked
		// position, both of them are unlocked.
		if (std::any_of(
				lockedRoles.begin(),
				lockedRoles.end(),
				[](const std::pair<
					uint16_t,
					std::vector<size_t>>& x)
				-> bool
				{ return x.second.size() > 1; }))
		{
			return 2;
		}

		// At this point, we have two jokers and both of them have exactly one
		// locked position tile. If they are unlocked by the same tile, only
		// one of them can really be in its locked position.
		return
			(lockedRoles.begin()->second[0] != lockedRoles.rbegin()->second[0])
				? 2 : 1;
	}
}

GroupConfiguration::GroupConfiguration(const Group& group)
	: mGroups(group.mCompatibleGroups.size()),
	  mTiles(group.mTileIdsInGroup),
	  mCompatibleGroups(group.compatibleGroups()),
	  mScore(group.score())
{
	mGroups.set(group.id());
}

bool GroupConfiguration::isValid() const
{
	// The configuration is valid if:
	// 1) All tiles from the table have been used AND
	// 2.1) The only table jokers are locked or inconditionally unlocked OR
	// 2.2) The conditionally locked jokers have been unlocked or kept in their
	//      original positions

	if ((mTiles & GameInfo::tableTilesMask()) != GameInfo::tableTilesMask())
	{
		return false;
	}

	const size_t conditionallyLockedJokers
		= GameInfo::conditionallyLockedJokers().size();

	if (conditionallyLockedJokers == 0)
	{
		return true;
	}

	const size_t conditionallyLockedJokersUnlocked
		= numberOfConditionallyLockedJokersUnlocked(mTiles);

	if (conditionallyLockedJokers == conditionallyLockedJokersUnlocked)
	{
		// All have been unlocked
		return true;
	}

	const size_t conditionallyLockedJokersInPosition
		= ::numberOfConditionallyLockedJokersInTheirLockedPosition(mGroups);

	return (conditionallyLockedJokersUnlocked
			+ conditionallyLockedJokersInPosition)
				== conditionallyLockedJokers;
}

uint16_t GroupConfiguration::score() const
{
	return mScore;
}

const boost::dynamic_bitset<>& GroupConfiguration::compatibleGroups() const
{
	return mCompatibleGroups;
}

void GroupConfiguration::addGroup(const Group& group)
{
	mGroups.set(group.id());
	mTiles |= group.mTileIdsInGroup;
	mCompatibleGroups &= group.compatibleGroups();
	mScore += group.score();
}
