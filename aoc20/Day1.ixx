export module Day1;

import std.core;

import Common;
import AoC;

static constexpr usize N = 1;

namespace AoC
{
	int32 PartA(std::vector<int32> const& _input)
	{
		std::array<bool, 2020> a{ false };
		for (int32 const num : _input)
		{
			if (num < 2020)
			{
				if (a[2020 - num])
				{
					return num * (2020 - num);
				}
				else
				{
					a[num] = true;
				}
			}
		}

		return 0;
	}

	int32 PartB(std::vector<int32> const& _input)
	{
		for (usize i = 0; i < _input.size(); ++i)
		{
			for (usize j = i + 1; j < _input.size(); ++j)
			{
				int32 const iandj = _input[i] + _input[j];
				if (iandj <= 2020)
				{
					for (usize k = j + 1; k < _input.size(); ++k)
					{
						if ((iandj + _input[k]) == 2020)
						{
							return _input[i] * _input[j] * _input[k];
						}
					}
				}
			}
		}

		return 0;
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("1 Read+Parse", 1).Run([]() { return Input(N).ToNumbers<int32>(); });
		auto const resultA = Benchmark("1A").Run(PartA, input);
		auto const resultB = Benchmark("1B").Run(PartB, input);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}