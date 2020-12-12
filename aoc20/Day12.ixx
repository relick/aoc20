export module Day12;

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

	struct Action
	{
		ActionType type;
		int8 dist;
	};

	std::vector<Action> Parse(std::vector<std::string> const& _input)
	{
		std::vector<Action> actions;
		actions.reserve(_input.size());
		for (auto const& line : _input)
		{
			uint16 dist = Util::QstoiR<uint16>(line);
			switch (line[0])
			{
			case 'N': actions.emplace_back(North, static_cast<int8>(dist)); break;
			case 'S': actions.emplace_back(South, static_cast<int8>(dist)); break;
			case 'E': actions.emplace_back(East, static_cast<int8>(dist)); break;
			case 'W': actions.emplace_back(West, static_cast<int8>(dist)); break;
			case 'L': actions.emplace_back(Left, static_cast<int8>(dist / 90)); break;
			case 'R': actions.emplace_back(Right, static_cast<int8>(dist / 90)); break;
			case 'F': actions.emplace_back(Forward, static_cast<int8>(dist)); break;
			}
		}

		return actions;
	}

	template<std::integral I>
	struct Vec2
	{
		I x{ 0 };
		I y{ 0 };
	};

	template<std::integral I>
	struct Ship
	{
		Vec2<I> pos;
		int8 dir{ ActionType::East };
	};
	
	static constexpr std::array<Vec2<int32>, 4> dirs{
		 1,  0,
		 0, -1,
		-1,  0,
		 0,  1,
	};

	uint64 PartA(std::vector<Action> const& _actions)
	{
		Ship<int32> ship;

		for (auto const& action : _actions)
		{
			switch (action.type)
			{
			case North:
			case South:
			case East:
			case West:
			{
				ship.pos.x += dirs[action.type].x * action.dist;
				ship.pos.y += dirs[action.type].y * action.dist;
				break;
			}
			case Left:
			{
				ship.dir -= action.dist;
				if (ship.dir < 0)
				{
					ship.dir += 4;
				}
				break;
			}
			case Right:
			{
				ship.dir += action.dist;
				if (ship.dir >= 4)
				{
					ship.dir -= 4;
				}
				break;
			}
			case Forward:
			{
				ship.pos.x += dirs[ship.dir].x * action.dist;
				ship.pos.y += dirs[ship.dir].y * action.dist;
				break;
			}
			}
		}

		return std::abs(ship.pos.x) + std::abs(ship.pos.y);
	}

	uint64 PartB(std::vector<Action> const& _actions)
	{
		Ship<int32> ship;
		Vec2<int32> wp{ 10, 1 };

		for (auto const& action : _actions)
		{
			switch (action.type)
			{
			case North:
			case South:
			case East:
			case West:
			{
				wp.x += dirs[action.type].x * action.dist;
				wp.y += dirs[action.type].y * action.dist;
				break;
			}
			case Left:
			{
				for (int8 i = 0; i < action.dist; ++i)
				{
					std::swap(wp.x, wp.y);
					wp.x = -wp.x;
				}
				break;
			}
			case Right:
			{
				for (int8 i = 0; i < action.dist; ++i)
				{
					std::swap(wp.x, wp.y);
					wp.y = -wp.y;
				}
				break;
			}
			case Forward:
			{
				ship.pos.x += wp.x * action.dist;
				ship.pos.y += wp.y * action.dist;
				break;
			}
			}
		}

		return std::abs(ship.pos.x) + std::abs(ship.pos.y);
	}

	export std::string Day12()
	{
		auto const input = Benchmark("12 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const actions = Benchmark("12 Parse into Actions").Run(Parse, input);
		auto const resultA = Benchmark("12A").Run(PartA, actions);
		auto const resultB = Benchmark("12B").Run(PartB, actions);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}