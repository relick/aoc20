export module Day17;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 17;

namespace AoC
{
	using intT = int64;

	struct CubeLoc
	{
		intT x;
		intT y;
		intT z;
		bool operator==(CubeLoc const& _o) const
		{
			return x == _o.x && y == _o.y && z == _o.z;
		}
	};

	struct CubeLoc4
	{
		intT x;
		intT y;
		intT z;
		intT w;
		bool operator==(CubeLoc4 const& _o) const
		{
			return x == _o.x && y == _o.y && z == _o.z && w == _o.w;
		}
	};

	enum class CubeState : uint8
	{
		Inactive,
		Active,
	};
}

namespace
{

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
    struct hash<AoC::CubeLoc>
    {
        size_t operator()(AoC::CubeLoc const& _cl) const
        {
            size_t seed = 0;
            hash_combine(seed, _cl.x);
            hash_combine(seed, _cl.y);
            hash_combine(seed, _cl.z);
            return seed;
        }

    };
	template<>
	struct hash<AoC::CubeLoc4>
	{
		size_t operator()(AoC::CubeLoc4 const& _cl) const
		{
			size_t seed = 0;
			hash_combine(seed, _cl.x);
			hash_combine(seed, _cl.y);
			hash_combine(seed, _cl.z);
			hash_combine(seed, _cl.w);
			return seed;
		}

	};
}

namespace AoC
{
	template<typename F>
	void ForEachAround(CubeLoc const& _loc, F&& _fn)
	{
		CubeLoc each{ _loc.x - 1, 0, 0, };
		for (; each.x <= _loc.x + 1; ++each.x)
		{
			for (each.y = _loc.y - 1; each.y <= _loc.y + 1; ++each.y)
			{
				for (each.z = _loc.z - 1; each.z <= _loc.z + 1; ++each.z)
				{
					if (each == _loc) { continue; }
					_fn(each);
				}
			}
		}
	}

	template<typename F>
	void ForEachAround(CubeLoc4 const& _loc, F&& _fn)
	{
		CubeLoc4 each{ _loc.x - 1, 0, 0, 0 };
		for (; each.x <= _loc.x + 1; ++each.x)
		{
			for (each.y = _loc.y - 1; each.y <= _loc.y + 1; ++each.y)
			{
				for (each.z = _loc.z - 1; each.z <= _loc.z + 1; ++each.z)
				{
					for (each.w = _loc.w - 1; each.w <= _loc.w + 1; ++each.w)
					{
						if (each == _loc) { continue; }
						_fn(each);
					}
				}
			}
		}
	}

	auto Parse(std::vector<std::string> const& _input)
	{
		std::unordered_map<CubeLoc, CubeState> cubes;
		std::unordered_map<CubeLoc4, CubeState> cubes4D;
		for (usize y = 0; y < _input.size(); ++y)
		{
			for (usize x = 0; x < _input[0].size(); ++x)
			{
				if (_input[y][x] == '#')
				{
					CubeLoc const loc = { static_cast<intT>(x), static_cast<intT>(y), 0 };
					cubes.insert_or_assign(loc, CubeState::Active);
					ForEachAround(loc, [&cubes](CubeLoc const& _o)
					{
						cubes.emplace(_o, CubeState::Inactive);
					});

					CubeLoc4 const loc4 = { static_cast<intT>(x), static_cast<intT>(y), 0, 0 };
					cubes4D.insert_or_assign(loc4, CubeState::Active);
					ForEachAround(loc4, [&cubes4D](CubeLoc4 const& _o)
					{
						cubes4D.emplace(_o, CubeState::Inactive);
					});
				}
			}
		}

		return std::pair(cubes, cubes4D);
	}

	auto PartA(std::unordered_map<CubeLoc, CubeState> _cubes)
	{
		std::unordered_map<CubeLoc, CubeState> cubes2 = _cubes;

		std::unordered_map<CubeLoc, CubeState>* readCubes = &_cubes;
		std::unordered_map<CubeLoc, CubeState>* writeCubes = &cubes2;

		for (usize runs = 0; runs < 6; ++runs)
		{
			for (auto const& [loc, state] : *readCubes)
			{
				uint8 numAround{ 0 };
				ForEachAround(loc, [&numAround, &readCubes](CubeLoc const& _o)
				{
					auto cubeI = readCubes->find(_o);
					numAround += (cubeI != readCubes->end() && cubeI->second == CubeState::Active);
				});
				switch (state)
				{
				using enum CubeState;
				case Active:
				{
					if (numAround != 2 && numAround != 3)
					{
						(*writeCubes)[loc] = Inactive;
					}
					else
					{
						(*writeCubes)[loc] = Active;
					}
					break;
				}
				case Inactive:
				{
					if (numAround == 3)
					{
						(*writeCubes)[loc] = Active;
						ForEachAround(loc, [&writeCubes](CubeLoc const& _o)
						{
							writeCubes->emplace(_o, CubeState::Inactive);
						});
					}
					else
					{
						(*writeCubes)[loc] = Inactive;
					}
					break;
				}
				}
			}
			std::swap(readCubes, writeCubes);
		}

		intT activeCount{ 0 };
		for(auto const& [loc, state] : *readCubes)
		{
			activeCount += state == CubeState::Active;
		}

		return activeCount;
	}

	auto PartB(std::unordered_map<CubeLoc4, CubeState> _cubes)
	{
		std::unordered_map<CubeLoc4, CubeState> cubes2 = _cubes;

		std::unordered_map<CubeLoc4, CubeState>* readCubes = &_cubes;
		std::unordered_map<CubeLoc4, CubeState>* writeCubes = &cubes2;

		for (usize runs = 0; runs < 6; ++runs)
		{
			for (auto const& [loc, state] : *readCubes)
			{
				uint8 numAround{ 0 };
				ForEachAround(loc, [&numAround, &readCubes](CubeLoc4 const& _o)
				{
					auto cubeI = readCubes->find(_o);
					numAround += (cubeI != readCubes->end() && cubeI->second == CubeState::Active);
				});
				switch (state)
				{
					using enum CubeState;
				case Active:
				{
					if (numAround != 2 && numAround != 3)
					{
						(*writeCubes)[loc] = Inactive;
					}
					else
					{
						(*writeCubes)[loc] = Active;
					}
					break;
				}
				case Inactive:
				{
					if (numAround == 3)
					{
						(*writeCubes)[loc] = Active;
						ForEachAround(loc, [&writeCubes](CubeLoc4 const& _o)
						{
							writeCubes->emplace(_o, CubeState::Inactive);
						});
					}
					else
					{
						(*writeCubes)[loc] = Inactive;
					}
					break;
				}
				}
			}
			std::swap(readCubes, writeCubes);
		}

		intT activeCount{ 0 };
		for (auto const& [loc, state] : *readCubes)
		{
			activeCount += state == CubeState::Active;
		}

		return activeCount;
	}

	export std::string Day17()
	{
		auto const input = Benchmark("17 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [cubes, cubes4D] = Benchmark("17 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("17A", 1).Run(PartA, cubes);
		auto const resultB = Benchmark("17B", 1).Run(PartB, cubes4D);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}