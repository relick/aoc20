export module Day3;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 3;

namespace AoC
{
	uint64 CheckSlope(Util::Grid<bool> const& _trees, usize _x_inc, usize _y_inc)
	{
		usize x{};
		usize y{};

		uint64 tree_count{};
		for (; y < _trees.height(); x += _x_inc, x >= _trees.width() ? x -= _trees.width() : 0, y += _y_inc)
		{
			tree_count += _trees.at(x, y) ? 1 : 0;
		}

		return tree_count;
	}

	std::pair<std::string, std::string> Solution(std::vector<std::string> const& _input)
	{
		usize const width{ _input[0].size() };
		usize const height{ _input.size() };
		Util::Grid<bool> trees(width, height, 0);

		for (usize y = 0; y < height; ++y)
		{
			for (usize x = 0; x < width; ++x)
			{
				if (_input[y][x] == '#')
				{
					trees.at(x, y) = true;
				}
			}
		}

		uint64 const slope_a = CheckSlope(trees, 3, 1);
		std::string part_a = std::to_string(slope_a);
		
		std::string part_b = std::to_string(
			CheckSlope(trees, 1, 1)
			* slope_a
			* CheckSlope(trees, 5, 1)
			* CheckSlope(trees, 7, 1)
			* CheckSlope(trees, 1, 2)
		);

		return { part_a, part_b };
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("3 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const output = Benchmark("3 Solution").Run(Solution, input);

		return NiceOutput(N, output.first, output.second);
	}
}