#ifndef TILE_COLOR_H
#define TILE_COLOR_H

#include <map>

namespace TileColor
{
enum Color
{
	BLACK  = 1,
	ORANGE = 1 << 1,
	BLUE   = 1 << 2,
	PINK    = 1 << 3
};

extern const uint8_t colorMask;

extern const std::map<char, Color> letterToColor;
extern const std::map<Color, char> colorToLetter;
}

#endif /* TILE_COLOR_H */
