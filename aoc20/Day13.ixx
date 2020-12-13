export module Day13;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 13;

namespace AoC
{
	struct Bus
	{
		uint64 id;
		uint64 timeUntil;
		uint64 minuteTarget;
	};

	std::pair<uint32, std::vector<Bus>> Parse(std::vector<std::string> const& _input)
	{
		auto const startTime = Util::QstoiR<uint32>(_input[0]);
		std::vector<Bus> buses;
		buses.reserve(_input[1].size());
		for (usize target = 0; auto const& id : util::str_split(_input[1], ','))
		{
			if (id[0] != 'x')
			{
				uint64 const idN = Util::QstoiR<uint64>(id);
				int64 sub = (idN - target);
				while (sub < 0) { sub += idN; }
				buses.emplace_back(idN, 0, sub % idN);
			}
			++target;
		}

		return { startTime, buses, };
	}

	uint64 PartA(uint32 _startTime, std::vector<Bus> _buses)
	{
		for (auto& bus : _buses)
		{
			bus.timeUntil = bus.id - (_startTime % bus.id);
		}
		auto min = std::min_element(_buses.begin(), _buses.end(), [](Bus const& _a, Bus const& _b)
		{
			return _a.timeUntil < _b.timeUntil;
		});
		return min->id * min->timeUntil;
	}

	uint64 PartB(std::vector<Bus> const& _buses)
	{
		uint64 const prod = std::reduce(_buses.begin(), _buses.end(), (uint64)1, [](uint64 _a, Bus const& _b) -> uint64
		{
			return _a * _b.id;
		});

		uint64 cur{ 0 };
		uint64 incr{ 1 };
		for (usize match{ 0 }; match < _buses.size(); cur += incr)
		{
			for (usize i = match; i < _buses.size(); ++i)
			{
				if (cur % _buses[i].id != _buses[i].minuteTarget) { break; }
				incr *= _buses[i].id;
				++match;
			}
		}

		return cur % prod;
	}

	export std::string Day13()
	{
		auto const input = Benchmark("13 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [startTime, buses] = Benchmark("13 Parse into Buses").Run(Parse, input);
		auto const resultA = Benchmark("13A").Run(PartA, startTime, buses);
		auto const resultB = Benchmark("13B").Run(PartB, buses);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}