export module Day24;

import <vector>;
import <string>;
import <unordered_set>;
import <unordered_map>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 24;

namespace AoC
{
	using intT = int32;
	using sizeT = usize;

	enum class Direction : uint8
	{
		E,
		SE,
		SW,
		W,
		NE,
		NW,
	};

	struct Pos
	{
		intT x;
		intT y;

		void Move(Direction const _d)
		{
			switch (_d)
			{
				using enum Direction;
			case E: x += 1; break;
			case SE: x += 1;
			case SW: y -= 1; break;
			case W: x -= 1; break;
			case NW: x -= 1;
			case NE: y += 1; break;
			}
		}

		bool operator==(Pos const& _o) const
		{
			return _o.x == x && _o.y == y;
		}
	};

	// Code from boost
	// Reciprocal of the golden ratio helps spread entropy
	//     and handles duplicates.
	// See Mike Seymour in magic-numbers-in-boosthash-combine:
	//     http://stackoverflow.com/questions/4948780

	template <class T>
	inline void hash_combine(std::size_t& seed, T const& v)
	{
		seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}

namespace std
{
	template<>
	struct hash<AoC::Pos>
	{
		size_t operator()(AoC::Pos const& _p) const
		{
			size_t seed = 0;
			AoC::hash_combine(seed, _p.x);
			AoC::hash_combine(seed, _p.y);
			return seed;
		}

	};
}

namespace AoC
{
	auto Parse(std::vector<std::string> const& _input)
	{
		std::vector<std::vector<Direction>> directions;
		directions.reserve(_input.size());
		for (auto const& line : _input)
		{
			directions.emplace_back();
			std::vector<Direction>& dirs = directions.back();
			dirs.reserve(line.size());
			char augment = 0;
			for (auto const c : line)
			{
				switch (c)
				{
				case 'w':
				{
					if (augment == 0)
					{
						dirs.emplace_back(Direction::W);
					}
					else if (augment == 'n')
					{
						dirs.emplace_back(Direction::NW);
					}
					else if (augment == 's')
					{
						dirs.emplace_back(Direction::SW);
					}
					augment = 0;
					break;
				}
				case 'e':
				{
					if (augment == 0)
					{
						dirs.emplace_back(Direction::E);
					}
					else if (augment == 'n')
					{
						dirs.emplace_back(Direction::NE);
					}
					else if (augment == 's')
					{
						dirs.emplace_back(Direction::SE);
					}
					augment = 0;
					break;
				}
				case 'n':
				case 's':
				{
					augment = c;
					break;
				}
				}
			}
		}

		return directions;
	}

	auto PartA(std::vector<std::vector<Direction>> const& _directions)
	{
		std::unordered_set<Pos> blackTiles;

		for (auto const& dirs : _directions)
		{
			Pos tile{ 0, 0 };
			for (auto const dir : dirs)
			{
				tile.Move(dir);
			}
			auto const inserted = blackTiles.insert(tile);
			if (!inserted.second)
			{
				blackTiles.erase(inserted.first);
			}
		}

		return std::pair(blackTiles.size(), blackTiles);
	}

	auto PartB(std::unordered_set<Pos> _blackTiles)
	{
		std::unordered_map<Pos, bool> activeTiles;
		activeTiles.reserve(_blackTiles.size() * 6);
		for (auto const& tile : _blackTiles)
		{
			activeTiles[tile] = true;
			for (uint8 i = 0; i < 6; ++i)
			{
				Pos p = tile;
				p.Move(static_cast<Direction>(i));
				activeTiles.emplace(p, false);
			}
		}
		std::unordered_map<Pos, bool> activeTiles2 = activeTiles;
		std::unordered_map<Pos, bool>* readTiles = &activeTiles;
		std::unordered_map<Pos, bool>* writeTiles = &activeTiles2;

		for (intT r = 0; r < 100; ++r)
		{
			for (auto const& [tile, active] : *readTiles)
			{
				uint8 neighbours{ 0 };
				for (uint8 i = 0; i < 6; ++i)
				{
					Pos p = tile;
					p.Move(static_cast<Direction>(i));
					auto const pI = readTiles->find(p);
					neighbours += pI != readTiles->end() && pI->second;
				}

				if (active && (neighbours == 1 || neighbours == 2))
				{
					writeTiles->insert_or_assign(tile, true);
				}
				else if (!active && neighbours == 2)
				{
					writeTiles->insert_or_assign(tile, true);
					for (uint8 i = 0; i < 6; ++i)
					{
						Pos p = tile;
						p.Move(static_cast<Direction>(i));
						writeTiles->emplace(p, false);
					}
				}
				else
				{
					writeTiles->insert_or_assign(tile, false);
				}
			}

			std::swap(readTiles, writeTiles);
		}

		uint64 blackTiles{ 0 };
		for (auto const& [tile, active] : *readTiles)
		{
			blackTiles += active;
		}

		return blackTiles;
	}

	export std::string Day24()
	{
		auto const input = Benchmark("24 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const directions = Benchmark("24 Parse").Run(Parse, input);
		auto const [resultA, blackTilesStart] = Benchmark("24A").Run(PartA, directions);
		auto const resultB = Benchmark("24B", 10).Run(PartB, blackTilesStart);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}