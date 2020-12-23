export module Day22;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 22;

namespace AoC
{
	using intT = uint8;
	using sizeT = uint8;

	auto Parse(std::vector<std::string> const& _input)
	{
		auto const& player1 = _input[0];
		std::deque<intT> p1;
		bool firstLine = true;
		for (auto const& line : util::str_split(player1, '\n'))
		{
			if (firstLine)
			{
				firstLine = false;
				continue;
			}

			p1.emplace_front(Util::QstoiR<intT>(line));
		}

		auto const& player2 = _input[1];
		std::deque<intT> p2;
		firstLine = true;
		for (auto const& line : util::str_split(player2, '\n'))
		{
			if (firstLine)
			{
				firstLine = false;
				continue;
			}

			p2.emplace_front(Util::QstoiR<intT>(line));
		}

		return std::pair(p1, p2);
	}

	auto PartA(std::deque<intT> _p1, std::deque<intT> _p2)
	{
		while (!_p1.empty() && !_p2.empty())
		{
			auto const p1Top = _p1.back();
			_p1.pop_back();

			auto const p2Top = _p2.back();
			_p2.pop_back();

			if (p1Top > p2Top)
			{
				_p1.emplace_front(p1Top);
				_p1.emplace_front(p2Top);
			}
			else
			{
				_p2.emplace_front(p2Top);
				_p2.emplace_front(p1Top);
			}
		}

		std::deque<intT> const& winner = _p1.empty() ? _p2 : _p1;

		uint64 result{ 0 };
		for (uint64 i = 1; auto const& num : winner)
		{
			result += i++ * num;
		}

		return result;
	}

	enum class Winner
	{
		P1,
		P2,
	};

	auto PartB_Rec(std::deque<intT>& _p1, std::deque<intT>& _p2)
	{
		std::vector<std::deque<intT>> seenBeforeP1;
		std::vector<std::deque<intT>> seenBeforeP2;

		while (!_p1.empty() && !_p2.empty())
		{
			bool seenBefore = false;
			for (auto const& seen : seenBeforeP1)
			{
				if (_p1 == seen)
				{
					seenBefore = true;
					break;
				}
			}
			if (seenBefore)
			{
				for (auto const& seen : seenBeforeP2)
				{
					if (_p2 == seen)
					{
						return Winner::P1;
					}
				}
			}
			seenBeforeP1.emplace_back(_p1);
			seenBeforeP2.emplace_back(_p2);

			auto const p1Top = _p1.back();
			_p1.pop_back();

			auto const p2Top = _p2.back();
			_p2.pop_back();

			if (p1Top > _p1.size() || p2Top > _p2.size())
			{
				if (p1Top > p2Top)
				{
					_p1.emplace_front(p1Top);
					_p1.emplace_front(p2Top);
				}
				else
				{
					_p2.emplace_front(p2Top);
					_p2.emplace_front(p1Top);
				}
			}
			else
			{
				std::deque<intT> newP1;
				std::deque<intT> newP2;

				for (sizeT i = 0; i < p1Top; ++i)
				{
					newP1.emplace_front(_p1[_p1.size() - 1 - i]);
				}
				for (sizeT i = 0; i < p2Top; ++i)
				{
					newP2.emplace_front(_p2[_p2.size() - 1 - i]);
				}

				auto const winner = PartB_Rec(newP1, newP2);
				if (winner == Winner::P1)
				{
					_p1.emplace_front(p1Top);
					_p1.emplace_front(p2Top);
				}
				else // winner == Winner::P2
				{
					_p2.emplace_front(p2Top);
					_p2.emplace_front(p1Top);
				}
			}
		}

		return _p1.empty() ? Winner::P2 : Winner::P1;
	}

	auto PartB(std::deque<intT> _p1, std::deque<intT> _p2)
	{
		auto const winner = PartB_Rec(_p1, _p2);
		auto const& winningDeck = (winner == Winner::P1) ? _p1 : _p2;

		uint64 result{ 0 };
		for (uint64 i = 1; auto const& num : winningDeck)
		{
			result += i++ * num;
		}

		return result;
	}

	export std::string Day22()
	{
		auto const input = Benchmark("22 Read", 1).Run([]() { return Input(N).ToBlankSeparated(); });
		auto const [p1Start, p2Start] = Benchmark("22 Parse").Run(Parse, input);
		auto const resultA = Benchmark("22A").Run(PartA, p1Start, p2Start);
		auto const resultB = Benchmark("22B", 1).Run(PartB, p1Start, p2Start);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}