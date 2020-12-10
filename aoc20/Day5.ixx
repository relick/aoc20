export module Day5;

import std.core;

import Common;
import AoC;
import StrSplit;

static constexpr usize N = 5;

namespace AoC
{
	std::pair<std::string, std::string> Solution(std::vector<std::string> const& _input)
	{
		uint32 highestSeatId = 0;
		uint32 lowestSeatId = 0;
		uint32 mySeatId = 0;
		uint64 total = 0;
		uint64 actual = 0;
		{
			std::vector<uint32> seats;
			seats.reserve(_input.size());
			for (auto const& line : _input)
			{
				static const usize lineSize = line.size();
				static const usize lineSizeM1 = lineSize - 1;
				uint32 thisSeatId = 0;
				for (usize i = 0; i < lineSize; ++i)
				{
					if (line[i] == 'B' || line[i] == 'R')
					{
						thisSeatId += (1 << (lineSizeM1 - i));
					}
				}
				seats.emplace_back(thisSeatId);
				highestSeatId = std::max(highestSeatId, thisSeatId);
				lowestSeatId = std::min(lowestSeatId, thisSeatId);
				actual += thisSeatId;
			}
			total = ((highestSeatId - lowestSeatId) * (highestSeatId + lowestSeatId)) / 2;
			mySeatId = (uint32)(total - actual);
		}

		return { std::to_string(highestSeatId), std::to_string(mySeatId) };
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("5 Read", 1).Run([]() { return Input(N).ToBlankSeparated(); });
		auto const output = Benchmark("5 Solution").Run(Solution, input);

		return NiceOutput(N, output.first, output.second);
	}
}