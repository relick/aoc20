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

	struct TileBorders
	{
		// msb = leftmost of up, lsb = rightmost of up, others are relative.

		// border
		uint16 up{};
		uint16 right{};
		uint16 down{};
		uint16 left{};

		uint16 upF{};
		uint16 rightF{};
		uint16 downF{};
		uint16 leftF{};

		uint8 rotation{ 0 };
		void Rotate()
		{
			++rotation;
			auto swap = up;
			up = right;
			right = down;
			down = left;
			left = swap;

			swap = upF;
			upF = rightF;
			rightF = downF;
			downF = leftF;
			leftF = swap;

			if (rotation == 4)
			{
				// Into flip horiz
				std::swap(left, leftF);
				std::swap(right, rightF);
				std::swap(up, down);
				std::swap(upF, downF);
			}
			else if (rotation == 8)
			{
				// Into flip vert
				std::swap(left, leftF);
				std::swap(right, rightF);
				std::swap(up, down);
				std::swap(upF, downF);
				std::swap(up, upF);
				std::swap(down, downF);
				std::swap(left, right);
				std::swap(leftF, rightF);

			}
			else if (rotation == 12)
			{
				// Into unflipped
				std::swap(up, upF);
				std::swap(down, downF);
				std::swap(left, right);
				std::swap(leftF, rightF);
				rotation = 0;
			}
		}

		void Rotate(uint8 _r)
		{
			while (rotation != _r)
			{
				Rotate();
			}
		}

		void SwitchL()
		{
			// maintains up-down borders
			switch (rotation)
			{
			case 0: Rotate(6); break;
			case 1: Rotate(11); break;
			case 2: Rotate(4); break;
			case 3: Rotate(9); break;
			case 4: Rotate(2); break;
			case 5: break;
			case 6: Rotate(0); break;
			case 7: break;
			case 8: break;
			case 9: Rotate(3); break;
			case 10: break;
			case 11: Rotate(1); break;
			}
		}

		void SwitchU()
		{
			// maintains left-right borders
			switch (rotation)
			{
			case 0: Rotate(10); break;
			case 1: Rotate(7); break;
			case 2: Rotate(8); break;
			case 3: Rotate(5); break;
			case 4: break;
			case 5: Rotate(3); break;
			case 6: break;
			case 7: Rotate(1); break;
			case 8: Rotate(2); break;
			case 9: break;
			case 10: Rotate(0); break;
			case 11: break;
			}
		}
	};

	auto Parse(std::vector<std::string> const& _input)
	{
		std::vector<uint16> tileIDs;
		tileIDs.reserve(_input.size());
		std::vector<TileBorders> tileBorders;
		tileBorders.reserve(_input.size());
		std::unordered_map<uint16, usize> borderSeenBefore;
		borderSeenBefore.reserve(_input.size() * 8);

		for (auto const& tile : _input)
		{
			tileBorders.emplace_back();
			TileBorders& newTile = tileBorders.back();
			bool firstLine = true;
			usize lineNum = 0;
			for (auto const& line : util::str_split(tile, '\n'))
			{
				if (firstLine)
				{
					tileIDs.emplace_back(Util::QstoiL<uint16>(*(++util::str_split(line, ' ').begin())));
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
							newTile.upF |= 1 << i;
						}
					}
				}

				{
					// left border
					if (line[0] == '#')
					{
						newTile.left |= 1 << lineNum;
						newTile.leftF |= 1 << (line.size() - 1 - lineNum);
					}
				}

				{
					// right border
					if (line.back() == '#')
					{
						newTile.right |= 1 << (line.size() - 1 - lineNum);
						newTile.rightF |= 1 << lineNum;
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
							newTile.downF |= 1 << (line.size() - 1 - i);
						}
					}
				}

				++lineNum;
			}

			if (
				(borderSeenBefore[newTile.up]++ == 2)
				|| (borderSeenBefore[newTile.upF]++ == 2)
				|| (borderSeenBefore[newTile.right]++ == 2)
				|| (borderSeenBefore[newTile.rightF]++ == 2)
				|| (borderSeenBefore[newTile.down]++ == 2)
				|| (borderSeenBefore[newTile.downF]++ == 2)
				|| (borderSeenBefore[newTile.left]++ == 2)
				|| (borderSeenBefore[newTile.leftF]++ == 2)
				)
			{
				// NB this never gets hit, implying that no border is shared by more than 2 tiles.
				std::cout << "seen before!\n";
			}
		}

		return std::pair(tileIDs, tileBorders);
	}

	auto PartA(std::vector<uint16> const& _tileIDs, std::vector<TileBorders> const& _tiles)
	{
		// because no border is shared by more than 2 tiles, we can be certain that the 4 corners are
		// the 4 tiles with only 2 connections.

		std::array<uint64, 4> cornerTiles;
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

		return cornerTiles[0] * cornerTiles[1] * cornerTiles[2] * cornerTiles[3];
	}

	auto PartB(std::vector<uint16> const& _tileIDs, std::vector<TileBorders> _tiles)
	{
		static usize constexpr sentinel = static_cast<usize>(-1);
		usize const width = (usize)sqrt(_tiles.size());
		Util::Grid<usize> tileGrid(width, width, sentinel);
		
		// Find first corner and orient correctly.
		for (usize tileI = 0; tileI < _tiles.size(); ++tileI)
		{
			std::array<uint16, 4> matches{ 0 };
			usize i = 0;
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
								matches[i++] = tileArr[x];
								goto breakout;
							}
						}
					}
				breakout:;
				}
			}
			if (matchCount == 2)
			{
				while (!(
					(matches[0] == _tiles[tileI].right && matches[1] == _tiles[tileI].down) ||
					(matches[1] == _tiles[tileI].right && matches[0] == _tiles[tileI].down)
					))
				{
					_tiles[tileI].Rotate();
				}
				tileGrid[0] = tileI;
				break;
			}
		}

		// build grid
		for (usize index = 0, y = 0; y < width; ++y)
		{
			for (usize x = 0; x < width; ++x, ++index)
			{
				if (x == 0 && y == 0)
				{
					continue; // first corner filled already.
				}
				else
				{
				tryAgain:;
					usize const& neighbourL = x > 0 ? tileGrid.at(x - 1, y) : tileGrid[0];
					usize const& neighbourU = y > 0 ? tileGrid.at(x, y - 1) : tileGrid[0];
					//TileBorders& lBorder = _tiles[neighbourL];
					//TileBorders& uBorder = _tiles[neighbourU];
					bool leftEverOkay = false;
					bool upEverOkay = false;
					for (usize tileI = 0; tileI < _tiles.size(); ++tileI)
					{
						if (tileI != neighbourL && tileI != neighbourU)
						{
							for (usize i = 0; i < 12; ++i)
							{
								bool const leftOkay = (x == 0 || _tiles[tileI].leftF == _tiles[neighbourL].right);
								bool const upOkay = (y == 0 || _tiles[tileI].upF == _tiles[neighbourU].down);
								leftEverOkay |= leftOkay;
								upEverOkay |= upOkay;
								if (leftOkay && upOkay)
								{
									tileGrid.at(x, y) = tileI;
									goto breakout4;
								}
								_tiles[tileI].Rotate();
							}
						}
					}
				breakout4:;
					if (tileGrid.at(x, y) == sentinel)
					{
						if (x == 1 && !leftEverOkay)
						{
							_tiles[neighbourL].SwitchL();
						}
						if (y == 1 && !upEverOkay)
						{
							_tiles[neighbourU].SwitchU();
						}
						goto tryAgain;
					}
				}
			}
		}

		return 0;
	}

	export std::string Day20()
	{
		auto const input = Benchmark("20 Read", 1).Run([]() { return Input(N).ToBlankSeparated(); });
		auto const [tileIDs, tileBorders] = Benchmark("20 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("20A", 1).Run(PartA, tileIDs, tileBorders);
		auto const resultB = Benchmark("20B", 1).Run(PartB, tileIDs, tileBorders);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}