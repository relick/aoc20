export module Common;

import std.core;

export using uint32 = uint32_t;
export using int32 = int32_t;
export using uint8 = uint8_t;
export using int8 = int8_t;
export using usize = size_t;
export using isize = ptrdiff_t;

namespace aoc
{
	size_t GetFileSize(std::string const& _filename)
	{
		std::ifstream f(_filename);
		f.seekg(0, std::ios::end);
		return f.tellg();
	}

	std::string GetFilenameFromDay(int32 _day)
	{
		std::string const dayStr = std::to_string(_day);
		return std::string{ "inputs/" } + (dayStr.size() < 2 ? "0" : "") + dayStr + ".txt";
	}

	export std::string LoadToString(int32 _day)
	{
		std::string const filename = GetFilenameFromDay(_day);
		size_t const size = GetFileSize(filename);
		std::ifstream f{ filename };
		std::string buffer(size, ' ');
		f.read(&buffer[0], size);
		return buffer;
	}

	export template<std::integral T>
	std::vector<T> LoadToNumbers(int32 _day)
	{
		std::string const filename = GetFilenameFromDay(_day);
		size_t const size = GetFileSize(filename);
		std::ifstream f(filename);
		std::vector<T> nums;
		nums.reserve(std::min<usize>(size / 2, 50000)); // reserve but set a reasonable upper limit
		while (!f.eof())
		{
			T i;
			if (f >> i)
			{
				nums.push_back(i);
			}
		}
		return nums;
	}

	export std::string NiceOutput(int32 _day, std::string const& _partA, std::string const& _partB)
	{
		return std::string{ "Day " } + std::to_string(_day) + "\n-- - \nPart A : " + _partA + "\nPart B : " + _partB + "\n\n";
	}
}