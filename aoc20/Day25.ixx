export module Day25;

import <vector>;
import <string>;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 25;

namespace AoC
{
	auto Parse(std::vector<std::string> const& _input)
	{
		auto const card = Util::QstoiR<uint64>(_input[0]);
		auto const door = Util::QstoiR<uint64>(_input[1]);
		return std::pair(card, door);
	}

	auto PartA(uint64 const _card, uint64 const _door)
	{
		uint64 cardLoopSize = 0;
		uint64 cardTransform = 1;
		while (cardTransform != _card)
		{
			cardLoopSize++;
			cardTransform *= 7;
			cardTransform %= 20201227;
		}

		uint64 doorLoopSize = 0;
		uint64 doorTransform = 1;
		while (doorTransform != _door)
		{
			doorLoopSize++;
			doorTransform *= 7;
			doorTransform %= 20201227;
		}

		uint64 encryptionCard = 1;
		for (uint64 i = 0; i < cardLoopSize; ++i)
		{
			encryptionCard *= _door;
			encryptionCard %= 20201227;
		}

		uint64 encryptionDoor = 1;
		for (uint64 i = 0; i < doorLoopSize; ++i)
		{
			encryptionDoor *= _card;
			encryptionDoor %= 20201227;
		}

		if (encryptionCard == encryptionDoor)
		{
			return encryptionCard;
		}
		return (uint64)0;
	}

	auto PartB(uint64 const _card, uint64 const _door)
	{
		return 0;
	}

	export std::string Day25()
	{
		auto const input = Benchmark("25 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const [card, door] = Benchmark("25 Parse", 1).Run(Parse, input);
		auto const resultA = Benchmark("25A", 1).Run(PartA, card, door);
		auto const resultB = Benchmark("25B", 1).Run(PartB, card, door);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}