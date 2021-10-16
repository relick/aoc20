export module Day16;

import <vector>;
import <string>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 16;

namespace AoC
{
	using intT = uint64;
	
	struct Bound
	{
		intT min;
		intT max;
	};

	struct Rule
	{
		std::string name;
		std::vector<Bound> bounds;
	};

	std::tuple<std::vector<Rule>, std::vector<intT>, std::vector<std::vector<intT>>> Parse(std::vector<std::string> const& _input)
	{
		std::vector<Rule> rules;
		int state = 0;
		std::vector<intT> myTicket;
		std::vector<std::vector<intT>> nearbyTickets;
		for (auto const& line : _input)
		{
			if (line.empty())
			{
				state++;
			}
			else
			{
				switch (state)
				{
				case 0:
				{
					auto splitRange = util::str_split(line, [](char const _c) { return _c == ':' || _c == ' '; });
					auto splitI = splitRange.begin();
					Rule newRule;
					newRule.name = std::string(splitI.next());
					while (splitI != splitRange.end())
					{
						auto boundsRange = util::str_split(splitI.next(), '-');
						auto boundsI = boundsRange.begin();
						newRule.bounds.emplace_back();
						newRule.bounds.back().min = Util::QstoiR<intT>(boundsI.next());
						newRule.bounds.back().max = Util::QstoiR<intT>(boundsI.next());
						++splitI;
					}
					rules.push_back(newRule);
					break;
				}
				case 1:
				{
					if (line[0] == 'y')
					{
						continue;
					}
					for (auto const& num : util::str_split(line, ','))
					{
						myTicket.emplace_back(Util::QstoiR<intT>(num));
					}
					break;
				}
				case 2:
				{
					if (line[0] == 'n')
					{
						continue;
					}
					nearbyTickets.emplace_back();
					for (auto const& num : util::str_split(line, ','))
					{
						nearbyTickets.back().emplace_back(Util::QstoiR<intT>(num));
					}
					break;
				}
				}
			}
		}

		return { rules, myTicket, nearbyTickets, };
	}

	std::pair<intT, intT> Solution(std::vector<Rule> const& _rules, std::vector<intT> const& _myTicket, std::vector<std::vector<intT>> const& _nearbyTickets)
	{
		intT partA{ 0 }, partB{ 1 };

		std::vector<std::vector<intT>> validTickets;
		validTickets.reserve(_nearbyTickets.size());

		// Part A
		for (auto const& ticket : _nearbyTickets)
		{
			bool validTicket = true;
			for (auto const& num : ticket)
			{
				bool valid = false;
				for (auto const& rule : _rules)
				{
					for (auto const& bound : rule.bounds)
					{
						if (num >= bound.min && num <= bound.max)
						{
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					partA += num;
					validTicket = false;
				}
			}
			if (validTicket)
			{
				validTickets.push_back(ticket);
			}
		}

		validTickets.push_back(_myTicket);

		// Part B
		static constexpr usize sentinel = static_cast<usize>(-1);
		std::vector<usize> colRules(_rules.size(), sentinel);
		bool done = false;
		while (!done)
		{
			usize rulesWith0Cols = 0;
			for (usize ruleI = 0; ruleI < _rules.size(); ++ruleI)
			{
				usize colsValid = 0;
				usize workingCol;
				for (usize colI = 0; colI < colRules.size(); ++colI)
				{
					if (colRules[colI] == sentinel)
					{
						bool colValid = true;
						for (auto const& ticket : validTickets)
						{
							bool valid = false;
							for (auto const& bound : _rules[ruleI].bounds)
							{
								if (ticket[colI] >= bound.min && ticket[colI] <= bound.max)
								{
									valid = true;
									break;
								}
							}
							if (!valid)
							{
								colValid = false;
								break;
							}
						}
						if (colValid)
						{
							colsValid++;
							workingCol = colI;
						}
					}
				}

				if (colsValid == 1)
				{
					colRules[workingCol] = ruleI;
					if (ruleI >= 0 && ruleI <= 5)
					{
						// departure
						partB *= _myTicket[workingCol];
					}
				}
				else if (colsValid == 0)
				{
					rulesWith0Cols++;
				}
			}

			if (rulesWith0Cols == _rules.size())
			{
				done = true;
			}
		}

		return { partA, partB, };
	}

	export std::string Day16()
	{
		auto const input = Benchmark("16 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [rules, myTicket, nearbyTickets] = Benchmark("16 Parse", 1).Run(Parse, input);
		auto const [resultA, resultB] = Benchmark("16 Solution", 1).Run(Solution, rules, myTicket, nearbyTickets);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}