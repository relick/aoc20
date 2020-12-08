export module day_2;

import std.core;

import common;
import str_split;

namespace aoc
{
	usize part_a(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const lowerBound = util::qstoir<int32>(iter.next());
			auto const upperBound = util::qstoir<int32>(iter.next());
			char const checkChar = iter.next()[0];
			std::string_view const testStr = iter.next();

			usize numFound{};
			for (char const c : testStr)
			{
				if (c == checkChar)
				{
					++numFound;
				}
			}

			if (numFound >= lowerBound && numFound <= upperBound)
			{
				++validPasswords;
			}
		}
		return validPasswords;
	}

	usize part_b(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const firstIndex = util::qstoir<usize>(iter.next()) - 1u;
			auto const lastIndex = util::qstoir<usize>(iter.next()) - 1u;
			char const checkChar = iter.next()[0];
			std::string_view const testStr = iter.next();

			if ((checkChar == testStr[firstIndex]) ^ (checkChar == testStr[lastIndex]))
			{
				++validPasswords;
			}
		}
		return validPasswords;
	}

	export std::string day2()
	{
		auto const input = aoc::input(2).to_lines();

		auto const timeA = aoc::multi_timer("2A");
		auto const resA = timeA.run(part_a, input);

		auto const timeB = aoc::multi_timer("2B");
		auto const resB = timeB.run(part_b, input);

		return nice_output(2, std::to_string(resA), std::to_string(resB));
	}
}