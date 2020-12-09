export module Day9;

import std.core;

import common;

namespace aoc
{
	using int_t = int64;

	std::vector<int_t> Parse(std::vector<std::string> const& _input)
	{
		std::vector<int_t> nums;
		nums.reserve(_input.size());

		std::transform(_input.begin(), _input.end(), std::back_inserter(nums), [](std::string const& _line)
		{
			return util::qstoir<int_t>(_line);
		});

		return nums;
	}

	int_t PartA(std::vector<int_t> const& _nums)
	{
		for (usize i = 25; i < _nums.size(); ++i)
		{
			bool goodNum = false;
			for (usize l = i - 25; l < i; ++l)
			{
				for (usize r = l + 1; r < i; ++r)
				{
					if (_nums[l] != _nums[r] && (_nums[l] + _nums[r]) == _nums[i])
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

		auto min = _nums[l];
		auto max = min;

		for (usize i = l; i < r; ++i)
		{
			max = std::max(_nums[i], max);
			min = std::min(_nums[i], min);
		}

		return min + max;
	}

	export auto Day9()
	{
		auto const input = aoc::input(9).to_lines();

		aoc::multi_timer timeP("9 parse into nums");
		auto const nums = timeP.run(Parse, input);

		aoc::multi_timer timeA("9A");
		auto const resultA = timeA.run(PartA, nums);

		aoc::multi_timer timeB("9B");
		auto const resultB = timeB.run(PartB, nums, resultA);


		return nice_output(9, std::to_string(resultA), std::to_string(resultB));
	}
}