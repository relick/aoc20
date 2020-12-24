export module Day25;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 25;

namespace AoC
{
	auto Parse(std::vector<std::string> const& _input)
	{
		return 0;
	}

	auto PartA(uint64 const& _directions)
	{
		return 0;
	}

	auto PartB(uint64 _blackTiles)
	{
		return 0;
	}

	export std::string Day25()
	{
		auto const input = Benchmark("25 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const output = Benchmark("25 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("25A", 1).Run(PartA, output);
		auto const resultB = Benchmark("25B", 1).Run(PartB, output);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}