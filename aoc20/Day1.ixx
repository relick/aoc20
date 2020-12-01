export module Day1;

import std.core;

import Common;

namespace aoc
{
	std::string partA(std::vector<int32> const& _input)
	{
		for (usize i = 0; i < _input.size(); ++i)
		{
			for (usize j = i + 1; j < _input.size(); ++j)
			{
				if ((_input[i] + _input[j]) == 2020)
				{
					return std::to_string(_input[i] * _input[j]);
				}
			}
		}
		return "FAIL";
	}

	std::string partB(std::vector<int32> const& _input)
	{
		for (usize i = 0; i < _input.size(); ++i)
		{
			for (usize j = i + 1; j < _input.size(); ++j)
			{
				for (usize k = j + 1; k < _input.size(); ++k)
				{
					if ((_input[i] + _input[j] + _input[k]) == 2020)
					{
						return std::to_string(_input[i] * _input[j] * _input[k]);
					}
				}
			}
		}
		return "FAIL";
	}

	export std::string day1()
	{
		auto const input = LoadToNumbers<int32>(1);
		return NiceOutput(1, partA(input), partB(input));
	}
}