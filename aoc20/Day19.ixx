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

		virtual bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const = 0;
	};

	struct Terminator : public Rule
	{
		bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const override
		{
			return _sv.size() == 0 ? io_validLens.emplace_back(0), true : false;
		}
	};

	struct Letter : public Rule
	{
		char const m_c;
		Letter(char const _c)
			: m_c{ _c }
		{}

		bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const override
		{
			return !_sv.empty() && _sv[0] == m_c ? io_validLens.emplace_back(1), true : false;
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

		bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const override
		{
			// assume that a termination rule never occurs in an alternation
			if (_sv.empty())
			{
				return false;
			}
			auto const aCheck = m_a->Check(io_validLens, _sv);
			auto const bCheck = m_b->Check(io_validLens, _sv);
			return aCheck || bCheck;
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

		bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const override
		{
			// assume that a termination rule only ever occurs in the second position of a concatenation.
			if (_sv.empty())
			{
				return false;
			}

			std::vector<sizeT> a;
			a.reserve(io_validLens.size());
			auto const aCheck = m_a->Check(a, _sv);
			std::vector<sizeT> b;
			b.reserve(io_validLens.size());
			bool foundAny = false;
			for (auto const& len : a)
			{
				b.clear();
				auto const bCheck = m_b->Check(b, _sv.substr(len));
				for (auto const& bLen : b)
				{
					io_validLens.push_back(len + bLen);
					foundAny = true;
				}
			}

			// if it helps?
			//std::sort(output.begin(), output.end());
			//output.erase(std::unique(output.begin(), output.end()), output.end());

			return foundAny;
		}
	};

	struct Clone : public Rule
	{
		Rule const* m_a;
		Clone(Rule const* _a)
			: m_a{ _a }
		{}

		bool Check(std::vector<sizeT>& io_validLens, std::string_view const& _sv) const override
		{
			return m_a->Check(io_validLens, _sv);
		}

		void Update(Rule* _a)
		{
			m_a = _a;
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
					usize const nextI = Util::QstoiR<usize>(tok);
					if (nextI == _ruleI)
					{
						// we have a self-ref. make an empty clone and fill it in with the final ptr for our rule later
						io_rules[_ruleI] = std::unique_ptr<Rule>(new Clone{ nullptr });
					}

					if (toUpdate)
					{
						inserted = true;
						io_rules.emplace_back(new Concatenate{ toUpdate, MakeRule(io_rules, _oRules, nextI) });
						toUpdate = io_rules.back().get();
					}
					else
					{
						toUpdate = MakeRule(io_rules, _oRules, nextI);
					}
				}
			}
		}

		std::unique_ptr<Rule> selfRefRule = std::move(io_rules[_ruleI]);

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

		if (selfRefRule)
		{
			static_cast<Clone*>(selfRefRule.get())->Update(io_rules[_ruleI].get());
			io_rules.emplace_back(std::move(selfRefRule));
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

	auto NumSatisfyRuleZero(std::vector<std::unique_ptr<Rule>> const& _rules, std::vector<std::string> const& _messages, bool _part1)
	{
		uint64 sum = 0;
		for (auto const& message : _messages)
		{
			std::vector<sizeT> a;
			bool const matched = _rules[0]->Check(a, std::string_view(message));
			if (matched)
			{
				sum += 1;
			}
		}
		return sum;
	}

	export std::string Day19()
	{
		auto const inputA = Benchmark("19 Read A", 1).Run([]() { return Input(N, "part1").ToLines(); });
		auto const [rulesA, messagesA] = Benchmark("19 Parse A").Run(Parse, inputA);
		auto const resultA = Benchmark("19A", 10).Run(NumSatisfyRuleZero, rulesA, messagesA, true);

		auto const inputB = Benchmark("19 Read B", 1).Run([]() { return Input(N, "part2").ToLines(); });
		auto const [rulesB, messagesB] = Benchmark("19 Parse B").Run(Parse, inputB);
		auto const resultB = Benchmark("19B", 10).Run(NumSatisfyRuleZero, rulesB, messagesB, false);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}