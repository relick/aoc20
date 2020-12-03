export module day_2;

import std.core;

import common;
import str_split;

namespace aoc
{
	std::string part_a(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const lowerBound = util::svtoi<int32>(iter.next());
			auto const upperBound = util::svtoi<int32>(iter.next());
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
		return std::to_string(validPasswords);
	}

	std::string part_b(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const firstIndex = util::svtoi<usize>(iter.next()) - 1u;
			auto const lastIndex = util::svtoi<usize>(iter.next()) - 1u;
			char const checkChar = iter.next()[0];
			std::string_view const testStr = iter.next();

			if ((checkChar == testStr[firstIndex]) ^ (checkChar == testStr[lastIndex]))
			{
				++validPasswords;
			}
		}
		return std::to_string(validPasswords);
	}

	export std::string day2()
	{
		auto const input = aoc::input(2).to_lines();
		return nice_output(2, part_a(input), part_b(input));
	}
}