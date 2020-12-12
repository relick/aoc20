module;

#include <smmintrin.h>

export module Day12Intrin;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 12;

namespace AoC
{
	enum ActionType : int8
	{
		East,
		South,
		West,
		North,
		Left,
		Right,
		Forward,

		Count,
	};

	/*union alignas(16) iVec
	{
		struct
		{
			int x, y, z, w;
		};
		__m128i v;
	};*/
	using iVec = __m128i;
	static iVec zero{ _mm_set1_epi32(0) };

	struct Action
	{
		iVec dist;
		ActionType type;
		int8 dist2;

		Action() {}
		Action(iVec _dist, ActionType _type, int8 _dist2) : dist(_dist), type(_type), dist2(_dist2) {}
	};

	using ActionPtr = std::unique_ptr<Action>;

	std::pair<ActionPtr, usize> Parse(std::vector<std::string> const& _input)
	{
		ActionPtr actions((Action*)new Action[_input.size()]);
		Action* actionsP = actions.get();
		for (usize i = 0; auto const& line : _input)
		{
			uint16 dist = Util::QstoiR<uint16>(line);
			switch (line[0])
			{
			case 'N': actionsP[i] = Action(_mm_set_epi32(dist, dist, 0, 0), North,   0); break;
			case 'S': actionsP[i] = Action(_mm_set_epi32(dist, dist, 0, 0), South,   0); break;
			case 'E': actionsP[i] = Action(_mm_set_epi32(dist, dist, 0, 0), East,    0); break;
			case 'W': actionsP[i] = Action(_mm_set_epi32(dist, dist, 0, 0), West,    0); break;
			case 'L': actionsP[i] = Action(zero,    Left,    static_cast<int8>(dist / 90)); break;
			case 'R': actionsP[i] = Action(zero,    Right,   static_cast<int8>(dist / 90)); break;
			case 'F': actionsP[i] = Action(_mm_set_epi32(dist, dist, 0, 0), Forward, 0); break;
			}
			++i;
		}

		return { std::move(actions), _input.size() };
	}

	struct Ship
	{
		iVec pos{ _mm_set_epi32(0, 0, 0, 0) };
		int8 dir{ ActionType::East };
	};

	static std::array<iVec, 4> dirs{
		_mm_set_epi32(1, 0, 0, 0),
		_mm_set_epi32(0, -1, 0, 0),
		_mm_set_epi32(-1, 0, 0, 0),
		_mm_set_epi32(0, 1, 0, 0),
	};

	static __m128i negx{ _mm_set_epi32(-1, 1, 0, 0) };
	static __m128i negy{ _mm_set_epi32(1, -1, 0, 0) };

	static constexpr int swapxy = _MM_SHUFFLE(2, 3, 1, 0);

	uint64 PartA(Action const* _actions, usize const _numActions)
	{
		Ship ship;

		for (usize actionI = 0; actionI < _numActions; ++actionI)
		{
			auto const& action = _actions[actionI];
			switch (action.type)
			{
			case North:
			case South:
			case East:
			case West:
			{
				ship.pos = _mm_add_epi32(ship.pos, _mm_mullo_epi32(dirs[action.type], action.dist));
				break;
			}
			case Left:
			{
				ship.dir -= action.dist2;
				if (ship.dir < 0)
				{
					ship.dir += 4;
				}
				break;
			}
			case Right:
			{
				ship.dir += action.dist2;
				if (ship.dir >= 4)
				{
					ship.dir -= 4;
				}
				break;
			}
			case Forward:
			{
				ship.pos = _mm_add_epi32(ship.pos, _mm_mullo_epi32(dirs[ship.dir], action.dist));
				break;
			}
			}
		}

		return _mm_extract_epi32(_mm_hadd_epi32(_mm_abs_epi32(ship.pos), zero), 1);
	}

	uint64 PartB(Action const* _actions, usize const _numActions)
	{
		Ship ship;
		iVec wp{ _mm_set_epi32(10, 1, 0, 0) };

		for (usize actionI = 0; actionI < _numActions; ++actionI)
		{
			auto const& action = _actions[actionI];
			switch (action.type)
			{
			case North:
			case South:
			case East:
			case West:
			{
				wp = _mm_add_epi32(wp, _mm_mullo_epi32(dirs[action.type], action.dist));
				break;
			}
			case Left:
			{
				for (int8 i = 0; i < action.dist2; ++i)
				{
					wp = _mm_sign_epi32(_mm_shuffle_epi32(wp, swapxy), negx);
				}
				break;
			}
			case Right:
			{
				for (int8 i = 0; i < action.dist2; ++i)
				{
					wp = _mm_sign_epi32(_mm_shuffle_epi32(wp, swapxy), negy);
				}
				break;
			}
			case Forward:
			{
				ship.pos = _mm_add_epi32(ship.pos, _mm_mullo_epi32(wp, action.dist));
				break;
			}
			}
		}

		return _mm_extract_epi32(_mm_hadd_epi32(_mm_abs_epi32(ship.pos), zero), 1);
	}

	export std::string Day12Intrin()
	{
		auto const input = Benchmark("12 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [actions, numActions] = Benchmark("12 Parse into Actions").Run(Parse, input);
		auto const resultA = Benchmark("12A").Run(PartA, actions.get(), numActions);
		auto const resultB = Benchmark("12B").Run(PartB, actions.get(), numActions);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}