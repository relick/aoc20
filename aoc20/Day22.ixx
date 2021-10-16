export module Day22;

import <vector>;
import <string>;

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
		std::vector<intT> p1;
		bool firstLine = true;
		for (auto const& line : util::str_split(player1, '\n'))
		{
			if (firstLine)
			{
				firstLine = false;
				continue;
			}

			p1.emplace_back(Util::QstoiR<intT>(line));
		}

		auto const& player2 = _input[1];
		std::vector<intT> p2;
		firstLine = true;
		for (auto const& line : util::str_split(player2, '\n'))
		{
			if (firstLine)
			{
				firstLine = false;
				continue;
			}

			p2.emplace_back(Util::QstoiR<intT>(line));
		}

		return std::pair(p1, p2);
	}

	auto PartA(std::vector<intT> _p1, std::vector<intT> _p2)
	{
		while (!_p1.empty() && !_p2.empty())
		{
			auto const p1Top = _p1.front();
			_p1.erase(_p1.begin());

			auto const p2Top = _p2.front();
			_p2.erase(_p2.begin());

			if (p1Top > p2Top)
			{
				_p1.emplace_back(p1Top);
				_p1.emplace_back(p2Top);
			}
			else
			{
				_p2.emplace_back(p2Top);
				_p2.emplace_back(p1Top);
			}
		}

		std::vector<intT> const& winner = _p1.empty() ? _p2 : _p1;

		uint64 result{ 0 };
		for (uint64 i = winner.size(); auto const& num : winner)
		{
			result += i-- * num;
		}

		return result;
	}

	enum class Winner
	{
		P1,
		P2,
	};

	auto PartB_Rec(std::vector<intT>& _p1, std::vector<intT>& _p2)
	{
		std::vector<std::vector<intT>> seenBeforeP1;
		std::vector<std::vector<intT>> seenBeforeP2;

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

			auto const p1Top = _p1.front();
			_p1.erase(_p1.begin());

			auto const p2Top = _p2.front();
			_p2.erase(_p2.begin());

			if (p1Top > _p1.size() || p2Top > _p2.size())
			{
				if (p1Top > p2Top)
				{
					_p1.emplace_back(p1Top);
					_p1.emplace_back(p2Top);
				}
				else
				{
					_p2.emplace_back(p2Top);
					_p2.emplace_back(p1Top);
				}
			}
			else
			{
				std::vector<intT> newP1(_p1.begin(), _p1.begin() + p1Top);
				std::vector<intT> newP2(_p2.begin(), _p2.begin() + p2Top);

				auto const winner = PartB_Rec(newP1, newP2);
				if (winner == Winner::P1)
				{
					_p1.emplace_back(p1Top);
					_p1.emplace_back(p2Top);
				}
				else // winner == Winner::P2
				{
					_p2.emplace_back(p2Top);
					_p2.emplace_back(p1Top);
				}
			}
		}

		return _p1.empty() ? Winner::P2 : Winner::P1;
	}

	auto PartB(std::vector<intT> _p1, std::vector<intT> _p2)
	{
		auto const winner = PartB_Rec(_p1, _p2);
		auto const& winningDeck = (winner == Winner::P1) ? _p1 : _p2;

		uint64 result{ 0 };
		for (uint64 i = winningDeck.size(); auto const& num : winningDeck)
		{
			result += i-- * num;
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