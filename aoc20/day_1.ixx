export module day_1;

import std.core;

import common;

namespace aoc
{
	std::string part_a(std::vector<int32> const& _input)
	{
		std::array<bool, 2020> a{ false };
		for (int32 const num : _input)
		{
			if (num < 2020)
			{
				if (a[2020 - num])
				{
					return std::to_string(num * (2020 - num));
				}
				else
				{
					a[num] = true;
				}
			}
		}
		return "FAIL";
	}

	std::string part_b(std::vector<int32> const& _input)
	{
		for (usize i = 0; i < _input.size(); ++i)
		{
			for (usize j = i + 1; j < _input.size(); ++j)
			{
				int32 const iandj = _input[i] + _input[j];
				if (iandj <= 2020)
				{
					for (usize k = j + 1; k < _input.size(); ++k)
					{
						if ((iandj + _input[k]) == 2020)
						{
							return std::to_string(_input[i] * _input[j] * _input[k]);
						}
					}
				}
			}
		}
		return "FAIL";
	}

	export std::string day1()
	{
		auto const input = aoc::input(1).to_numbers<int32>();
		return nice_output(1, part_a(input), part_b(input));
	}
}