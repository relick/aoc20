export module Day21;

import <vector>;
import <string>;
import <unordered_map>;
import <unordered_set>;
import <algorithm>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 21;

namespace AoC
{
	using intT = int32;
	using sizeT = uint32;

	auto Parse(std::vector<std::string> const& _input)
	{
		sizeT nextID = 0;
		std::unordered_map<std::string, sizeT> stringToID;
		std::vector<std::string> stringPool;
		std::vector<std::unordered_set<sizeT>> allIngredientLists;
		std::unordered_map<sizeT, std::vector<std::unordered_set<sizeT>>> allergenToIngredients;
		for (auto const& line : _input)
		{
			std::unordered_set<uint32> ingredients;
			bool allergenList = false;
			for (auto const& tok : util::str_split(line, ' '))
			{
				if (tok[0] == '(')
				{
					allergenList = true;
					continue;
				}

				if (allergenList)
				{
					auto const [iter, inserted] = stringToID.emplace(tok.substr(0, tok.size() - 1), nextID);
					if (inserted)
					{
						allergenToIngredients[nextID].emplace_back(ingredients);
						stringPool.emplace_back(tok.substr(0, tok.size() - 1));
						nextID++;
					}
					else
					{
						allergenToIngredients[iter->second].emplace_back(ingredients);
					}
				}
				else
				{
					auto const [iter, inserted] = stringToID.emplace(tok, nextID);
					if (inserted)
					{
						ingredients.insert(nextID);
						stringPool.emplace_back(tok);
						nextID++;
					}
					else
					{
						ingredients.insert(iter->second);
					}
				}
			}

			allIngredientLists.emplace_back(ingredients);
		}

		return std::tuple<
			std::vector<std::string>,
			std::vector<std::unordered_set<sizeT>>,
			std::unordered_map<sizeT, std::vector<std::unordered_set<sizeT>>>
			>(stringPool, allIngredientLists, allergenToIngredients);
	}

	auto PartA
	(
		std::vector<std::unordered_set<sizeT>> const& _allIngredientLists,
		std::unordered_map<uint32, std::vector<std::unordered_set<uint32>>> const& _allergenToIngredients
	)
	{
		std::unordered_map<sizeT, sizeT> usedIngredients;
		usedIngredients.reserve(_allergenToIngredients.size());
		std::unordered_set<sizeT> usedAllergens;
		usedAllergens.reserve(_allergenToIngredients.size());
		while (usedIngredients.size() != _allergenToIngredients.size())
		{
			for (auto const& [allergen, lists] : _allergenToIngredients)
			{
				if (!usedAllergens.contains(allergen))
				{
					std::unordered_set<sizeT> output = lists[0];
					for (usize i = 0; i < lists.size(); ++i)
					{
						for (auto ingredientI = output.begin(); ingredientI != output.end();)
						{
							if (!lists[i].contains(*ingredientI) || usedIngredients.contains(*ingredientI))
							{
								ingredientI = output.erase(ingredientI);
							}
							else
							{
								ingredientI++;
							}
						}
					}

					if (output.size() == 1)
					{
						usedIngredients.emplace(*output.begin(), allergen);
						usedAllergens.insert(allergen);
					}
				}
			}
		}

		uint64 timesUsed = 0;
		for (auto const& list : _allIngredientLists)
		{
			for (auto const& ingredient : list)
			{
				if (!usedIngredients.contains(ingredient))
				{
					timesUsed++;
				}
			}
		}

		return std::pair(timesUsed, usedIngredients);
	}

	auto PartB(std::vector<std::string> const& _stringPool, std::unordered_map<sizeT, sizeT> const& _usedIngredients)
	{
		std::vector<std::pair<std::string, sizeT>> allergenNameToIngredient;
		allergenNameToIngredient.reserve(_usedIngredients.size());
		for (auto const& [ingredient, allergen] : _usedIngredients)
		{
			allergenNameToIngredient.emplace_back(_stringPool[allergen], ingredient);
		}

		std::sort(allergenNameToIngredient.begin(), allergenNameToIngredient.end(), [](std::pair<std::string, sizeT> const& _a, std::pair<std::string, sizeT> _b)
		{
			return _a.first < _b.first;
		});

		std::string output;
		for (auto const& [allergenName, ingredient] : allergenNameToIngredient)
		{
			output += _stringPool[ingredient] + ',';
		}
		return output.substr(0, output.size() - 1); // remove last comma
	}

	export std::string Day21()
	{
		auto const input = Benchmark("21 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [stringPool, allIngredientLists, allergenToIngredients] = Benchmark("21 Parse", 1).Run(Parse, input);
		auto const [resultA, usedIngredients] = Benchmark("21A", 1).Run(PartA, allIngredientLists, allergenToIngredients);
		auto const resultB = Benchmark("21B", 1).Run(PartB, stringPool, usedIngredients);

		return NiceOutput(N, std::to_string(resultA), resultB);
	}
}