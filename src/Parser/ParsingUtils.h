#ifndef PARSING_UTILS_H
#define PARSING_UTILS_H

#include "PotentialGroup.h"
#include "Tile.h"
#include "TileColor.h"
#include <boost/optional.hpp>
#include <fstream>
#include <string>

namespace ParsingUtils
{
	class TemporaryTile
	{
		public:
		static TemporaryTile joker();
		static TemporaryTile regular(uint8_t number, TileColor::Color color);

		bool isJoker() const;
		uint8_t number() const;
		TileColor::Color color() const;


		private:
		TemporaryTile(
				const boost::optional<
					std::pair<uint8_t, TileColor::Color>>& regularInfo);

		boost::optional<std::pair<uint8_t, TileColor::Color>> mRegularInfo;
	};

	boost::optional<std::vector<std::vector<TemporaryTile>>> parseFile(
			std::ifstream& input);

	boost::optional<std::vector<std::unique_ptr<Tile>>> temporaryTilesToTiles(
				const std::vector<std::vector<TemporaryTile>>& tmpTiles,
				const std::vector<std::unique_ptr<Tile>>& deckTiles);
}

#endif /* PARSING_UTILS_H */
