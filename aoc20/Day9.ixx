export module Day9;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 9;

namespace AoC
{
	using int_t = uint32;

	std::vector<int_t> Parse(std::vector<std::string> const& _input)
	{
		std::vector<int_t> nums;
		nums.reserve(_input.size());

		for (auto const& line : _input)
		{
			nums.emplace_back(Util::QstoiR<int_t>(line));
		}

		return nums;
	}

	int_t PartA(std::vector<int_t> const& _nums)
	{
		for (usize i = 25; i < _nums.size(); ++i)
		{
			bool goodNum = false;
			for (usize l = i - 25; l < i; ++l)
			{
				if (_nums[i] == 2 * _nums[l])
				{
					continue;
				}
				auto const sub = _nums[i] - _nums[l];
				for (usize r = l + 1; r < i; ++r)
				{
					if (sub == _nums[r])
					{
						goodNum = true;
						goto breakout;
					}
				}
			}
		breakout:
			if (!goodNum)
			{
				return _nums[i];
			}
		}

		return 0;
	}

	int_t PartB(std::vector<int_t> const& _nums, int_t _resultA)
	{
		usize l = 0;
		usize r = 1;
		int_t sum = _nums[0];

		while (sum != _resultA)
		{
			while (sum < _resultA)
			{
				sum += _nums[r];
				++r;
			}

			while (sum > _resultA)
			{
				sum -= _nums[l];
				++l;
			}
		}

		auto const min = *std::min_element(_nums.begin() + l, _nums.begin() + r);
		auto const max = *std::max_element(_nums.begin() + l, _nums.begin() + r);

		return min + max;
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("9 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const nums = Benchmark("9 Parse into nums").Run(Parse, input);
		auto const resultA = Benchmark("9A").Run(PartA, nums);
		auto const resultB = Benchmark("9B").Run(PartB, nums, resultA);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}