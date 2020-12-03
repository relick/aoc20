export module str_split;

import std.core;

import common;

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
}