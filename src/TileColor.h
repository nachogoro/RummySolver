#ifndef TILE_COLOR_H
#define TILE_COLOR_H

#include <map>

namespace TileColor
{
enum Color
{
	BLACK  = 0x0001,
	ORANGE = 0x0010,
	BLUE   = 0x0100,
	RED    = 0x1000
};

extern const uint8_t colorMask;

extern const std::map<char, Color> letterToColor;
extern const std::map<Color, char> colorToLetter;
}

#endif /* TILE_COLOR_H */
