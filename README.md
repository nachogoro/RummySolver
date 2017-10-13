# RummySolver
## Introduction
RummySolver is a C++ tool to find the best possible move in a game of rummikub.

It receives the information about the state of the game from two text files and
finds the move that would maximize the points scored by the user.

## About Rummikub
[Rummikub](https://en.wikipedia.org/wiki/Rummikub) is a tile-based version of
the popular card game [Rummy](https://en.wikipedia.org/wiki/Rummy). In this
game, players are given a set of tiles and their aim is to get rid of them by
placing them on the table.

There are 106 tiles in a game: 104 of them represent a number and a color,
while the remaining two are jokers, which can potentially play the role of any
tile. There are four possible colors and 13 possible numbers, and each tile is
duplicated exactly once (`13 x 4 x 2 = 104`).

When it's the player's turn, they can place whichever tiles they want from
their hand on the table, as long as the tiles on the table when their turn is
over fulfil certain restrictions: they must be assorted into melds (or trios)
-groups of three or four tiles of the same number and different colors- or runs
(or stairs) -groups of three or more tiles of the same color and consecutive
numbers.

The general rules can be easily found online, so I will only specify here the
ones to which the tool adheres that tend to change the most between versions of
the game:
* Jokers are worth 50 points.
* If a joker is part of a group in the table, it remains locked in that
  position unless the player replaces it with a tile which plays a similar
  role. This tile can come from the player's deck or from the table.
* In a stair, a tile with number 13 can be followed by a tile with number 1,
  but that would be the end of the stair. That is: **if a stair contains a tile
  with number 1, this tile will either be the first tile or the last tile of the
  group**.

## Building the tool
Clone this git directory and then use `cmake` and `make`:
```
$ git clone https://github.com/nachogoro/RummySolver.git
$ cd RummySolver
$ mkdir bin
$ cd bin
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make all
```

## Using the tool
```
Allowed options:
  --help                   Print this message
  --deck arg (=deck.txt)   Player's deck tile file
  --table arg (=table.txt) Table tile file
  --time_limit arg (=120)  Time limit (in seconds) to find a solution
```

The game takes three parameters:
1. A file describing the tiles owned by the player.
2. A file describing the tiles laid on the table.
3. A timeout in seconds after which the tool will stop looking for better moves.

Tiles in each of the two files are described by a letter indicating the color
of the tile followed by its number (in case of a regular tile) or a letter `J`
in case of a joker.

The color codes are `O` for orange; `B` for blue; `K` for black; and `R` for
red.

In the case of the player's deck file, there must be one tile per line.

In the case of the table file, there must be one group per line. Tiles inside
the same group must be separated by a blank space. It is important for stairs
that they're described from left to right (i.e. in increasing order).

### Warning about usage
The tool makes use of special character codes to move the cursor in the
terminal, in order to provide a nice looking progress bar with updated
information on the current best configuration. This works fine in Linux
terminals but might fail in other systems.
