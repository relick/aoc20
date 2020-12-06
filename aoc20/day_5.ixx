export module day_5;

import std.core;

import common;
import str_split;

namespace aoc
{
	std::pair<std::string, std::string> solution(std::vector<std::string> const& _input)
	{
		uint32 highestSeatId = 0;
		uint32 mySeatId = 0;
		{
			std::vector<uint32> seats;
			seats.reserve(_input.size());
			aoc::timer time;
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
				seats.push_back(thisSeatId);
			}

			std::sort(seats.begin(), seats.end());

			highestSeatId = seats.back();

			for (usize i = 0; i < seats.size() - 1; ++i)
			{
				if (seats[i] + 1 != seats[i + 1])
				{
					mySeatId = seats[i] + 1;
					break;
				}
			}

			time.end("process a+b");
		}

		std::vector<uint32> seats;
		seats.reserve(_input.size());

		auto process = [&_input, &seats]()
		{
			seats.clear();
			uint32 highestSeatId = 0;
			uint32 lowestSeatId = (uint32) -1;
			uint32 mySeatId = 0;
			uint64 total = 0;
			uint64 actual = 0;
			{
				//aoc::timer time;
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
				mySeatId = (uint32) (total - actual);

				//time.end("process a+b");
			}
		};

		aoc::multi_timer time(process, "process");
		time.run();

		return { std::to_string(highestSeatId), std::to_string(mySeatId) };
	}

	export std::string day_5()
	{
		auto const input = aoc::input(5).to_lines();
		auto const output = solution(input);
		return nice_output(5, output.first, output.second);
	}
}