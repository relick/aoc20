export module day_7;

import std.core;

import common;
import str_split;

namespace aoc
{
	struct BagData
	{
		std::unordered_map<std::string, usize> colourIDs;
		struct rule
		{
			usize bagID;
			int num;
		};
		std::unordered_multimap<usize, rule> containedInRules;
		std::unordered_multimap<usize, rule> containsRules;
	};

	BagData parse(std::vector<std::string> const& _input)
	{
		usize nextColourID{ 0 };
		BagData bagData;

		enum class line_state
		{
			FirstBagColour,
			SkipAcross,
			NumBags,
			BagColour,
		};

		for (auto const& line : _input)
		{
			line_state state = line_state::FirstBagColour;
			std::string bagColour;
			usize bagID;

			BagData::rule currentRule;
			for (auto const& str : util::str_split(line, ' '))
			{
				switch (state)
				{
				case line_state::FirstBagColour:
				{
					if (str == "bags")
					{
						if (!bagData.colourIDs.contains(bagColour))
						{
							bagID = nextColourID++;
							bagData.colourIDs[bagColour] = bagID;
						}
						else
						{
							bagID = bagData.colourIDs[bagColour];
						}
						state = line_state::SkipAcross;
						bagColour.clear();
						break;
					}
					bagColour += bagColour.empty() ? "" : " ";
					bagColour += str;
					break;
				}
				case line_state::SkipAcross:
				{
					if (!isdigit(str[0]))
					{
						break;
					}
					state = line_state::NumBags;
				}
				case line_state::NumBags:
				{
					currentRule.num = util::svtoi<int>(str);
					state = line_state::BagColour;
					break;
				}
				case line_state::BagColour:
				{
					if (str[0] == 'b' && str[1] == 'a' && str[2] == 'g')
					{
						usize thisBagID;
						if (!bagData.colourIDs.contains(bagColour))
						{
							thisBagID = nextColourID++;
							bagData.colourIDs[bagColour] = thisBagID;
						}
						else
						{
							thisBagID = bagData.colourIDs[bagColour];
						}
						currentRule.bagID = bagID;
						bagData.containedInRules.emplace(thisBagID, currentRule);
						currentRule.bagID = thisBagID;
						bagData.containsRules.emplace(bagID, currentRule);
						state = line_state::SkipAcross;
						bagColour.clear();
						break;
					}
					bagColour += bagColour.empty() ? "" : " ";
					bagColour += str;
					break;
				}
				}
			}
		}

		return bagData;
	}

	uint64 partA(BagData const& _bagData)
	{
		std::unordered_set<usize> bagsContainingShinyGold;
		std::vector<usize> newBagsToCheck;

		newBagsToCheck.push_back(_bagData.colourIDs.at("shiny gold"));
		while (!newBagsToCheck.empty())
		{
			usize const bagID = newBagsToCheck.back();
			newBagsToCheck.pop_back();

			auto const range = _bagData.containedInRules.equal_range(bagID);
			for (auto it = range.first; it != range.second; ++it)
			{
				auto const& rule = it->second;
				if (bagsContainingShinyGold.insert(rule.bagID).second)
				{
					newBagsToCheck.push_back(rule.bagID);
				}
			}
		}
		return bagsContainingShinyGold.size();
	}

	uint64 partB_rec(BagData const& _bagData, usize _id)
	{
		uint64 total = 0;
		auto const range = _bagData.containsRules.equal_range(_id);
		if (range.first == range.second)
		{
			total = 0;
		}
		else
		{
			for (auto it = range.first; it != range.second; ++it)
			{
				auto const& rule = it->second;
				total += rule.num * (1 + partB_rec(_bagData, rule.bagID));
			}
		}
		return total;
	}

	uint64 partB(BagData const& _bagData)
	{
		return partB_rec(_bagData, _bagData.colourIDs.at("shiny gold"));
	}

	export auto day_7()
	{
		auto const input = aoc::input(7).to_lines();

		aoc::multi_timer timeP("7 Make Graph", 50);
		auto const bagData = timeP.run(parse, input);

		aoc::multi_timer timeA("7A");
		auto const resultA = timeA.run(partA, bagData);

		aoc::multi_timer timeB("7B");
		auto const resultB = timeB.run(partB, bagData);


		return nice_output(7, std::to_string(resultA), std::to_string(resultB));
	}
}