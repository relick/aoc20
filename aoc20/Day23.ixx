export module Day23;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 23;

namespace AoC
{
	using intT = uint8;
	using sizeT = uint8;

	using intTB = uint32;
	using sizeTB = uint32;

	template<std::integral I>
	struct CList
	{
		CList<I>* next;
		I val;
	};

	auto Parse(std::string const& _input)
	{
		std::vector<CList<intT>> cups(_input.size());
		std::vector<CList<intTB>> cupsB(1000000);
		usize nextCup = 0;
		for (auto const& c : _input)
		{
			cups[nextCup].val = static_cast<intT>(c - '0');
			cupsB[nextCup++].val = static_cast<intT>(c - '0');
		}

		for (intTB i = (intTB)_input.size() + 1; i <= 1000000; ++i)
		{
			cupsB[i - 1].val = i;
		}

		return std::pair(cups, cupsB);
	}

	template<std::integral I>
	void MakeList(std::vector<CList<I>>& _cups)
	{
		for (usize i = 1; i < _cups.size(); ++i)
		{
			_cups[i - 1].next = &_cups[i];
		}
		_cups[_cups.size() - 1].next = &_cups[0];
	}

	auto PartA(std::vector<CList<intT>> _cups)
	{
		MakeList(_cups);
		CList<intT>* curCup = &_cups[0];
		std::array<CList<intT>*, 3> pickedUp;

		auto const max = [&]() -> intT
		{
			auto max = curCup->val;
			auto head = curCup->next;
			while (head != curCup)
			{
				max = std::max(max, head->val);
				head = head->next;
			}
			return max;
		}();

		for (sizeT r = 0; r < 100; ++r)
		{
			auto destNum = curCup->val;

			// keep track of moving numbers
			pickedUp[0] = curCup->next;
			for (sizeT i = 1; i < 3; ++i)
			{
				pickedUp[i] = pickedUp[i - 1]->next;
			}
			
			// erase moving numbers
			curCup->next = pickedUp[2]->next;
			
			// find dest num
			bool decOnce = true;
			while (decOnce || destNum == pickedUp[0]->val || destNum == pickedUp[1]->val || destNum == pickedUp[2]->val)
			{
				--destNum;
				if (destNum == 0)
				{
					destNum = max;
				}
				decOnce = false;
			}

			// find dest pos
			CList<intT>* destCup = curCup->next;
			while (destCup != curCup)
			{
				if (destCup->val == destNum)
				{
					break;
				}
				destCup = destCup->next;
			}

			// insert 3 back in.
			pickedUp[2]->next = destCup->next;
			destCup->next = pickedUp[0];

			// forward curCup
			curCup = curCup->next;
		}

		while (curCup->val != 1)
		{
			curCup = curCup->next;
		}

		std::string ret;
		auto head = curCup->next;
		while (head != curCup)
		{
			ret += static_cast<char>(head->val + '0');
			head = head->next;
		}

		return ret;
	}

	auto PartB(std::vector<CList<intTB>> _cups)
	{
		MakeList(_cups);
		std::unordered_map<intTB, CList<intTB>*> valToNode;
		valToNode.reserve(_cups.size());
		for (auto& node : _cups)
		{
			valToNode.emplace(node.val, &node);
		}
		CList<intTB>* curCup = &_cups[0];
		std::array<CList<intTB>*, 3> pickedUp;

		auto const max = [&]() -> intTB
		{
			auto max = curCup->val;
			auto head = curCup->next;
			while (head != curCup)
			{
				max = std::max(max, head->val);
				head = head->next;
			}
			return max;
		}();

		for (sizeTB r = 0; r < 10000000; ++r)
		{
			auto destNum = curCup->val;

			// keep track of moving numbers
			pickedUp[0] = curCup->next;
			pickedUp[1] = pickedUp[0]->next;
			pickedUp[2] = pickedUp[1]->next;

			// erase moving numbers
			curCup->next = pickedUp[2]->next;

			// find dest num
			bool decOnce = true;
			while (decOnce || destNum == pickedUp[0]->val || destNum == pickedUp[1]->val || destNum == pickedUp[2]->val)
			{
				--destNum;
				if (destNum == 0)
				{
					destNum = max;
				}
				decOnce = false;
			}

			// find dest pos
			CList<intTB>* destCup = valToNode[destNum];

			// insert 3 back in.
			pickedUp[2]->next = destCup->next;
			destCup->next = pickedUp[0];

			// forward curCup
			curCup = curCup->next;
		}

		curCup = valToNode[1];

		return (uint64) curCup->next->next->val * curCup->next->val;
	}

	export std::string Day23()
	{
		auto const input = Benchmark("23 Read", 1).Run([]() { return Input(N).ToString(); });
		auto const [cupsStart, cupsStartB] = Benchmark("23 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("23A").Run(PartA, cupsStart);
		auto const resultB = Benchmark("23B", 10).Run(PartB, cupsStartB);

		return NiceOutput(N, resultA, std::to_string(resultB));
	}
}