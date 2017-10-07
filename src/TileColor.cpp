#include "TileColor.h"

const uint8_t TileColor::colorMask = static_cast<uint8_t>(0x1111);

const std::map<char, TileColor::Color> TileColor::letterToColor = {
	{'K', TileColor::BLACK},
	{'O', TileColor::ORANGE},
	{'B', TileColor::BLUE},
	{'R', TileColor::RED}
};

const std::map<TileColor::Color, char> TileColor::colorToLetter = {
	{TileColor::BLACK, 'K'},
	{TileColor::ORANGE, 'O'},
	{TileColor::BLUE, 'B'},
	{TileColor::RED, 'R'}
};
