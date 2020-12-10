export module Day10;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 10;

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

		std::sort(nums.begin(), nums.end());

		return nums;
	}

	usize PartA(std::vector<int_t> const& _nums)
	{
		std::array<usize, 4> diffs{ 0 }; // ignore 0, diffs 1-3 index directly.

		diffs[_nums[0]]++; // outlet to first adapter

		for (usize i = 0; i < _nums.size() - 1; ++i)
		{
			diffs[_nums[i + 1] - _nums[i]]++;
		}
		
		diffs[3]++; // last adapter into device

		return diffs[1] * diffs[3];
	}

	usize CalculateNumWays(usize const _seq1s)
	{
		static constexpr std::array<usize, 5> numWaysFor{
			1,
			1,
			2, // first 3 initial
			4, // 1 + 1 + 2
			7, // 1 + 2 + 4
			// remainder can be calculated
		};

		// if precalced just return
		if (numWaysFor.size() > _seq1s)
		{
			return numWaysFor[_seq1s];
		}

		// otherwise we can figure it out
		usize prev3 = numWaysFor[numWaysFor.size() - 3];
		usize prev2 = numWaysFor[numWaysFor.size() - 2];
		usize prev1 = numWaysFor[numWaysFor.size() - 1];
		usize current = prev3 + prev2 + prev1;
		usize index = numWaysFor.size();

		while (index < _seq1s)
		{
			prev3 = prev2;
			prev2 = prev1;
			prev1 = current;
			current = prev3 + prev2 + prev1;
			++index;
		}

		return current;
	}

	usize PartB(std::vector<int_t> const& _nums)
	{
		usize seq1s{ 0 };
		usize numWays{ 1 }; // always 1 way, we'll multiply this

		if (_nums[0] == 1) { seq1s++; } // outlet to first adapter

		for (usize i = 0; i < _nums.size() - 1; ++i)
		{
			if (_nums[i + 1] - _nums[i] == 1)
			{
				seq1s++;
			}
			else
			{
				numWays *= CalculateNumWays(seq1s);
				seq1s = 0;
			}
		}

		numWays *= CalculateNumWays(seq1s); // last adapter into device

		return numWays;
	}

	export std::string Day10()
	{
		auto const input = Benchmark("10 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const nums = Benchmark("10 Parse and Sort").Run(Parse, input);
		auto const resultA = Benchmark("10A").Run(PartA, nums);
		auto const resultB = Benchmark("10B").Run(PartB, nums);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}