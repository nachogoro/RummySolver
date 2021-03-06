#include "TileColor.h"

const uint8_t TileColor::colorMask = static_cast<uint8_t>(0x000F);

const std::map<char, TileColor::Color> TileColor::letterToColor = {
	{'K', TileColor::BLACK},
	{'O', TileColor::ORANGE},
	{'B', TileColor::BLUE},
	{'P', TileColor::PINK}
};

const std::map<TileColor::Color, char> TileColor::colorToLetter = {
	{TileColor::BLACK, 'K'},
	{TileColor::ORANGE, 'O'},
	{TileColor::BLUE, 'B'},
	{TileColor::PINK, 'P'}
};
