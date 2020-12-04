export module day_4;

import std.core;
import std.regex;

import common;
import str_split;

namespace aoc
{
	struct passport
	{
		std::unordered_map<std::string, std::string> items;
	};

	std::pair<std::string, std::string> solution(std::vector<std::string> const& _input)
	{
		aoc::timer time;
		std::vector<passport> passports;
		// parse
		{
			passports.reserve(_input.size());
			for (auto const& block : _input)
			{
				passport currentPassport;
				for (auto const& keyPairStr : util::str_split(block, [](char const& _c) { return _c == ' ' || _c == '\n'; }))
				{
					auto keyPairSplit = util::str_split(keyPairStr, ':');
					auto iter = keyPairSplit.begin();
					auto const key = iter.next();
					auto const value = iter.next();
					currentPassport.items[std::string(key)] = value;
				}
				passports.push_back(currentPassport);
			}
		}
		time.end("parse");

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

			std::regex const byr{ "[0-9]{4}" };
			std::regex const iyr{ "[0-9]{4}" };
			std::regex const eyr{ "[0-9]{4}" };
			std::regex const hgt{ "([0-9]+)(cm|in)" };
			std::regex const hcl{ "#[0-9a-f]{6}" };
			std::regex const ecl{ "amb|blu|brn|gry|grn|hzl|oth" };
			std::regex const pid{ "[0-9]{9}" };

			for (auto const& passport : passports)
			{
				if (passport.items.size() >= 8)
				{
					numValidA++;
				}
				else if (passport.items.size() >= 7 && !passport.items.contains("cid"))
				{
					numValidA++;
				}
				else
				{
					continue;
				}

				std::smatch rmatches;
				if (
					std::regex_match(passport.items.at("byr"), rmatches, byr) && betweenIncl(std::stoi(rmatches[0].str()), 1920, 2002)
					&& std::regex_match(passport.items.at("iyr"), rmatches, iyr) && betweenIncl(std::stoi(rmatches[0].str()), 2010, 2020)
					&& std::regex_match(passport.items.at("eyr"), rmatches, eyr) && betweenIncl(std::stoi(rmatches[0].str()), 2020, 2030)
					&& std::regex_match(passport.items.at("hgt"), rmatches, hgt) && hgtPolicy(std::stoi(rmatches[1].str()), rmatches[2].str() == "cm")
					&& std::regex_match(passport.items.at("hcl"), hcl)
					&& std::regex_match(passport.items.at("ecl"), ecl)
					&& std::regex_match(passport.items.at("pid"), pid)
					)
				{
					numValidB++;
				}
			}

			part_a = std::to_string(numValidA);
			part_b = std::to_string(numValidB);
		}
		time.end("process");

		return { part_a, part_b };
	}

	export std::string day_4()
	{
		auto const input = aoc::input(4).to_blank_separated();
		auto const output = solution(input);
		return nice_output(4, output.first, output.second);
	}
}