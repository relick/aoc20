export module Day2;

import <vector>;
import <string>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 2;

namespace AoC
{
	usize PartA(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const lowerBound = Util::QstoiR<int32>(iter.next());
			auto const upperBound = Util::QstoiR<int32>(iter.next());
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

	usize PartB(std::vector<std::string> const& _input)
	{
		usize validPasswords{};
		for (auto const& line : _input)
		{
			auto range = util::str_split(line, [](char const& _c) { return _c == '-' || _c == ' ' || _c == ':'; });
			auto iter = range.begin();

			auto const firstIndex = Util::QstoiR<usize>(iter.next()) - 1u;
			auto const lastIndex = Util::QstoiR<usize>(iter.next()) - 1u;
			char const checkChar = iter.next()[0];
			std::string_view const testStr = iter.next();

			if ((checkChar == testStr[firstIndex]) ^ (checkChar == testStr[lastIndex]))
			{
				++validPasswords;
			}
		}
		return validPasswords;
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("2 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const resultA = Benchmark("2A").Run(PartA, input);
		auto const resultB = Benchmark("2B").Run(PartB, input);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}