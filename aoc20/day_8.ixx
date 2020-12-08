export module day_8;

import std.core;

import common;

namespace aoc
{
	uint64 partA(std::vector<std::string> const& _input)
	{
		std::vector<char> visited(_input.size(), false);

		usize ip = 0;
		int64 acc = 0;
		while (!visited[ip])
		{
			visited[ip] = true;
			if (_input[ip][0] == 'n') // nop
			{
				++ip;
			}
			else if (_input[ip][0] == 'a') // acc
			{
				acc += util::svtoi<int64>(std::string_view(_input[ip].begin() + (_input[ip][4] == '+' ? 5 : 4), _input[ip].end()));
				++ip;
			}
			else // jmp
			{
				ip += util::svtoi<isize>(std::string_view(_input[ip].begin() + (_input[ip][4] == '+' ? 5 : 4), _input[ip].end()));
			}
		}

		return acc;
	}

	usize partB_rec(std::vector<std::string> const& _input, std::vector<usize>& _counts, usize _exe, usize _ip, usize _acc, bool _prevFlipped)
	{
		static constexpr usize sentinel = static_cast<usize>(-1);
		if (_ip >= _input.size())
		{
			return _acc;
		}

		if (_counts[_ip] >= _exe)
		{
			_counts[_ip] = _exe;
			if (_input[_ip][0] == 'n') // nop
			{
				auto const asNop = partB_rec(_input, _counts, _exe + 1, _ip + 1, _acc, _prevFlipped);
				if (!_prevFlipped)
				{
					usize const jmpDist = util::svtoi<isize>(std::string_view(_input[_ip].begin() + (_input[_ip][4] == '+' ? 5 : 4), _input[_ip].end()));
					auto const asJmp = partB_rec(_input, _counts, _exe + 1, _ip + jmpDist, _acc, true);
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
					return asNop;
				}
			}
			else if (_input[_ip][0] == 'a') // acc
			{
				_acc += util::svtoi<int64>(std::string_view(_input[_ip].begin() + (_input[_ip][4] == '+' ? 5 : 4), _input[_ip].end()));
				return partB_rec(_input, _counts, _exe + 1, _ip + 1, _acc, _prevFlipped);
			}
			else // jmp
			{
				usize const jmpDist = util::svtoi<isize>(std::string_view(_input[_ip].begin() + (_input[_ip][4] == '+' ? 5 : 4), _input[_ip].end()));
				auto const asJmp = partB_rec(_input, _counts, _exe + 1, _ip + jmpDist, _acc, _prevFlipped);
				if (!_prevFlipped)
				{
					auto const asNop = partB_rec(_input, _counts, _exe + 1, _ip + 1, _acc, true);
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
					return asJmp;
				}
			}
		}
		else
		{
			return sentinel; // failure, we revisited
		}
	}

	uint64 partB(std::vector<std::string> const& _input)
	{
		std::vector<usize> executionCounts(_input.size(), std::numeric_limits<usize>::max());

		return partB_rec(_input, executionCounts, 0, 0, 0, false);
	}

	export auto day_8()
	{
		auto const input = aoc::input(8).to_lines();

		aoc::multi_timer timeA("8A");
		auto const resultA = timeA.run(partA, input);

		aoc::multi_timer timeB("8B");
		auto const resultB = timeB.run(partB, input);


		return nice_output(8, std::to_string(resultA), std::to_string(resultB));
	}
}