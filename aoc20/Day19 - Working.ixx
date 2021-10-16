export module Day19;

import <vector>;
import <string>;
import <memory>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 19;

namespace AoC
{
	using intT = int64;
	using sizeT = usize;

	struct Rule
	{
		virtual ~Rule() {}

		virtual std::pair<bool, sizeT> Check(std::string_view const& _sv) const = 0;
		virtual std::pair<bool, sizeT> Check2(std::string_view const& _sv) const { return { false, 0 }; }
	};

	struct Terminator : public Rule
	{
		std::pair<bool, sizeT> Check(std::string_view const& _sv) const override
		{
			return { _sv.size() == 0, 0 };
		}
	};

	struct Letter : public Rule
	{
		char const m_c;
		Letter(char const _c)
			: m_c{ _c }
		{}

		std::pair<bool, sizeT> Check(std::string_view const& _sv) const override
		{
			return { !_sv.empty() && _sv[0] == m_c, 1 };
		}
	};

	struct Alternate : public Rule
	{
		Rule const* m_a;
		Rule const* m_b;
		Alternate(Rule const* _a, Rule const* _b)
			: m_a{ _a }
			, m_b{ _b }
		{}

		std::pair<bool, sizeT> Check(std::string_view const& _sv) const override
		{
			{
				auto const [aOkay, aLength] = m_a->Check(_sv);
				if (aOkay)
				{
					return { true, aLength };
				}
			}
			return m_a->Check2(_sv);
		}

		std::pair<bool, sizeT> Check2(std::string_view const& _sv) const override
		{
			{
				auto const [bOkay, bLength] = m_b->Check(_sv);
				if (bOkay)
				{
					return { true, bLength };
				}
			}
			return m_b->Check2(_sv);
		}
	};

	struct Concatenate : public Rule
	{
		Rule const* m_a;
		Rule const* m_b;
		Concatenate(Rule const* _a, Rule const* _b)
			: m_a{ _a }
			, m_b{ _b }
		{}

		std::pair<bool, sizeT> Check(std::string_view const& _sv) const override
		{
			auto const [aOkay, aLength] = m_a->Check(_sv);
			if (aOkay)
			{
				auto const [bOkay, bLength] = m_b->Check(_sv.substr(aLength));
				if (bOkay)
				{
					return { true, aLength + bLength };
				}
				
				auto const [b2Okay, b2Length] = m_b->Check2(_sv.substr(aLength));
				if (b2Okay)
				{
					return { true, aLength + b2Length };
				}
			}
			
			auto const [a2Okay, a2Length] = m_a->Check2(_sv);
			if (a2Okay)
			{
				auto const [bOkay, bLength] = m_b->Check(_sv.substr(a2Length));
				if (bOkay)
				{
					return { true, a2Length + bLength };
				}

				auto const [b2Okay, b2Length] = m_b->Check2(_sv.substr(a2Length));
				if (b2Okay)
				{
					return { true, a2Length + b2Length };
				}
			}

			return { false, 0, };
		}
	};

	struct Clone : public Rule
	{
		Rule const* m_a;
		Clone(Rule const* _a)
			: m_a{ _a }
		{}

		std::pair<bool, sizeT> Check(std::string_view const& _sv) const override
		{
			return m_a->Check(_sv);
		}

		std::pair<bool, sizeT> Check2(std::string_view const& _sv) const override
		{
			return m_a->Check2(_sv);
		}
	};

	Rule* MakeRule(std::vector<std::unique_ptr<Rule>>& io_rules, std::vector<std::string> const& _oRules, usize _ruleI)
	{
		{
			Rule* existingRule = io_rules[_ruleI].get();
			if (existingRule)
			{
				return existingRule;
			}
		}
		bool alternate = false;
		Rule* a{ nullptr };
		Rule* b{ nullptr };
		bool inserted = false;
		for (auto const& tok : util::str_split(_oRules[_ruleI], ' '))
		{
			if (tok[0] == '|')
			{
				if (alternate)
				{
					std::cout << "alternated twice!\n\n";
				}
				alternate = true;
			}
			else
			{
				Rule*& toUpdate = alternate ? b : a;
				if (tok[0] == '"')
				{
					inserted = true;
					io_rules.emplace_back(new Letter{ tok[1] });
					if (toUpdate)
					{
						io_rules.emplace_back(new Concatenate{ toUpdate, io_rules.back().get() });
					}
					toUpdate = io_rules.back().get();
				}
				else
				{
					if (!toUpdate)
					{
						toUpdate = MakeRule(io_rules, _oRules, Util::QstoiR<usize>(tok));
					}
					else
					{
						inserted = true;
						io_rules.emplace_back(new Concatenate{ toUpdate, MakeRule(io_rules, _oRules, Util::QstoiR<usize>(tok)) });
						toUpdate = io_rules.back().get();
					}
				}
			}
		}

		if (alternate)
		{
			io_rules[_ruleI] = std::unique_ptr<Rule>{ new Alternate{a, b} };
		}
		else if(inserted)
		{
			io_rules[_ruleI] = std::move(io_rules.back());
			io_rules.pop_back();
		}
		else
		{
			io_rules[_ruleI] = std::unique_ptr<Rule>{ new Clone{a} };
		}
		return io_rules[_ruleI].get();
	}

	auto Parse(std::vector<std::string> const& _input)
	{
		std::vector<std::string> orderedRules;
		std::vector<std::string> messages;

		bool isRules = true;
		for (auto const& line : _input)
		{
			if (line.empty())
			{
				isRules = false;
				continue;
			}

			if (isRules)
			{
				usize const ruleID = Util::QstoiL<usize>(line);
				if (orderedRules.size() <= ruleID)
				{
					orderedRules.resize(ruleID + 1);
				}
				orderedRules[ruleID] = std::string(std::string_view(std::find(line.begin(), line.end(), ':') + 2, line.end()));
			}
			else
			{
				messages.emplace_back(line);
			}
		}

		std::vector<std::unique_ptr<Rule>> rules(orderedRules.size());
		
		for (usize i = 0; i < orderedRules.size(); ++i)
		{
			MakeRule(rules, orderedRules, i);
		}

		rules.emplace_back(new Terminator{});
		Rule* terminator = rules.back().get();
		for (usize i = 0; i < orderedRules.size(); ++i)
		{
			rules.emplace_back(std::move(rules[i]));
			rules[i] = std::unique_ptr<Rule>(new Concatenate{ rules.back().get(), terminator });
		}

		return std::pair(std::move(rules), messages);
	}

	auto PartA(std::vector<std::unique_ptr<Rule>> const& _rules, std::vector<std::string> const& _messages)
	{
		uint64 sum = 0;
		for (auto const& message : _messages)
		{
			sum += _rules[0]->Check(std::string_view(message)).first;
		}
		return sum;
	}

	auto PartB(std::vector<std::unique_ptr<Rule>> const& _rules, std::vector<std::string> const& _messages)
	{
		return 0;
	}

	export std::string Day19()
	{
		auto const input = Benchmark("19 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [rules, messages] = Benchmark("19 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("19A", 1).Run(PartA, rules, messages);
		auto const resultB = Benchmark("19B", 1).Run(PartB, rules, messages);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}