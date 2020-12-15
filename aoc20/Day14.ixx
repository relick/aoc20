export module Day14;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 14;

namespace AoC
{
	// 36 bits
	static constexpr uint64 all36 = 68719476736L - 1L;
	struct Bitmask
	{
		uint64 zeroes{ all36 }; // and with value
		uint64 ones{ 0 }; // or with value
		std::array<bool, 36> floating{ false }; // both valuese
		uint64 floatMask{ all36 };
		usize numX{ 0 };
	};

	struct Mov
	{
		uint64 addr{ 0 };
		uint64 val{ 0 };
		usize mask{ 0 };
	};

	std::array<uint64, 36> n;
	std::array<uint64, 36> nInv;

	struct MemoryMachine
	{
		std::vector<Bitmask> masks;
		uint64 curMask{ 0 };
		std::unordered_map<uint64, uint64> memory;
		uint64 sum{ 0 };
		std::vector<uint64> addressScratch;

		void SetMask(uint64 _mask) { curMask = _mask; }

		void MovR(uint8 _i, uint64 _baseAddr, uint64 _val)
		{
			if (_i >= 36)
			{
				//if (memory.insert(_baseAddr).second)
				{
					sum += _val;
				}
				return;
			}
			if (masks[curMask].floating[_i])
			{
				MovR(_i + 1, _baseAddr & nInv[_i], _val);
				MovR(_i + 1, _baseAddr | n[_i], _val);
			}
			else
			{
				MovR(_i + 1, _baseAddr, _val);
			}
		}

		void Generate(uint8 _i, uint64 _baseAddr)
		{
			if (_i >= 36)
			{
				addressScratch.push_back(_baseAddr);
				return;
			}
			if (masks[curMask].floating[_i])
			{
				auto const n = (1LL << _i);
				Generate(_i + 1, _baseAddr & (~n));
				Generate(_i + 1, _baseAddr | n);
			}
			else
			{
				Generate(_i + 1, _baseAddr);
			}
		}

		void Mov(uint64 _baseAddr, uint64 _val)
		{
			_baseAddr |= masks[curMask].ones;
			/*addressScratch.clear();
			Generate(0, _baseAddr);
			
			for (auto const& addr : addressScratch)
			{
				if (memory.insert(addr).second)
				{
					sum += _val;
				}
			}*/

			//MovR(0, _baseAddr, _val);

			usize const numAddr = 1LL << masks[curMask].numX;
			uint64 floatMask = masks[curMask].floatMask;
			_baseAddr &= floatMask;
			for (usize i = 0; i < numAddr; ++i)
			{
				uint64 addr = _baseAddr | (~floatMask & all36);
				memory[addr] = _val;
				floatMask = (floatMask + 1LL) | masks[curMask].floatMask;
			}
		}
	};

	std::pair<MemoryMachine, std::vector<Mov>> Parse(std::vector<std::string> const& _input)
	{
		std::generate(n.begin(), n.end(), [i = 0]() mutable { return (1LL << i++); });
		std::generate(nInv.begin(), nInv.end(), [i = 0]() mutable { return ~(1LL << i++); });

		MemoryMachine machine;
		std::vector<Mov> movs;
		machine.masks.reserve(_input.size());
		movs.reserve(_input.size());

		for (auto const& line : _input)
		{
			if (line[1] == 'a')
			{
				// mask
				usize const end = line.size() - 1;
				Bitmask newMask;
				for (uint64 i = 0; i < 36; ++i)
				{
					if (line[end - i] == '0')
					{
						newMask.zeroes -= 1LL << i;
					}
					else if (line[end - i] == '1')
					{
						newMask.ones += 1LL << i;
					}
					else
					{
						newMask.floating[i] = true;
						newMask.floatMask -= 1LL << i;
						newMask.numX++;
					}
				}
				machine.masks.push_back(newMask);
			}
			else
			{
				// mem
				movs.push_back({
					.addr = Util::QstoiL<uint64>(std::string_view(line.begin() + 4, line.end())),
					.val = Util::QstoiR<uint64>(line),
					.mask = machine.masks.size() - 1,
				});
			}
		}

		return { machine, movs, };
	}

	uint64 PartA(MemoryMachine const& _machine, std::vector<Mov> const& _movs)
	{
		std::unordered_map<uint64, uint64> memory;
		memory.reserve(_movs.size());

		for (auto const& mov : _movs)
		{
			memory[mov.addr] = (mov.val & _machine.masks[mov.mask].zeroes) | _machine.masks[mov.mask].ones;
		}

		uint64 sum{ 0 };
		for (auto const& [addr, val] : memory)
		{
			sum += val;
		}

		return sum;
	}

	uint64 PartB(MemoryMachine& _machine, std::vector<Mov> const& _movs)
	{
		_machine.memory.clear();
		_machine.memory.reserve(_movs.size());
		_machine.sum = 0;

		for (auto const& mov : _movs)
		{
			_machine.SetMask(mov.mask);
			_machine.Mov(mov.addr, mov.val);
		}

		for (auto const& [_, val] : _machine.memory)
		{
			_machine.sum += val;
		}

		return _machine.sum;
	}

	export std::string Day14()
	{
		auto const input = Benchmark("14 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto [machine, movs] = Benchmark("14 Parse into Masks and Movs").Run(Parse, input);
		auto const resultA = Benchmark("14A").Run(PartA, machine, movs);
		//std::reverse(movs.begin(), movs.end());
		auto const resultB = Benchmark("14B", 100).Run(PartB, machine, movs);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}