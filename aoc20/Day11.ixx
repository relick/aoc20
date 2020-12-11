export module Day11;

import std.core;

import Common;
import AoC;
import Util;

static constexpr usize N = 11;

namespace AoC
{
	enum class SpaceType : uint8
	{
		Floor,
		Empty,
		Occupied,
	};

	struct Space
	{
		SpaceType type{ SpaceType::Floor };
		uint8 adjCount{ 0 };
	};

	Util::Grid<Space> Parse(std::vector<std::string> const& _input)
	{
		Util::Grid<Space> layout(_input[0].size(), _input.size());

		for (usize y = 0; auto const& line : _input)
		{
			for (usize x = 0; char const c : line)
			{
				if (c == 'L')
				{
					layout.at(x, y).type = SpaceType::Empty;
				}
				++x;
			}
			++y;
		}

		return layout;
	}

	usize PartA(Util::Grid<Space> _gridA, Util::Grid<Space> _gridB)
	{
		using enum SpaceType;

		usize const rows = _gridA.height();
		usize const cols = _gridA.width();

		auto* readGrid = &_gridA;
		auto* writeGrid = &_gridB;

		bool notMatching = true;

		while (notMatching)
		{
			notMatching = false;

			usize index = 0;
			for (usize y = 0; y < rows; ++y)
			{
				for (usize x = 0; x < cols; ++x, ++index)
				{
					Space& cur = (*readGrid)[index];
					if (cur.type != Floor)
					{
						uint8 toAddToOther = cur.type == Occupied ? 1 : 0;
						bool const yAboveBottom = y < rows - 1;
						if (x < cols - 1)
						{
							Space& r = (*readGrid)[index + 1];
							if (r.type == Occupied)
							{
								cur.adjCount++;
							}
							r.adjCount += toAddToOther;

							if (yAboveBottom)
							{
								Space& dr = (*readGrid)[index + 1 + cols];
								if (dr.type == Occupied)
								{
									cur.adjCount++;
								}
								dr.adjCount += toAddToOther;
							}
						}
						
						if (x > 0 && yAboveBottom)
						{
							Space& dl = (*readGrid)[index - 1 + cols];
							if (dl.type == Occupied)
							{
								cur.adjCount++;
							}
							dl.adjCount += toAddToOther;
						}

						if (yAboveBottom)
						{
							Space& d = (*readGrid)[index + cols];
							if (d.type == Occupied)
							{
								cur.adjCount++;
							}
							d.adjCount += toAddToOther;
						}

						if (cur.type == Empty && cur.adjCount == 0)
						{
							(*writeGrid)[index].type = Occupied;
							notMatching = true;
						}
						else if (cur.type == Occupied && cur.adjCount >= 4)
						{
							(*writeGrid)[index].type = Empty;
							notMatching = true;
						}
						else
						{
							(*writeGrid)[index].type = cur.type;
						}
						cur.adjCount = 0;
					}
				}
			}

			std::swap(readGrid, writeGrid);
		}

		usize occupied = 0;
		for (auto const& space : *readGrid)
		{
			if (space.type == SpaceType::Occupied)
			{
				++occupied;
			}
		}

		return occupied;
	}

	usize PartB(Util::Grid<Space> _gridA, Util::Grid<Space> _gridB)
	{
		using enum SpaceType;

		usize const rows = _gridA.height();
		usize const cols = _gridA.width();

		auto* readGrid = &_gridA;
		auto* writeGrid = &_gridB;

		bool notMatching = true;

		while (notMatching)
		{
			notMatching = false;

			usize index = 0;
			for (usize y = 0; y < rows; ++y)
			{
				for (usize x = 0; x < cols; ++x, ++index)
				{
					Space& cur = (*readGrid)[index];
					if (cur.type != Floor)
					{
						uint8 toAddToOther = cur.type == Occupied ? 1 : 0;

						// right
						usize const maxX = cols - x;
						usize const maxY = cols * (rows - y);
						{
							usize dx = 1;
							while (dx < maxX)
							{
								Space& r = (*readGrid)[index + dx];
								if (r.type == Floor)
								{
									++dx;
								}
								else
								{
									if (r.type == Occupied)
									{
										cur.adjCount++;
									}
									r.adjCount += toAddToOther;
									break;
								}
							}
						}

						// down right
						{
							usize dx = 1;
							usize dy = cols;
							while (dx < maxX && dy < maxY)
							{
								Space& dr = (*readGrid)[index + dx + dy];
								if (dr.type == Floor)
								{
									++dx;
									dy += cols;
								}
								else
								{
									if (dr.type == Occupied)
									{
										cur.adjCount++;
									}
									dr.adjCount += toAddToOther;
									break;
								}
							}
						}

						// down
						{
							usize dy = cols;
							while (dy < maxY)
							{
								Space& d = (*readGrid)[index + dy];
								if (d.type == Floor)
								{
									dy += cols;
								}
								else
								{
									if (d.type == Occupied)
									{
										cur.adjCount++;
									}
									d.adjCount += toAddToOther;
									break;
								}
							}
						}

						// down left
						{
							isize const minX = -static_cast<isize>(x);
							isize dx = -1;
							usize dy = cols;
							while (dx >= minX && dy < maxY)
							{
								Space& dl = (*readGrid)[index + dx + dy];
								if (dl.type == Floor)
								{
									--dx;
									dy += cols;
								}
								else
								{
									if (dl.type == Occupied)
									{
										cur.adjCount++;
									}
									dl.adjCount += toAddToOther;
									break;
								}
							}
						}

						if (cur.type == Empty && cur.adjCount == 0)
						{
							(*writeGrid)[index].type = Occupied;
							notMatching = true;
						}
						else if (cur.type == Occupied && cur.adjCount >= 5)
						{
							(*writeGrid)[index].type = Empty;
							notMatching = true;
						}
						else
						{
							(*writeGrid)[index].type = cur.type;
						}
						cur.adjCount = 0;
					}
				}
			}

			std::swap(readGrid, writeGrid);
		}

		usize occupied = 0;
		for (auto const& space : *readGrid)
		{
			if (space.type == SpaceType::Occupied)
			{
				++occupied;
			}
		}

		return occupied;
	}

	export std::string Day11()
	{
		auto const input = Benchmark("11 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const layout = Benchmark("11 Parse into Spaces").Run(Parse, input);
		auto const resultA = Benchmark("11A").Run(PartA, layout, layout);
		auto const resultB = Benchmark("11B").Run(PartB, layout, layout);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}