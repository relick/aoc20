export module Common;

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
	export template<std::predicate<char> DelimFunction>
	class str_split_iter
	{
		std::string const& m_s;
		DelimFunction m_fn;

		using str_iter = std::string::const_iterator; // for now

		str_iter m_currentBegin;
		str_iter m_currentEnd;

		void next_nondelim(str_iter& io_iter)
		{
			while (io_iter != m_s.end() && m_fn(*io_iter))
			{
				++io_iter;
			}
		}

		str_iter find_next()
		{
			auto current = m_currentEnd;
			next_nondelim(current);

			while (current != m_s.end() && !m_fn(*current))
			{
				++current;
			}

			return current;
		}

	public:
		str_split_iter(std::string const& _s, DelimFunction const& _fn, str_iter const& _start)
			: m_s{ _s }
			, m_fn{ _fn }
			, m_currentBegin{ _start }
			, m_currentEnd{ _start }
		{
			operator++();
		}

		str_split_iter& operator++()
		{
			auto oldEnd = m_currentEnd;
			next_nondelim(oldEnd);
			m_currentEnd = find_next();
			m_currentBegin = oldEnd;

			return *this;
		}

		str_split_iter operator++(int)
		{
			str_split_iter clone{ *this };
			operator++();
			return clone;
		}

		std::string_view operator*()
		{
			return std::string_view(m_currentBegin, m_currentEnd);
		}

		bool operator!=(str_split_iter const& _b)
		{
			return m_currentBegin != _b.m_currentBegin; // assumed ends are equal if begins are equal.
		}
	};

	export template<std::predicate<char> DelimFunction>
	class str_split_range
	{
		std::string const& m_s;
		DelimFunction m_fn;

	public:
		using iterator = str_split_iter<DelimFunction>;
		using difference_type = isize;
		using size_type = usize;
		using value_type = std::string_view;
		using pointer = std::string_view*;
		using reference = std::string_view&;

		str_split_range(std::string const& _s, DelimFunction&& _fn)
			: m_s{ _s }
			, m_fn{ _fn }
		{}

		iterator begin()
		{
			return iterator(m_s, m_fn, m_s.begin());
		}

		iterator end()
		{
			return iterator(m_s, m_fn, m_s.end());
		}
	};

	export template<std::predicate<char> DelimFunction>
	str_split_range<DelimFunction> str_split(std::string const& _s, DelimFunction&& _fn)
	{
		return { _s, std::forward<DelimFunction>(_fn) };
	}

	namespace detail
	{
		export struct char_is_delim
		{
			char m_delim;
			char_is_delim(char _delim) : m_delim{ _delim } {}
			bool operator()(char const& _c) { return _c == m_delim; }
		};
	}

	export str_split_range<detail::char_is_delim> str_split(std::string const& _s, char _delim)
	{
		return { _s, detail::char_is_delim(_delim) };
	}

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