export module Day8;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 8;

namespace AoC
{
	enum class OpType : uint8
	{
		NOP,
		ACC,
		JMP,
	};

	struct Op
	{
		OpType type;
		// oof ow 1 byte padding
		int16 action;
	};

	std::vector<Op> Parse(std::vector<std::string> const& _input)
	{
		std::vector<Op> ops;
		ops.reserve(_input.size());

		for (auto const& line : _input)
		{
			OpType const type = line[0] == 'n' ? OpType::NOP : line[0] == 'a' ? OpType::ACC : OpType::JMP;
			int16 const action = Util::QstoiR<int16>(line);
			ops.emplace_back(type, action);
		}

		return ops;
	}

	uint64 PartA(std::vector<Op> const& _ops)
	{
		std::vector<char> visited(_ops.size(), false);

		usize ip = 0;
		int64 acc = 0;
		while (!visited[ip])
		{
			visited[ip] = true;
			switch (_ops[ip].type)
			{
			using enum OpType;
			case NOP:
			{
				++ip;
				break;
			}
			case ACC:
			{
				acc += _ops[ip].action;
				++ip;
				break;
			}
			case JMP:
			{
				ip += _ops[ip].action;
				break;
			}
			}
		}

		return acc;
	}

	usize PartBRecursion(std::vector<Op> const& _ops, std::vector<char>& _visited, usize _ip, usize _acc, bool _prevFlipped)
	{
		static constexpr usize sentinel = static_cast<usize>(-1);

		while (_ip < _ops.size() && !_visited[_ip])
		{
			_visited[_ip] = true;
			switch (_ops[_ip].type)
			{
			using enum OpType;
			case NOP:
			{
				if (!_prevFlipped)
				{
					auto const asNop = PartBRecursion(_ops, _visited, _ip + 1, _acc, _prevFlipped);
					auto const asJmp = PartBRecursion(_ops, _visited, _ip + _ops[_ip].action, _acc, true);
					if (asNop != sentinel)
					{
						return asNop;
					}
					else if (asJmp != sentinel)
					{
						return asJmp;
					}
					else
					{
						return sentinel;
					}
				}
				else
				{
					++_ip;
				}
				break;
			}
			case ACC:
			{
				_acc += _ops[_ip].action;
				++_ip;
				break;
			}
			case JMP:
			{
				if (!_prevFlipped)
				{
					auto const asJmp = PartBRecursion(_ops, _visited, _ip + _ops[_ip].action, _acc, _prevFlipped);
					auto const asNop = PartBRecursion(_ops, _visited, _ip + 1, _acc, true);
					if (asNop != sentinel)
					{
						return asNop;
					}
					else if (asJmp != sentinel)
					{
						return asJmp;
					}
					else
					{
						return sentinel;
					}
				}
				else
				{
					_ip += _ops[_ip].action;
				}
				break;
			}
			}
		}

		if (_ip >= _ops.size())
		{
			return _acc;
		}
		else
		{
			return sentinel; // failure, we revisited
		}
	}

	uint64 PartB(std::vector<Op> const& _ops)
	{
		std::vector<char> visited(_ops.size(), false);

		return PartBRecursion(_ops, visited, 0, 0, false);
	}

	export template<>
	std::string Day<N>()
	{
		auto const input = Benchmark("8 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const ops = Benchmark("8 Parse into Ops").Run(Parse, input);
		auto const resultA = Benchmark("8A").Run(PartA, ops);
		auto const resultB = Benchmark("8B").Run(PartB, ops);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}