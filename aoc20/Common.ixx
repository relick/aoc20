export module common;

import std.core;

export using uint32 = uint32_t;
export using int32 = int32_t;
export using uint8 = uint8_t;
export using int8 = int8_t;
export using usize = size_t;
export using isize = ptrdiff_t;

namespace input
{
	size_t get_filesize(std::string const& _filename)
	{
		std::ifstream f(_filename);
		f.seekg(0, std::ios::end);
		return f.tellg();
	}

	std::string get_filename_from_day(int32 _day)
	{
		std::string const dayStr = std::to_string(_day);
		return std::string{ "inputs/" } + (dayStr.size() < 2 ? "0" : "") + dayStr + ".txt";
	}

	export std::string to_string(int32 _day)
	{
		std::string const filename = get_filename_from_day(_day);
		size_t const size = get_filesize(filename);
		std::ifstream f{ filename };
		std::string buffer(size, ' ');
		f.read(&buffer[0], size);
		return buffer;
	}

	export template<std::integral T>
	std::vector<T> to_numbers(int32 _day)
	{
		std::string const filename = get_filename_from_day(_day);
		size_t const size = get_filesize(filename);
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

	export std::vector<std::string> to_lines(int32 _day)
	{
		std::string const filename = get_filename_from_day(_day);
		size_t const size = get_filesize(filename);
		std::ifstream f(filename);
		std::vector<std::string> lines;
		lines.reserve(std::min<usize>(size / 2, 50000)); // reserve but set a reasonable upper limit
		std::string s;
		while (std::getline(f, s))
		{
			lines.push_back(s);
		}
		return lines;
	}
}
namespace util
{
	export template <std::integral T>
	T svtoi(std::string_view const& _sv)
	{
		T output;
		std::from_chars_result res = std::from_chars(_sv.data(), _sv.data() + _sv.size(), output);

		if (res.ec == std::errc::invalid_argument)
		{
			throw std::invalid_argument{ "invalid_argument" };
		}
		else if (res.ec == std::errc::result_out_of_range)
		{
			throw std::out_of_range{ "out_of_range" };
		}

		return output;
	}
}

namespace aoc
{
	export std::string nice_output(int32 _day, std::string const& _partA, std::string const& _partB)
	{
		return std::string{ "Day " } + std::to_string(_day) + "\n--- \nPart A : " + _partA + "\nPart B : " + _partB + "\n\n";
	}
}