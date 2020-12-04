export module day_4;

#include "ctre.hpp"
#include <vector>
#include <string>
//import std.core;

import common;
import str_split;

namespace aoc
{
	std::pair<std::string, std::string> solution(std::vector<std::string> const& _input)
	{
		aoc::timer time;

		std::string part_a;
		std::string part_b;
		{
			uint64 numValidA{};
			uint64 numValidB{};

			auto betweenIncl = [](auto const& _i, auto const& _l, auto const& _u) -> bool
			{
				return (_i <= _u) && (_i >= _l);
			};
			auto hgtPolicy = [&](auto const& _num, bool const& _isCM) -> bool
			{
				return betweenIncl(_num, _isCM ? 150 : 59, _isCM ? 193 : 76);
			};

			static constexpr auto byr = ctll::fixed_string{ "^[0-9]{4}$" };
			static constexpr auto iyr = ctll::fixed_string{ "^[0-9]{4}$" };
			static constexpr auto eyr = ctll::fixed_string{ "^[0-9]{4}$" };
			static constexpr auto hgt = ctll::fixed_string{ "^([0-9]+)(cm|in)$" };
			static constexpr auto hcl = ctll::fixed_string{ "^#[0-9a-f]{6}$" };
			static constexpr auto ecl = ctll::fixed_string{ "^(amb|blu|brn|gry|grn|hzl|oth)$" };
			static constexpr auto pid = ctll::fixed_string{ "^[0-9]{9}$" };

			for (auto const& block : _input)
			{
				uint8 validFieldsA = 0;
				uint8 validFieldsB = 0;

				for (auto const& keyPairStr : util::str_split(block, [](char const& _c) { return _c == ' ' || _c == '\n'; }))
				{
					auto keyPairSplit = util::str_split(keyPairStr, ':');
					auto iter = keyPairSplit.begin();
					auto const key = iter.next();
					auto const value = iter.next();

					if (key == "byr")
					{
						validFieldsA++;
						auto m = ctre::match<byr>(value);
						validFieldsB += (m && betweenIncl(util::svtoi<int32>(m.get<0>().to_view()), 1920, 2002)) ? 1 : 0;
					}
					else if (key == "iyr")
					{
						validFieldsA++;
						auto m = ctre::match<iyr>(value);
						validFieldsB += (m && betweenIncl(util::svtoi<int32>(m.get<0>().to_view()), 2010, 2020)) ? 1 : 0;
					}
					else if (key == "eyr")
					{
						validFieldsA++;
						auto m = ctre::match<eyr>(value);
						validFieldsB += (m && betweenIncl(util::svtoi<int32>(m.get<0>().to_view()), 2020, 2030)) ? 1 : 0;
					}
					else if (key == "hgt")
					{
						validFieldsA++;
						auto m = ctre::match<hgt>(value);
						validFieldsB += (m && hgtPolicy(util::svtoi<int32>(m.get<1>().to_view()), m.get<2>().to_view() == "cm")) ? 1 : 0;
					}
					else if (key == "hcl")
					{
						validFieldsA++;
						validFieldsB += (ctre::match<hcl>(value)) ? 1 : 0;
					}
					else if (key == "ecl")
					{
						validFieldsA++;
						validFieldsB += (ctre::match<ecl>(value)) ? 1 : 0;
					}
					else if (key == "pid")
					{
						validFieldsA++;
						validFieldsB += (ctre::match<pid>(value)) ? 1 : 0;
					}
				}

				if (validFieldsA == 7)
				{
					numValidA++;
				}

				if (validFieldsB == 7)
				{
					numValidB++;
				}
			}

			part_a = std::to_string(numValidA);
			part_b = std::to_string(numValidB);
		}
		time.end("parse + process");

		return { part_a, part_b };
	}

	export std::string day_4()
	{
		auto const input = aoc::input(4).to_blank_separated();
		auto const output = solution(input);
		return nice_output(4, output.first, output.second);
	}
}