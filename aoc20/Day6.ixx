export module Day6;

import <vector>;
import <string>;
import <array>;
import <algorithm>;

import Common;
import AoC;

static constexpr usize N = 6;

namespace AoC
{
	uint64 PartA(std::vector<std::string> const& _input)
	{
		std::array<uint32, 26> bitflags;
		std::generate(bitflags.begin(), bitflags.end(), [i = 0]() mutable { return (1 << i++); });

		uint64 count{ 0 };
		for (auto const& block : _input)
		{
			uint32 groupAnswers{ 0 };
			for (char const& c : block)
			{
				if (c != '\n')
				{
					groupAnswers |= bitflags[c - 'a'];
				}
			}

			count += std::popcount(groupAnswers);
		}

		return count;
	}

	uint64 PartB(std::vector<std::string> const& _input)
	{
		std::array<uint32, 26> bitflags;
		std::generate(bitflags.begin(), bitflags.end(), [i = 0]() mutable { return (1 << i++); });

		uint64 count{ 0 };
		for (auto const& block : _input)
		{
			uint32 groupAnswers{ static_cast<uint32>(-1) };
			uint32 currentPerson{ 0 };

			for (char const& c : block)
			{
				if (c != '\n')
				{
					currentPerson |= bitflags[c - 'a'];
				}
				else
				{
					groupAnswers &= currentPerson;
					currentPerson = 0;
				}
			}
			groupAnswers &= currentPerson;

			count += std::popcount(groupAnswers);
		}

		return count;
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("6 Read+Parse", 1).Run([]() { return Input(N).ToBlankSeparated(); });
		auto const resultA = Benchmark("6A").Run(PartA, input);
		auto const resultB = Benchmark("6B").Run(PartB, input);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}