export module Day20;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 20;

namespace AoC
{
	using intT = int32;
	
	static constexpr uint16 TEN_BITS = 0b0000001111111111;

	struct Tile
	{
		uint16 id;

		uint16 up;
		uint16 right;
		uint16 down;
		uint16 left;

		bool edge{ false };

		bool swapLR{ false };
		bool swapUD{ false };

		uint8 rotation{ 0 };

		bool Rotate()
		{
			// anti-clockwise
			RotateStripped();
			std::swap(up, right);
			std::swap(right, down);
			std::swap(down, left);
			rotation = rotation == 3 ? 0 : rotation+1;
			return rotation != 0;
		}

		bool RotateAndSwap()
		{
			if (!Rotate())
			{
				if (!swapLR && !swapUD)
				{
					SwapLR();
				}
				else if (swapLR && !swapUD)
				{
					SwapLR();
					SwapUD();
				}
				else if (!swapLR && swapUD)
				{
					SwapUD();
					return false;
				}
			}
			return true;
		}

		void SwapLR()
		{
			SwapLRStripped();
			std::swap(left, right);
			swapLR = !swapLR;
		}

		void SwapUD()
		{
			SwapUDStripped();
			std::swap(up, down);
			swapUD = !swapUD;
		}

		std::vector<std::string> strippedData;

		void RotateStripped()
		{
			// anti-clockwise
			// new vector is top-to-bottom right-to-left
			std::vector<std::string> newStrippedData;
			for (isize i = strippedData.size() - 1; i >= 0; --i)
			{
				newStrippedData.emplace_back();
				std::string& line = newStrippedData.back();
				for (isize j = 0; j < strippedData[0].size(); ++j)
				{
					line += strippedData[j][i];
				}
			}

			strippedData = std::move(newStrippedData);
		}

		void SwapLRStripped()
		{
			for (auto& line : strippedData)
			{
				for (usize i = 0; i < (line.size() / 2); ++i)
				{
					std::swap(line[i], line[line.size() - 1 - i]);
				}
			}
		}

		void SwapUDStripped()
		{
			for (usize i = 0; i < (strippedData.size() / 2); ++i)
			{
				std::swap(strippedData[i], strippedData[strippedData.size() - 1 - i]);
			}
		}
	};

	auto Parse(std::vector<std::string> const& _input)
	{
		std::vector<Tile> tiles;
		tiles.reserve(_input.size());
		std::unordered_map<uint16, uint16> borderMap;
		std::unordered_map<uint16, uint16> bordersSeen;
		borderMap.reserve(_input.size() * 8);

		for (auto const& tile : _input)
		{
			tiles.emplace_back();
			Tile& newTile = tiles.back();
			Tile flipped{};
			bool firstLine = true;
			usize lineNum = 0;
			for (auto const& line : util::str_split(tile, '\n'))
			{
				if (firstLine)
				{
					newTile.id = Util::QstoiL<uint16>(*(++util::str_split(line, ' ').begin()));
					firstLine = false;
					continue;
				}

				if (lineNum == 0)
				{
					// up border
					for (usize i = 0; i < line.size(); ++i)
					{
						if (line[i] == '#')
						{
							newTile.up |= 1 << (line.size() - 1 - i);
							flipped.up |= 1 << i;
						}
					}
				}

				{
					// left border
					if (line[0] == '#')
					{
						newTile.left |= 1 << lineNum;
						flipped.left |= 1 << (line.size() - 1 - lineNum);
					}
				}

				{
					// right border
					if (line.back() == '#')
					{
						newTile.right |= 1 << (line.size() - 1 - lineNum);
						flipped.right |= 1 << lineNum;
					}
				}

				if (lineNum == line.size() - 1)
				{
					// down border
					for (usize i = 0; i < line.size(); ++i)
					{
						if (line[i] == '#')
						{
							newTile.down |= 1 << i;
							flipped.down |= 1 << (line.size() - 1 - i);
						}
					}
				}

				if (lineNum > 0 && lineNum < line.size() - 1)
				{
					newTile.strippedData.emplace_back(line.substr(1, line.size() - 2));
				}

				++lineNum;
			}

			auto const upB = borderMap.emplace(newTile.up, newTile.up);
			auto const rightB = borderMap.emplace(newTile.right, newTile.right);
			auto const downB = borderMap.emplace(newTile.down, newTile.down);
			auto const leftB = borderMap.emplace(newTile.left, newTile.left);

			auto const upFB = borderMap.emplace(flipped.up, newTile.up);
			auto const rightFB = borderMap.emplace(flipped.right, newTile.right);
			auto const downFB = borderMap.emplace(flipped.down, newTile.down);
			auto const leftFB = borderMap.emplace(flipped.left, newTile.left);

			if (!upB.second || !upFB.second) { newTile.up = upB.first->second; }
			if (!rightB.second || !rightFB.second) { newTile.right = rightB.first->second; }
			if (!downB.second || !downFB.second) { newTile.down = downB.first->second; }
			if (!leftB.second || !leftFB.second) { newTile.left = leftB.first->second; }

			bordersSeen[newTile.up]++;
			bordersSeen[newTile.right]++;
			bordersSeen[newTile.down]++;
			bordersSeen[newTile.left]++;
		}

		for (auto& tile : tiles)
		{
			if (bordersSeen[tile.up] == 1
				|| bordersSeen[tile.right] == 1
				|| bordersSeen[tile.down] == 1
				|| bordersSeen[tile.left] == 1)
			{
				tile.edge = true;
			}
		}

		return tiles;
	}

	auto PartA(std::vector<Tile> const& _tiles)
	{
		// because no border is shared by more than 2 tiles, we can be certain that the 4 corners are
		// the 4 tiles with only 2 connections.

		/*std::array<uint64, 4> cornerTiles;
		usize cornerI = 0;
		for (usize tileI = 0; tileI < _tiles.size(); ++tileI)
		{
			uint16 const* tileArr = std::bit_cast<uint16 const*, TileBorders const*>(&(_tiles[tileI]));
			usize matchCount = 0;
			for (usize otherTileI = 0; otherTileI < _tiles.size(); ++otherTileI)
			{
				if (tileI != otherTileI)
				{
					uint16 const* otherTileArr = std::bit_cast<uint16 const*, TileBorders const*>(&(_tiles[otherTileI]));
					for (uint8 x = 0; x < 8; ++x)
					{
						for (uint8 y = 0; y < 8; ++y)
						{
							if (tileArr[x] == otherTileArr[y])
							{
								matchCount++;
								goto breakout;
							}
						}
					}
				breakout:;
				}
			}
			if (matchCount == 2)
			{
				cornerTiles[cornerI++] = _tileIDs[tileI];
			}
			else if (matchCount > 4)
			{
				std::cout << "ohno\n";
			}
		}

		return cornerTiles[0] * cornerTiles[1] * cornerTiles[2] * cornerTiles[3];*/
		return 0;
	}

	static usize constexpr sentinel = static_cast<usize>(-1);

	auto PartB(std::vector<Tile> _tiles)
	{
		std::vector<usize> remainingTiles(_tiles.size());
		std::iota(remainingTiles.begin(), remainingTiles.end(), 0);

		usize const width = (usize)sqrt(_tiles.size());
		Util::Grid<usize> tileGrid(width, width, sentinel);
		
		// Find first corner and its matching borders
		std::array<uint16, 4> matches;
		for (auto const tileI : remainingTiles)
		{
			usize nextMatch = 0;
			uint8 neighbours{ 0 };
			// only edges can be corners
			if (_tiles[tileI].edge)
			{
				for (auto const tileI2 : remainingTiles)
				{
					if (tileI != tileI2)
					{
						do
						{
							if (
								(_tiles[tileI].up == _tiles[tileI2].up)
								|| (_tiles[tileI].down == _tiles[tileI2].up)
								|| (_tiles[tileI].left == _tiles[tileI2].up)
								|| (_tiles[tileI].right == _tiles[tileI2].up)
								)
							{
								neighbours++;
								matches[nextMatch++] = _tiles[tileI2].up;
							}
						} while (_tiles[tileI2].Rotate());
					}
					if (neighbours > 2)
					{
						// can't be a corner
						break;
					}
				}

				if (neighbours == 2)
				{
					tileGrid.at(0, 0) = tileI;
					break;
				}
			}
		}

		remainingTiles.erase(std::remove(remainingTiles.begin(), remainingTiles.end(), tileGrid[0]));

		// orient corner correctly.
		while (matches[0] != _tiles[tileGrid[0]].right)
		{
			_tiles[tileGrid[0]].Rotate();
		}

		while (matches[1] != _tiles[tileGrid[0]].down)
		{
			_tiles[tileGrid[0]].SwapUD();
		}

		// fill top row
		for(usize x = 1; x < width; ++x)
		{
			Tile const& neighbour = _tiles[tileGrid.at(x - 1, 0)];
			bool found{ false };
			for (auto tileI = remainingTiles.begin(); tileI != remainingTiles.end(); ++tileI)
			{
				Tile& tile = _tiles[*tileI];
				if (tile.edge)
				{
					do
					{
						if (tile.left == neighbour.right)
						{
							found = true;
							break;
						}
					} while (tile.RotateAndSwap());

					if (found)
					{
						tileGrid.at(x, 0) = *tileI;
						remainingTiles.erase(tileI);
						break;
					}
				}
			}
		}

		// fill left column
		for (usize y = 1; y < width; ++y)
		{
			Tile const& neighbour = _tiles[tileGrid.at(0, y - 1)];
			bool found{ false };
			for (auto tileI = remainingTiles.begin(); tileI != remainingTiles.end(); ++tileI)
			{
				Tile& tile = _tiles[*tileI];
				if (tile.edge)
				{
					do
					{
						if (tile.up == neighbour.down)
						{
							found = true;
							break;
						}
					} while (tile.RotateAndSwap());

					if (found)
					{
						tileGrid.at(0, y) = *tileI;
						remainingTiles.erase(tileI);
						break;
					}
				}
			}
		}

		// fill remaining
		for (usize x = 1; x < width; ++x)
		{
			for (usize y = 1; y < width; ++y)
			{
				bool uEverOkay{ false };
				bool lEverOkay{ false };
				while (!uEverOkay || !lEverOkay)
				{
					Tile& neighbourU = _tiles[tileGrid.at(x, y - 1)];
					Tile& neighbourL = _tiles[tileGrid.at(x - 1, y)];
					bool found{ false };
					for (auto tileI = remainingTiles.begin(); tileI != remainingTiles.end(); ++tileI)
					{
						Tile& tile = _tiles[*tileI];
						do
						{
							bool const uOkay = (tile.up == neighbourU.down);
							bool const lOkay = (tile.left == neighbourL.right);
							uEverOkay |= uOkay;
							lEverOkay |= lOkay;
							if (uOkay && lOkay)
							{
								found = true;
								break;
							}
						} while (tile.RotateAndSwap());

						if (found)
						{
							tileGrid.at(x, y) = *tileI;
							remainingTiles.erase(tileI);
							break;
						}
					}

					if (!uEverOkay && y == 1)
					{
						neighbourU.SwapUD();
					}
					if (!lEverOkay && x == 1)
					{
						neighbourL.SwapLR();
					}
				}
			}
		}

		// build full image with rotated tiles
		Tile fullImage{};
		std::vector<std::string>& fullImageData = fullImage.strippedData;
		for (usize y = 0; y < width; ++y)
		{
			for (usize i = 0; i < 8; ++i)
			{
				fullImageData.emplace_back();
				std::string& line = fullImageData.back();
				for (usize x = 0; x < width; ++x)
				{
					usize const tileI = tileGrid.at(x, y);
					line += _tiles[tileI].strippedData[i];
				}
			}
		}

		// search for nessie
		std::array<std::string, 3> const nessie = {
			"                  # ",
			"#    ##    ##    ###",
			" #  #  #  #  #  #   ",
		};
		usize const nessieHeight = nessie.size();
		usize const nessieWidth = nessie[0].size();

		bool seaMonsterFound = false;
		do
		{
			for (usize y = 0; y < fullImageData.size() - nessieHeight; ++y)
			{
				for (usize x = 0; x < fullImageData.size() - nessieWidth; ++x)
				{
					bool match = true;
					for (usize h = 0; h < nessieHeight; ++h)
					{
						for (usize w = 0; w < nessieWidth; ++w)
						{
							if (nessie[h][w] == '#' && fullImageData[y + h][x + w] == '.')
							{
								match = false;
								goto NoMatchBreakout;
							}
						}
					}
				NoMatchBreakout:;

					if (match)
					{
						seaMonsterFound = true;

						for (usize h = 0; h < nessieHeight; ++h)
						{
							for (usize w = 0; w < nessieWidth; ++w)
							{
								if (nessie[h][w] == '#')
								{
									fullImageData[y + h][x + w] = '^'; // so we can just count # to find sea later
								}
							}
						}
					}
				}
			}
		} while (!seaMonsterFound && fullImage.RotateAndSwap());

		// count rough sea
		uint64 roughSea{ 0 };
		for (auto const& line : fullImageData)
		{
			for (auto const c : line)
			{
				if (c == '#')
				{
					roughSea++;
				}
			}
		}

		return roughSea;
	}

	export std::string Day20()
	{
		auto const input = Benchmark("20 Read", 1).Run([]() { return Input(N).ToBlankSeparated(); });
		auto const tiles = Benchmark("20 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("20A", 1).Run(PartA, tiles);
		auto const resultB = Benchmark("20B", 1).Run(PartB, tiles);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}