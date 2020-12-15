export module Day15;

import std.core;

import Common;
import AoC;
import Util;
import StrSplit;

static constexpr usize N = 15;

namespace AoC
{
	std::vector<uint32> Parse(std::vector<std::string> const& _input)
	{
		std::vector<uint32> nums;
		nums.reserve(_input.size());

		for (auto const& num : util::str_split(_input[0], ','))
		{
			nums.emplace_back(Util::QstoiR<uint32>(num));
		}

		return nums;
	}

	static constexpr uint32 sentinel{ static_cast<uint32>(-1) };

	template<typename T>
	class PODVector
	{
		T* m_data{ nullptr };
		usize m_size{ 0 };
		usize m_reserved{ 0 };

		usize CalculateGrowth(usize const _size)
		{
			usize const geometric = m_reserved + m_reserved / 2;
			if (geometric < _size)
			{
				return _size;
			}
			return geometric;
		}

	public:
		PODVector() {}
		PODVector(usize _size)
		{
			Resize(_size);
		}
		template<std::same_as<T> U>
		PODVector(usize _size, U&& _default)
		{
			Resize(_size, std::forward(_default));
		}

		~PODVector() noexcept
		{
			Clear();
			if (m_data)
			{
				_aligned_free(m_data);
			}
		}

		void Reserve(usize const _size)
		{
			if (_size > m_reserved)
			{
				usize const newSize = CalculateGrowth(_size);
				T* newData = static_cast<T*>(_aligned_malloc(sizeof(T) * newSize, alignof(T)));
				if (m_data)
				{
					std::memcpy(newData, m_data, sizeof(T) * m_size);
					_aligned_free(m_data);
				}
				m_data = newData;
				m_reserved = newSize;
			}
		}

		void Resize(usize const _size, bool const _zero = true)
		{
			Reserve(_size);
			usize const oldSize = m_size;
			m_size = _size;
			if (_zero)
			{
				std::memset(m_data + oldSize, 0, sizeof(T) * (m_size - oldSize));
			}
		}

		void Resize(usize const _size, T const& _default)
		{
			Reserve(_size);
			usize const oldSize = m_size;
			m_size = _size;
			std::fill(m_data + oldSize, m_data + m_size, _default);
		}

		T& PushBack(T const& _val)
		{
			Reserve(m_size + 1);
			m_data[m_size] = _val;
			return m_data[m_size++];
		}
		T const& PopBack()
		{
			return m_data[(m_size--)-1];
		}

		void Clear()
		{
			m_size = 0;
		}

		usize Size() const { return m_size; }

		T& operator[](usize _i)
		{
			return m_data[_i];
		}
		T const& operator[](usize _i) const
		{
			return m_data[_i];
		}
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }
	};

	uint32 RunGame(std::vector<uint32> const& _nums, uint32 _max)
	{
		//std::vector<uint32> whenNumLastSeen;
		//whenNumLastSeen.resize(_max, 0);
		PODVector<uint32> whenNumLastSeen;
		whenNumLastSeen.Resize(_max);
		//uint32* whenNumLastSeen = new uint32[_max]();

		uint32 turnNum{ 0 };
		uint32 prevNum{ sentinel };

		for (usize i = 0; i < _nums.size(); ++i)
		{
			uint32 thisNum = _nums[i];
			if (prevNum != sentinel)
			{
				whenNumLastSeen[prevNum] = turnNum;
			}
			++turnNum;
			prevNum = thisNum;
		}

		while (turnNum < _max)
		{
			uint32 const thisNum = (whenNumLastSeen[prevNum] != 0) * (turnNum - whenNumLastSeen[prevNum]);
			whenNumLastSeen[prevNum] = turnNum++;
			prevNum = thisNum;
		}

		//delete[] whenNumLastSeen;

		return prevNum;
	}

	uint32 PartA(std::vector<uint32> const& _nums)
	{
		return RunGame(_nums, 2020);
	}

	uint32 PartB(std::vector<uint32> const& _nums)
	{
		return RunGame(_nums, 30000000);
	}

	std::pair<uint32, uint32> FullGame(std::vector<uint32> const& _nums)
	{
		uint32* whenNumLastSeen = new uint32[30000000]();

		uint32 turnNum{ 0 };
		uint32 prevNum{ sentinel };

		for (usize i = 0; i < _nums.size(); ++i)
		{
			uint32 thisNum = _nums[i];
			if (prevNum != sentinel)
			{
				whenNumLastSeen[prevNum] = turnNum;
			}
			++turnNum;
			prevNum = thisNum;
		}

		uint32 partA, partB;
		while (turnNum < 2020)
		{
			uint32 const thisNum = (whenNumLastSeen[prevNum] != 0) * (turnNum - whenNumLastSeen[prevNum]);
			whenNumLastSeen[prevNum] = turnNum++;
			prevNum = thisNum;
		}
		partA = prevNum;

		while (turnNum < 30000000)
		{
			uint32 const thisNum = (whenNumLastSeen[prevNum] != 0) * (turnNum - whenNumLastSeen[prevNum]);
			whenNumLastSeen[prevNum] = turnNum++;
			prevNum = thisNum;
		}
		partB = prevNum;

		delete[] whenNumLastSeen;

		return { partA, partB };
	}

	export std::string Day15()
	{
		auto const input = Benchmark("15 Read", 1).Run([]() { return Input(N).ToLines(); });
		auto const nums = Benchmark("15 Parse into Nums").Run(Parse, input);
		auto const resultA = Benchmark("15A").Run(PartA, nums);
		auto const resultB = Benchmark("15B", 10).Run(PartB, nums);
		auto const [pA, pB] = Benchmark("15 Together", 10).Run(FullGame, nums);

		return NiceOutput(N, std::to_string(resultA), std::to_string(resultB));
	}
}