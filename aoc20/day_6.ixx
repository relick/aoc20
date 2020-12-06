export module day_6;

import std.core;

import common;
import str_split;

namespace aoc
{
	std::pair<std::string, std::string> solution(std::vector<std::string> const& _input)
	{
		uint64 countA2{ 0 };
		uint64 countB2{ 0 };
		aoc::timer100 time([&countA2, &countB2, &_input]()
		{
			uint64 countA = 0;
			uint64 countB = 0;
			std::array<uint32, 26> bitflags;
			for (uint32 i = 0; i < 26; ++i)
			{
				bitflags[i] = 1 << i;
			}
			for (auto const& block : _input)
			{
				uint32 thisBlockA{ 0 };
				uint32 thisBlockB{ 0 };
				uint32 currentPersonB{ 0 };

				bool first{ true };
				for (char const& c : block)
				{
					if (c != '\n')
					{
						uint32 const& n = bitflags[c - 'a'];
						thisBlockA |= n;
						currentPersonB |= n;
					}
					else
					{
						if (first)
						{
							thisBlockB = currentPersonB;
							first = false;
						}
						else
						{
							thisBlockB &= currentPersonB;
						}
						currentPersonB = 0;
					}
				}
				if (first)
				{
					thisBlockB = currentPersonB;
				}
				else
				{
					thisBlockB &= currentPersonB;
				}

				for (uint32 i = 0; i < 26; ++i)
				{
					uint32 const& n = bitflags[i];
					if ((thisBlockA & n) != 0)
					{
						countA++;
					}
					if ((thisBlockB & n) != 0)
					{
						countB++;
					}
				}
			}

			countA2 = countA;
			countB2 = countB;
		});
		time.run();

		return { std::to_string(countA2), std::to_string(countB2) };
	}

	export std::string day_6()
	{
		auto const input = aoc::input(6).to_blank_separated();
		auto const output = solution(input);
		return nice_output(6, output.first, output.second);
	}
}