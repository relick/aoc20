export module day_3;

import std.core;

import common;
import str_split;

namespace aoc
{
	usize check_slope(util::grid<char> const& _trees, usize _x_inc, usize _y_inc)
	{
		usize x{};
		usize y{};

		usize tree_count{};
		for (; y < _trees.height(); x += _x_inc, y += _y_inc)
		{
			tree_count += _trees.at(x % _trees.width(), y);
		}

		return tree_count;
	}

	std::pair<std::string, std::string> solution(std::vector<std::string> const& _input)
	{
		usize const width{ _input[0].size() };
		usize const height{ _input.size() };
		util::grid<char> trees(width, height, 0);

		for (usize y = 0; y < height; ++y)
		{
			for (usize x = 0; x < width; ++x)
			{
				if (_input[y][x] == '#')
				{
					trees.at(x, y) = 1;
				}
			}
		}

		usize const slope_a = check_slope(trees, 3, 1);
		std::string part_a = std::to_string(slope_a);

		std::string part_b = std::to_string(
			check_slope(trees, 1, 1)
			* slope_a
			* check_slope(trees, 5, 1)
			* check_slope(trees, 7, 1)
			* check_slope(trees, 1, 2)
		);

		return { part_a, part_b };
	}

	export std::string day_3()
	{
		auto const input = input::to_lines(3);
		auto const output = solution(input);
		return nice_output(3, output.first, output.second);
	}
}