export module day_8;

import std.core;

import common;

namespace aoc
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

	std::vector<Op> parse(std::vector<std::string> const& _input)
	{
		std::vector<Op> ops;
		ops.reserve(_input.size());
		for (auto const& line : _input)
		{
			OpType const type = line[0] == 'n' ? OpType::NOP : line[0] == 'a' ? OpType::ACC : OpType::JMP;
			//int16 const action = util::svtoi<int16>(std::string_view(line.begin() + (line[4] == '+' ? 5 : 4), line.end()));
			int16 const action = std::stoi(line.substr(line[4] == '+' ? 5 : 4)); // somehow this is actually faster than the string view. // even though stoi handles +, it's faster ignoring it here too.
			ops.emplace_back(type, action);
		}

		return ops;
	}

	uint64 partA(std::vector<Op> const& _ops)
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

	usize partB_rec(std::vector<Op> const& _ops, std::vector<usize>& _counts, usize _exe, usize _ip, usize _acc, bool _prevFlipped)
	{
		static constexpr usize sentinel = static_cast<usize>(-1);

		while (_counts[_ip] >= _exe && _ip < _ops.size())
		{
			_counts[_ip] = _exe;
			switch (_ops[_ip].type)
			{
			using enum OpType;
			case NOP:
			{
				if (!_prevFlipped)
				{
					auto const asNop = partB_rec(_ops, _counts, _exe + 1, _ip + 1, _acc, _prevFlipped);
					auto const asJmp = partB_rec(_ops, _counts, _exe + 1, _ip + _ops[_ip].action, _acc, true);
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
					auto const asJmp = partB_rec(_ops, _counts, _exe + 1, _ip + _ops[_ip].action, _acc, _prevFlipped);
					auto const asNop = partB_rec(_ops, _counts, _exe + 1, _ip + 1, _acc, true);
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
			_exe++;
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

	uint64 partB(std::vector<Op> const& _ops)
	{
		std::vector<usize> executionCounts(_ops.size(), std::numeric_limits<usize>::max());

		return partB_rec(_ops, executionCounts, 0, 0, 0, false);
	}

	export auto day_8()
	{
		auto const input = aoc::input(8).to_lines();

		aoc::multi_timer timeP("8 parse into ops");
		auto const ops = timeP.run(parse, input);

		aoc::multi_timer timeA("8A");
		auto const resultA = timeA.run(partA, ops);

		aoc::multi_timer timeB("8B");
		auto const resultB = timeB.run(partB, ops);


		return nice_output(8, std::to_string(resultA), std::to_string(resultB));
	}
}