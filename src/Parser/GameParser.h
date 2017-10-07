#ifndef GAME_PARSER_H
#define GAME_PARSER_H

#include "Tile.h"
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

namespace GameParser
{
	class Result
	{
		public:
			Result(
					std::vector<std::unique_ptr<Tile>> playerTiles,
					std::vector<std::unique_ptr<Tile>> tableTiles);

			const std::vector<std::unique_ptr<Tile>>& playerTiles() const;
			const std::vector<std::unique_ptr<Tile>>& tableTiles() const;
			const std::vector<std::reference_wrapper<Tile>>& allTiles() const;

		private:
			std::vector<std::unique_ptr<Tile>> mPlayerTiles;
			std::vector<std::unique_ptr<Tile>> mTableTiles;
			std::vector<std::reference_wrapper<Tile>> mAllTiles;
	};

	boost::optional<Result> parseGame(
			const boost::filesystem::path& playersDeckFile,
			const boost::filesystem::path& tableFile);
}

#endif /* GAME_PARSER_H */
