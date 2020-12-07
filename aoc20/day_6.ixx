export module day_6;

import std.core;

import common;

namespace aoc
{
	uint64 partA(std::vector<std::string> const& _input)
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

			count += __popcnt(groupAnswers);
		}

		return count;
	}

	uint64 partB(std::vector<std::string> const& _input)
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

			count += __popcnt(groupAnswers);
		}

		return count;
	}

	export std::string day_6()
	{
		auto const input = aoc::input(6).to_blank_separated();

		aoc::multi_timer timeA("6A");
		auto const outputA = timeA.run(partA, input);

		aoc::multi_timer timeB("6B");
		auto const outputB = timeB.run(partB, input);

		return nice_output(6, std::to_string(outputA), std::to_string(outputB));
	}
}